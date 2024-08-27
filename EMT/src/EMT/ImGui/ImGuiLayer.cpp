#include "emtpch.h"
#include "ImGuiLayer.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include <GLFW/glfw3.h>
#include "EMT/Application.h"

namespace EMT {
	ImGuiLayer::ImGuiLayer(const Ref<Scene>& scene, const Ref<RenderPipeLine>& pipeLine)
	: Layer("ImGuiLayer"),m_Scene(scene), m_PipeLine(pipeLine) {
		esgstl::vector<std::string> skyboxFilePaths;
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/right.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/left.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/top.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/bottom.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/back.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/front.png");
		m_SkyBoxes.push_back(std::make_shared<Skybox>(skyboxFilePaths));
		skyboxFilePaths = {};
		skyboxFilePaths.push_back("../EMT/assets/skybox/sea/right.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/sea/left.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/sea/top.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/sea/bottom.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/sea/back.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/sea/front.png");
		m_SkyBoxes.push_back(std::make_shared<Skybox>(skyboxFilePaths));
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// 获取我们自己的窗口
		Application& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetNativeWindow());

		// 将悬停IMGUI绑定给自己的窗口
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::Begin("Scene");
		m_PipeLine->GetLastRenderRes()->GetColorTexture()->DisplayTexture();
		ImGui::End();

		ImGui::Begin("Effect Checkbox");
		ImGui::Checkbox("Enable SSAO", &m_PipeLine->GetPassContext().useSSAO);
		ImGui::Checkbox("Enable SSR", &m_PipeLine->GetPassContext().useSSR);
		ImGui::DragFloat("SSAO Effect", &m_PipeLine->GetPassContext().SSAOEffect, DRAG_SPEED, 0.0, 1.0);
		ImGui::DragFloat("SSAO Radius", &m_PipeLine->GetPassContext().SSAORadius, DRAG_SPEED, 0.0, 10.0);
		ImGui::DragInt("SSAO KernelSize", &m_PipeLine->GetPassContext().SSAOKernelSize, DRAG_SPEED, 0, 64);
		ImGui::DragFloat("SSR Thickness", &m_PipeLine->GetPassContext().ssrThickness, DRAG_SPEED, 0.0, 5.0);
		
		if (ImGui::RadioButton("city_night", mCurSkyBoxIdx == 0)) {
			mCurSkyBoxIdx = 0;
		}
		if (ImGui::RadioButton("sea", mCurSkyBoxIdx == 1)) {
			mCurSkyBoxIdx = 1;
		}
		if (mCurSkyBoxIdx != mLastSkyBoxIdx) {
			m_Scene->GetSkybox() = m_SkyBoxes[mCurSkyBoxIdx];
			m_PipeLine->PreDraw();
			mLastSkyBoxIdx = mCurSkyBoxIdx;
		}

		ImGui::End();

		ImGui::Begin("IrradianceCubeMap");
		m_PipeLine->GetPassContext().irradianceMapOutput.irradianceCubemap->DisplayTexture();
		ImGui::End();

		ImGui::Begin("LpreFilterCubeMap");
		m_PipeLine->GetPassContext().lPrefilterOutput.lPreFilterCubemap->DisplayTexture();
		ImGui::End();

		ImGui::Begin("LUT");
		m_PipeLine->GetPassContext().lutOutput.fbo->GetColorTexture()->DisplayTexture();
		ImGui::End();

		ImGui::Begin("Eavg");
		m_PipeLine->GetPassContext().eavgOutput.fbo->GetColorTexture()->DisplayTexture();
		ImGui::End();


		ImGui::Begin("LigthPass");
		m_PipeLine->GetPassContext().lightOutput.fbo->GetColorTexture()->DisplayTexture();
		ImGui::End();

		ImGui::Begin("Scene_Hierarchy");
		//all kinds of light
		auto lightManager = m_Scene->GetLightManager();
		lightManager->OnImGuiRender();

		//model
		if (ImGui::TreeNode("Models"))
		{
			auto models = m_Scene->GetModels();
			for (auto& model : models)
			{
				if (ImGui::TreeNode(model->GetName().c_str())) {
					model->OnImGuiRender();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}


	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetInstance();
		io.DisplaySize = ImVec2((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 更新和渲染其他平台窗口（暂时还不太懂这段）
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	

	

}