#include "emtpch.h"
#include "EavgGenPass.h"

namespace EMT {
	EavgGenPass::EavgGenPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/pbr/EavgGen.vert", "../EMT/assets/shader/pbr/EavgGen.frag");

		RenderPass::s_Context.eavgOutput.fbo = FrameBuffer::Create(512, 512);

		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_R16F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.eavgOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RED, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.eavgOutput.fbo->SetUpFrameBuffer();
	}
	
	void EavgGenPass::Draw() {
		auto mfbo = RenderPass::s_Context.eavgOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();
		RenderCommand::SetViewport(0, 0, 512, 512);
		RenderPass::s_Context.lutOutput.fbo->GetColorTexture()->Bind(0);
		m_Shader->Bind();
		m_Shader->setInt("EiMap", 0);
		Renderer::RenderNDCPlane();
		mfbo->UnBind();
	}
}
