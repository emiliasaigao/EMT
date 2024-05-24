#pragma once
#include "Core.h"
#include "Window.h"
#include "Event/Event.h"
#include "EMT/Event/ApplicationEvent.h"
#include "EMT/Event/KeyEvent.h"
#include "EMT/LayerStack.h"
#include "EMT/ImGui/ImGuiLayer.h"


namespace EMT {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* overLayer);

		static Application& GetInstance() { return *m_Instance; }
		inline const std::unique_ptr<Window>& GetWindow() const { return m_Window; }

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnKeyPress(KeyPressedEvent& e);

	private:
		static Application* m_Instance;
	};

	Application* CreateApplication();

}