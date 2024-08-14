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

	// CSM settings
	const int CSCADED_SIZE = 4;
}



// AA Settings
#define MSAA_SAMPLE_AMOUNT 4 // Only used in forward rendering & for water

// Camera Settings
#define CAMERA_SPEED 20.0f
#define CAMERA_ROTATION_SENSITIVITY 0.15f
#define CAMERA_FOV 60.0f
#define NEAR_PLANE 0.3f
#define FAR_PLANE 5000.0f


// Shadowmap Options
#define SHADOWMAP_RESOLUTION_X 2048
#define SHADOWMAP_RESOLUTION_Y 2048
#define SHADOWMAP_NEAR_PLANE 0.1f
#define SHADOWMAP_FAR_PLANE 250.0f

//drag ui speed
#define DRAG_SPEED 0.01

//Bone setting. Never change, the code is coupled hard to change
#define MAX_BONE_INFLUENCE 4
#define MAX_BONES 100

//#define DEBUG
