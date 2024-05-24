#include "emtpch.h"
#include "Application.h"
#include "EMT/Log.h"

#include "KeyCode.h"
#include "Input.h"

namespace EMT {

	Application* Application::m_Instance = nullptr;

	Application::Application() 
	{
		EMT_CORE_ASSERT(!m_Instance, "已经存在App实例！");
		m_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FCN(Application::OnEvent));

	}

	Application::~Application() {}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.DisPacth<WindowCloseEvent>(BIND_EVENT_FCN(Application::OnWindowClose));
		dispatcher.DisPacth<KeyPressedEvent>(BIND_EVENT_FCN(Application::OnKeyPress));
		
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
		if (key == EMT_KEY_ESCAPE) m_Running = false;
		return true;
	}

	void Application::Run() {

		while (m_Running) {
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
			/*if (Input::isKeyPressed(GLFW_KEY_TAB)) {
				EMT_CORE_ERROR("TAB键是被嗯着的！");
			}*/

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverLayer(Layer* overLayer) {
		m_LayerStack.PushOverLayer(overLayer);
		overLayer->OnAttach();
	}

}