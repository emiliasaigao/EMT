#pragma once
#ifdef EMT_PLATFORM_WINDOWS
	#ifdef EMT_BUILD_DLL
		#define EMT_API __declspec(dllexport)
	#else
		#define EMT_API __declspec(dllimport)
	#endif // EMT_BUILD_DLL

#else error EMT ENGINE ONLY SUPPORT WINDOWS!
#endif // EMT_ENGINE_WINDOWS
