#include "emtpch.h"
#include "Application.h"
#include "EMT/Log.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"

namespace EMT {

	Application* Application::m_Instance = nullptr;

	Application::Application() {
		EMT_CORE_ASSERT(!m_Instance, "�Ѿ�����Appʵ����");
		m_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FCN(Application::OnEvent));

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		const float vertices[9] = {
			-0.5f, -0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			0.f, 0.5f, 0.f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));

		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

		const unsigned int indices[3] = { 0,1,2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (void*)indices, GL_STATIC_DRAW);
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
		if (key == GLFW_KEY_ESCAPE) m_Running = false;
		return true;
	}

	void Application::Run() {
		
		while (m_Running) {
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}
			/*if (Input::isKeyPressed(GLFW_KEY_TAB)) {
				EMT_CORE_ERROR("TAB���Ǳ����ŵģ�");
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