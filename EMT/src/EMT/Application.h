#pragma once
#include "Core.h"
#include "Window.h"
#include "Event/Event.h"
#include "EMT/Event/ApplicationEvent.h"
#include "EMT/Event/KeyEvent.h"
#include "EMT/LayerStack.h"

namespace EMT {
	class EMT_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLayer);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnKeyPress(KeyPressedEvent& e);
	};

	Application* CreateApplication();

}