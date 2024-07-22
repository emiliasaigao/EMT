#pragma once
#include <memory>

#ifdef EMT_PLATFORM_WINDOWS
	#ifdef EMT_DYNAMIC_LINK
		#ifdef EMT_BUILD_DLL
		#define EMT_API __declspec(dllexport)
		#else
		#define EMT_API __declspec(dllimport)
		#endif // EMT_BUILD_DLL
	#else
		#define EMT_API
	#endif // EMT_DYNAMIC_LINK
#else error EMT ENGINE ONLY SUPPORT WINDOWS!
#endif // EMT_ENGINE_WINDOWS

#ifdef EMT_ENABLE_ASSERTS
	#define EMT_CORE_ASSERT(x, ...) { if (!(x)) { EMT_CORE_ERROR("∂œ—‘ ß∞‹: {0}",__VA_ARGS__); __debugbreak(); } } 
	#define	EMT_ASSERT(x, ...) {if (!(x)) { EMT_ERROR("∂œ—‘ ß∞‹: {0}",__VA_ARGS__); __debugbreak();} } 
#else
	#define EMT_CORE_ASSERT(x, ...)
	#define	EMT_ASSERT(x, ...)
#endif // EMT_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define BIND_EVENT_FCN(x) std::bind(&x, this, std::placeholders::_1)

namespace EMT {
	template <typename T>
	using Ref = std::shared_ptr<T>;
	
	template <typename T>
	using Scope = std::unique_ptr<T>;
}
