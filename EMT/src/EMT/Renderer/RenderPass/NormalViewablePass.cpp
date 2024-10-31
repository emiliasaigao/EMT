#include "emtpch.h"
#include "NormalViewablePass.h"

namespace EMT {
	NormalViewablePass::NormalViewablePass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/debug/NormalViewable.vert", 
			"../EMT/assets/shader/debug/NormalViewable.frag", 
			"../EMT/assets/shader/debug/NormalViewable.geo");

		RenderPass::s_Context.normalViewableOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
		TextureSettings depthStencilTextureSettings;
		depthStencilTextureSettings.TextureFormat = EMT_DEPTH_COMPONENT32F;
		depthStencilTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.HasBorder = true;
		depthStencilTextureSettings.HasMips = false;

		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGBA32F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;

		RenderPass::s_Context.normalViewableOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);

		RenderPass::s_Context.normalViewableOutput.fbo->AddDepthStencilTexture(depthStencilTextureSettings, EMT_DEPTH_COMPONENT, EMT_FLOAT, EMT_DEPTH_ATTACHMENT);
		RenderPass::s_Context.normalViewableOutput.fbo->SetUpFrameBuffer();
	}

	void NormalViewablePass::Draw() {
		auto& mfbo = RenderPass::s_Context.normalViewableOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();

		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		Ref<Camera> camera = m_Scene->GetCamera();
		m_Shader->Bind();
		// set view, projection matrix, note the model matrix is set in modelrenderer
		m_Shader->setMat4f("view", camera->getViewMatrix());
		m_Shader->setMat4f("projection", camera->getProjectionMatrix());

		Renderer::Render(m_Scene, m_Shader, true);
	}

	void NormalViewablePass::OnWindowResize() {
		RenderPass::s_Context.normalViewableOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}

}

