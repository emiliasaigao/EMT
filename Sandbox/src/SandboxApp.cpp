#include <EMT.h>
#include "imgui/imgui.h"

class ExampleLayer : public EMT::Layer {
public:
	ExampleLayer(const std::string& name = "ExampleLayer") : Layer(name) {}
	~ExampleLayer(){
	}

	void OnUpdate() override {
		//EMT_INFO("ExampleLayer Updating!");
	}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::End();
	}

	void OnEvent(EMT::Event& e) override {
		EMT_ERROR("{0}", e);
	}
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
