#include "Application.h"
#include "EMT/Log.h"
#include "EMT/Event/ApplicationEvent.h"

namespace EMT {
	Application::Application() {}

	Application::~Application(){}

	void Application::Run() {
		WindowResizeEvent e(1920u, 1080u);
		EMT_ERROR(e);
		while (true) {

		}
	}
}