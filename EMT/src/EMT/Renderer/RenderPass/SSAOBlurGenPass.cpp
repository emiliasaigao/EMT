#include "emtpch.h"
#include "SSAOBlurGenPass.h"

namespace EMT {
	SSAOBlurGenPass::SSAOBlurGenPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/postprocess/SSAOBlur.vert", "../EMT/assets/shader/postprocess/SSAOBlur.frag");
		RenderPass::s_Context.ssaoBlurOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_R16F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.ssaoBlurOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RED, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.ssaoBlurOutput.fbo->SetUpFrameBuffer();
	}

	void SSAOBlurGenPass::Draw() {
		RenderCommand::DisableDepthTest();
		RenderCommand::DisableMultiSample();

		auto mfbo = RenderPass::s_Context.ssaoBlurOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();
		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		m_Shader->Bind();
		RenderPass::s_Context.ssaoOutput.fbo->GetColorTexture()->Bind(0);
		RenderPass::s_Context.lightOutput.fbo->GetColorTexture()->Bind(1);
		m_Shader->setInt("SSAOTexture", 0);

		Renderer::RenderNDCPlane();
		mfbo->UnBind();
		RenderCommand::EnableDepthTest();
	}

	void SSAOBlurGenPass::OnWindowResize() {

	}

}
