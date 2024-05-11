#pragma once
#ifdef EMT_PLATFORM_WINDOWS
	#ifdef EMT_BUILD_DLL
		#define EMT_API __declspec(dllexport)
	#else
		#define EMT_API __declspec(dllimport)
	#endif // EMT_BUILD_DLL

#else error EMT ENGINE ONLY SUPPORT WINDOWS!
#endif // EMT_ENGINE_WINDOWS

#ifdef EMT_ENABLE_ASSERTS
	#define EMT_CORE_ASSERT(x, ...) { if (!(x)) { EMT_ERROR("∂œ—‘ ß∞‹: {0}",__VA_ARGS__); __debugbreak(); } } 
	#define	EMT_ASSERT(x, ...) {if (!(x)) EMT_CORE_ERROR("∂œ—‘ ß∞‹: {0}",__VA_ARGS__); __debugbreak(); } 
#else
	#define EMT_CORE_ASSERT(x, ...)
	#define	EMT_ASSERT(x, ...)
#endif // EMT_ENABLE_ASSERTS


#define BIT(x) (1 << x)
