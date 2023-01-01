#pragma once

// TODO: Fix or stop using VS auto formatting on save.
#ifdef HZ_PLATFORM_WINDOWS
#ifdef HZ_DYNAMIC_LINK
#ifdef HZ_BUILD_DLL
#define HAZEL_API __declspec(dllexport)
#else
#define HAZEL_API __declspec(dllimport)
#endif // HZ_BUILD_DLL
#else
#define HAZEL_API
#endif // HZ_DYNAMIC_LINK
#else
#error Hazel only supports Windows!
#endif // HZ_PLATFORM_WINDOWS

#ifdef HZ_DEBUG
#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
#define HZ_CORE_ASSERT(x, ...) { if (!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define HZ_CLIENT_ASSERT(x, ...) { if (!(x)) { HZ_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define HZ_CORE_ASSERT(x, ...)
#define HZ_CLIENT_ASSERT(x, ...)
#endif // HZ_ENABLE_ASSERTS

static constexpr int BIT(int x) { return 1 << x; }

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
