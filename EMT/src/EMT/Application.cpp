#include "emtpch.h"
#include "Application.h"
#include "EMT/Log.h"

#include <GLFW/glfw3.h>

namespace EMT {
#define BIND_EVENT_FCN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FCN(OnEvent));
	}

	Application::~Application() {}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.DisPacth<WindowCloseEvent>(BIND_EVENT_FCN(OnWindowClose));
		dispatcher.DisPacth<KeyPressedEvent>(BIND_EVENT_FCN(OnKeyPress));

		EMT_CORE_TRACE("{0}", e);
		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.isHandled()) break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnKeyPress(KeyPressedEvent& e) {
		int key = e.GetKeyCode();
		if (key == GLFW_KEY_ESCAPE) m_Running = false;
		return true;
	}

	void Application::Run() {
		
		while (m_Running) {
			glClearColor(1, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverLayer(Layer* overLayer) {
		m_LayerStack.PushOverLayer(overLayer);
	}

}