#include <EMT.h>
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public EMT::Layer {
public:
	ExampleLayer(const std::string& name = "ExampleLayer") : Layer(name) {
		m_VAO = EMT::VertexArray::Create();

		float vertices[] = {
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // 0
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // 1
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // 2
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // 3
			// 后面
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 4
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // 5
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // 6
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // 7
			// 左面
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 8
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 9
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 10
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 11
			// 右面
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 12
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 13
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 14
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 15
			 // 上面
			 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // 16
			  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // 17
			  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // 18
			 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, // 19
			 // 下面
			 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // 20
			  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // 21
			  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // 22
			 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f  // 23  
		};

		EMT::Ref<EMT::VertexBuffer> vertexBuffer;
		vertexBuffer = EMT::VertexBuffer::Create(vertices, sizeof(vertices));

		vertexBuffer->SetLayout({
			{EMT::ShaderDataType::Float3,"a_Pos"},
			{EMT::ShaderDataType::Float3,"a_Normal"},
			{EMT::ShaderDataType::Float2,"a_TexCoord"}
			});

		m_VAO->AddVertexBuffer(vertexBuffer);

		unsigned int indices[] = { /*0,1,2*/
			0, 1, 2, 2, 3, 0,  // 前面
			4, 5, 6, 6, 7, 4,  // 后面
			8, 9, 10, 10, 11, 8, // 左面
			12, 13, 14, 14, 15, 12, // 右面
			16, 17, 18, 18, 19, 16, // 上面
			20, 21, 22, 22, 23, 20  // 下面
		};
		EMT::Ref<EMT::ElementBuffer> elementBuffer;
		elementBuffer = EMT::ElementBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));

		m_VAO->SetElementBuffer(elementBuffer);

		m_Shader = EMT::Shader::Create("assets/shader/vertex.glsl", "assets/shader/fragment.glsl");

		std::static_pointer_cast<EMT::OpenGLShader>(m_Shader)->setInt("uCubeTex", 1);
		//m_CubeTex = EMT::Texture2D::Create("assets/texture/container_diff.png");
		//m_CubeTex->Bind(1);
		
		m_LastX = 1600.f / 2.f;
		m_LastY = 900.f / 2.f;
	}

	~ExampleLayer(){
	}

	void OnUpdate() override {
		// 处理相机运动
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
	EMT::Ref<EMT::VertexArray> m_VAO;
	EMT::Ref<EMT::Shader> m_Shader;
	//EMT::Ref<EMT::Texture2D> m_CubeTex;

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
