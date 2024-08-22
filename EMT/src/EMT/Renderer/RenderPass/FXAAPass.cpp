#include "emtpch.h"
#include "FXAAPass.h"

namespace EMT {
	FXAAPass::FXAAPass(const Ref<Scene>& scene)
		: RenderPass(scene){
		m_Shader = Shader::Create("../EMT/assets/shader/postprocess/FXAA.vert", "../EMT/assets/shader/postprocess/FXAA.frag");
		RenderPass::s_Context.postprocOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGBA32F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.postprocOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.postprocOutput.fbo->SetUpFrameBuffer();
	}

	void FXAAPass::Draw() {
		RenderCommand::DisableDepthTest();
		RenderCommand::DisableMultiSample();

		auto mfbo = RenderPass::s_Context.postprocOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();

		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());
		m_Shader->Bind();

		RenderPass::s_Context.passThroughOutput.fbo->GetColorTexture()->Bind(8);

		m_Shader->setInt("inputTexture", 8);
		m_Shader->setVec2f("texelSize", glm::vec2(1.0f / mfbo->GetWidth(), 1.0f / mfbo->GetHeight()));


		Renderer::RenderNDCPlane();
		mfbo->UnBind();
		RenderCommand::EnableDepthTest();
	}

	void FXAAPass::OnWindowResize() {
		RenderPass::s_Context.postprocOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}

}
