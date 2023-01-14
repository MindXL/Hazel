#pragma once

#include <chrono>

namespace Hazel
{
	class Timestep
	{
	public:
		using FloatSecond = std::chrono::duration<float, std::chrono::seconds::period>;

		explicit Timestep(FloatSecond time = FloatSecond{0.0f})
			: m_Duration{time} {}

		[[nodiscard]] float GetSeconds() const { return m_Duration.count(); }
		[[nodiscard]] float GetMilliseconds() const
		{
			return std::chrono::duration<float, std::chrono::milliseconds::period>{m_Duration}.count();
		}

	private:
		FloatSecond m_Duration;
	};
}
