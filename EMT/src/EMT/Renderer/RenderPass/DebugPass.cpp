#include "emtpch.h"
#include "DebugPass.h"
namespace EMT {
	DebugPass::DebugPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/debug/SATDebug.vert", "../EMT/assets/shader/debug/SATDebug.frag");
		RenderPass::s_Context.debugPassOutput.fbo = FrameBuffer::Create(SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGBA32F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.debugPassOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.debugPassOutput.fbo->SetUpFrameBuffer();
	}

	void DebugPass::Draw() {
		auto& mfbo = RenderPass::s_Context.debugPassOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();

		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		m_Shader->Bind();
		RenderPass::s_Context.shadowOutput.fbo->GetColorTexture()->Bind(1);
		m_Shader->setInt("depthSAT", 1);

		Renderer::RenderNDCPlane();
		mfbo->UnBind();
	}

	void DebugPass::OnWindowResize() { }

}

