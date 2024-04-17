#pragma once
#include "Core.h"

namespace EMT {
	class EMT_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
}