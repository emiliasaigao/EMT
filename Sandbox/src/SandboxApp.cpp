#include <EMT.h>
#include "imgui/imgui.h"

class ExampleLayer : public EMT::Layer {
public:
	ExampleLayer(const std::string& name = "ExampleLayer") : Layer(name) {
		m_VAO.reset(EMT::VertexArray::Create());

		float vertices[] = {
			/*-0.5f, -0.5f, 0.f,
			0.5f, -0.5f, 0.f,
			0.f, 0.5f, 0.f*/
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 1
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 2
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // 3
			-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // 4
			 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // 5
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, // 6
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f  // 7   
		};

		std::shared_ptr<EMT::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(EMT::VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout({
			{EMT::ShaderDataType::Float3,"a_Pos"},
			{EMT::ShaderDataType::Float3,"a_Normal"},
			{EMT::ShaderDataType::Float2,"a_TexCoord"}
			});

		m_VAO->AddVertexBuffer(vertexBuffer);

		unsigned int indices[] = { /*0,1,2*/
			// 后面
			0, 1, 2,
			2, 3, 0,
			// 前面
			4, 5, 6,
			6, 7, 4,
			// 左面
			0, 4, 7,
			7, 3, 0,
			// 右面
			1, 5, 6,
			6, 2, 1,
			// 底面
			0, 1, 5,
			5, 4, 0,
			// 顶面
			3, 2, 6,
			6, 7, 3
		};
		std::shared_ptr<EMT::ElementBuffer> elementBuffer;
		elementBuffer.reset(EMT::ElementBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		m_VAO->setElementBuffer(elementBuffer);

		m_Shader.reset(EMT::Shader::Create("E:\\gitpackage\\EMT\\EMT\\src\\EMT\\Renderer\\shader\\vertex.glsl",
			"E:\\gitpackage\\EMT\\EMT\\src\\EMT\\Renderer\\shader\\fragment.glsl"));
		
		m_LastX = 1600.f / 2.f;
		m_LastY = 900.f / 2.f;
	}

	~ExampleLayer(){
	}

	void OnUpdate() override {
		if (EMT::Input::isKeyPressed(EMT_KEY_W)) m_MainCamera.processKeyBoard(EMT::Camera_Movement::FORWARD, m_DeltaTime);
		if (EMT::Input::isKeyPressed(EMT_KEY_S)) m_MainCamera.processKeyBoard(EMT::Camera_Movement::BACK, m_DeltaTime);
		if (EMT::Input::isKeyPressed(EMT_KEY_A)) m_MainCamera.processKeyBoard(EMT::Camera_Movement::LEFT, m_DeltaTime);
		if (EMT::Input::isKeyPressed(EMT_KEY_D)) m_MainCamera.processKeyBoard(EMT::Camera_Movement::RIGHT, m_DeltaTime);
		float curTime = EMT::RenderCommand::GetTime();
		m_DeltaTime = curTime - m_Time;
		m_Time = curTime;

		EMT::RenderCommand::SetClearColor({ 0.1,0.1,0.1,1.0 });
		EMT::RenderCommand::Clear();
		EMT::RenderCommand::EnableDepthTest();

		EMT::Renderer::BeginScene(m_MainCamera);
		EMT::Renderer::Submit(m_Shader, m_VAO);
		EMT::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		
	}

	void OnEvent(EMT::Event& e) override {
		EMT::EventDispatcher dispatcher(e);
		dispatcher.DisPacth<EMT::MouseMovedEvent>(BIND_EVENT_FCN(ExampleLayer::OnMouseMove));
		dispatcher.DisPacth<EMT::MouseScrolledEvent>(BIND_EVENT_FCN(ExampleLayer::OnMouseScroll));
	}

private:
	bool OnMouseMove(EMT::MouseMovedEvent& e) {
		float xpos = e.GetX();
		float ypos = e.GetY();
		if (m_FirstMouse) {
			xpos = m_LastX;
			ypos = m_LastY;
			m_FirstMouse = false;
		}
		float xoffset = xpos - m_LastX;
		// glfwSetCursorPosCallback返回给mouse_callback函数的(x, y) 是鼠标相对于窗口左【上】角的位置
		// 所以需要将(ypos - lastY) 取反
		float yoffset = m_LastY - ypos;
		m_LastX = xpos;
		m_LastY = ypos;
		m_MainCamera.processMouseMovement(xoffset, yoffset);
		return true;
	}

	bool OnMouseScroll(EMT::MouseScrolledEvent& e) {
		m_MainCamera.processMouseScroll(e.GetYOffset());
		return true;
	}
private:
	std::shared_ptr<EMT::VertexArray> m_VAO;
	std::shared_ptr<EMT::Shader> m_Shader;

	float m_Time = 0.f;
	float m_DeltaTime = 0.f;
	bool m_FirstMouse = true;
	// 鼠标初始设定位置
	float m_LastX;
	float m_LastY;

	EMT::Camera m_MainCamera;
};

class SandboxApp : public EMT::Application {
public:
	SandboxApp(){
		PushLayer(new ExampleLayer());
		PushOverLayer(new EMT::ImGuiLayer());
	}
	~SandboxApp(){}
};

EMT::Application* EMT::CreateApplication() {
	return new SandboxApp();
}
