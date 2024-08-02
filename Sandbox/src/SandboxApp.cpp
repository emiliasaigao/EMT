#include <EMT.h>
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public EMT::Layer {
public:
	ExampleLayer(const EMT::Ref<EMT::Scene>& scene, const EMT::Ref<EMT::RenderPipeLine>& pipeLine)
		: Layer("ExampleLayer"),m_LastX(800), m_LastY(450), m_Scene(scene), m_PipeLine(pipeLine) {
		
	}

	~ExampleLayer(){
	}

	void OnUpdate() override {
		// 处理相机运动
		if (EMT::Input::isKeyPressed(EMT_KEY_W)) m_Scene->GetCamera()->processKeyBoard(EMT::Camera_Movement::FORWARD, m_DeltaTime);
		if (EMT::Input::isKeyPressed(EMT_KEY_S)) m_Scene->GetCamera()->processKeyBoard(EMT::Camera_Movement::BACK, m_DeltaTime);
		if (EMT::Input::isKeyPressed(EMT_KEY_A)) m_Scene->GetCamera()->processKeyBoard(EMT::Camera_Movement::LEFT, m_DeltaTime);
		if (EMT::Input::isKeyPressed(EMT_KEY_D)) m_Scene->GetCamera()->processKeyBoard(EMT::Camera_Movement::RIGHT, m_DeltaTime);
		
		float curTime = EMT::RenderCommand::GetTime();
		m_DeltaTime = curTime - m_Time;
		m_Time = curTime;
		EMT::RenderCommand::SetClearColor(glm::vec4(0.2));
		EMT::RenderCommand::Clear();
		m_PipeLine->Draw();
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
		m_Scene->GetCamera()->processMouseMovement(xoffset, yoffset);
		return true;
	}

	bool OnMouseScroll(EMT::MouseScrolledEvent& e) {
		m_Scene->GetCamera()->processMouseScroll(e.GetYOffset());
		return true;
	}

private:

	float m_Time = 0.f;
	float m_DeltaTime = 0.f;
	bool m_FirstMouse = true;
	// 鼠标初始设定位置
	float m_LastX;
	float m_LastY;

	EMT::Ref<EMT::Scene> m_Scene;
	EMT::Ref<EMT::RenderPipeLine> m_PipeLine;
};

class SandboxApp : public EMT::Application {
public:
	SandboxApp()
	:m_Scene(new EMT::Scene()){
		m_RenderPipeLine = std::make_shared<EMT::RenderPipeLine>(m_Scene, GetWindow()->GetWidth(), GetWindow()->GetHeight());
		EMT::TextureLoader::InitDefaultTextures();
		PushLayer(new ExampleLayer(m_Scene, m_RenderPipeLine));
		PushOverLayer(new EMT::ImGuiLayer(m_Scene, m_RenderPipeLine));
	}
	~SandboxApp(){}
private:
	EMT::Ref<EMT::Scene> m_Scene;
	EMT::Ref<EMT::RenderPipeLine> m_RenderPipeLine;
};

EMT::Application* EMT::CreateApplication() {
	return new SandboxApp();
}
