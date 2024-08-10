#include "emtpch.h"
#include "LutGenPass.h"

namespace EMT {
	EMT::LutGenPass::LutGenPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/pbr/LutGen.vert", "../EMT/assets/shader/pbr/LutGen.frag");

		RenderPass::s_Context.lutOutput.fbo = FrameBuffer::Create(512, 512);

		// add attachment and check whether success by call CreateFrameBuffer
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGB16F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.lutOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGB, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.lutOutput.fbo->SetUpFrameBuffer();

	}
	
	void LutGenPass::Draw() {
		auto mfbo = RenderPass::s_Context.lutOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();
		RenderCommand::SetViewport(0, 0, 512, 512);
		m_Shader->Bind();

		Renderer::RenderNDCPlane();
		mfbo->UnBind();
	}
}
