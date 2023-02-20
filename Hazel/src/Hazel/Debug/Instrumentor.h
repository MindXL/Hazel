#pragma once

namespace Hazel
{
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0) { }

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_OutputStream.open(filepath, std::ios_base::out);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{name};
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount > 0)
				m_OutputStream << ",";
			m_ProfileCount++;

			std::string name{result.Name};
			std::ranges::replace(name, '"', '\'');

			m_OutputStream << "{"
				<< R"("cat": "function", )"
				<< R"("dur": )" << result.End - result.Start << ", "
				<< R"("name": ")" << name << R"(", )"
				<< R"("ph": "X", )"
				<< R"("pid": 0, )"
				<< R"("tid": )" << result.ThreadID << ", "
				<< R"("ts": )" << result.Start
				<< "}";

			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			m_OutputStream << R"({"otherData": {},"traceEvents":[)";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	};

	class InstrumentationTimer
	{
	public:
		explicit InstrumentationTimer(const char* name)
			: m_Name(name), m_StartTimepoint{std::chrono::steady_clock::now()}, m_Stopped(false) {}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			const auto endTimepoint = std::chrono::steady_clock::now();

			const long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			const long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			const uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({m_Name, start, end, threadID});

			m_Stopped = true;
		}

	private:
		const char* m_Name;
		const std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#define HZ_PROFILE

#ifdef HZ_PROFILE
// TODO: Find a way to control full session lifetime at compile time.
/*
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath) #ifdef HZ_PROFILE_SESSION_BEGUN \
													 #error "Previous profile session has not been closed yet." \
													 #endif \
													 #define HZ_PROFILE_SESSION_BEGUN \
													 ::Hazel::Instrumentor::Get().BeginSession(name, filepath)
	#define HZ_PROFILE_END_SESSION() #ifndef HZ_PROFILE_SESSION_BEGUN \
									 #error "No profile session to end." \
									 #endif \
									 #undef HZ_PROFILE_SESSION_BEGUN \
									 ::Hazel::Instrumentor::Get().EndSession()
*/

#define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::Hazel::Instrumentor::Get().BeginSession(name, filepath)
#define HZ_PROFILE_END_SESSION() ::Hazel::Instrumentor::Get().EndSession()
#define HZ_PROFILE_SCOPE(name) ::Hazel::InstrumentationTimer timer##__LINE__(name)
#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
	#define HZ_PROFILE_END_SESSION()
	#define HZ_PROFILE_SCOPE(name)
	#define HZ_PROFILE_FUNCTION()
#endif
