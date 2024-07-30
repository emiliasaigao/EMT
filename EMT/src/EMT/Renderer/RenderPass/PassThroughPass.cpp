#include "emtpch.h"
#include "PassThroughPass.h"

namespace EMT {
	PassThroughPass::PassThroughPass(const Ref<Scene>& scene)
		:RenderPass(scene){
		
		m_Shader = Shader::Create("assets/shader/passThrough.vert", "assets/shader/passThrough.frag");
		RenderPass::s_Context.passThroughOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGBA32F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.passThroughOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.passThroughOutput.fbo->SetUpFrameBuffer();
	}

	void PassThroughPass::Draw() {
		// disable depth and multisample
		RenderCommand::DisableDepthTest();
		RenderCommand::DisableMultiSample();

		auto mfbo = RenderPass::s_Context.passThroughOutput.fbo;
		// bind lighting framebuffer
		mfbo->Bind();
		mfbo->Clear();
		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		// bind shader
		m_Shader->Bind();

		// set ssr and deferred lighting
		/*if (ssrInput.ssrFBO != nullptr)
		{
			ssrInput.ssrFBO->GetColourTexture()->bind(6);
			mPassThroughShader->SetUniform("ssrTexture", 6);
		}*/
		RenderPass::s_Context.lightOutput.fbo->GetColorTexture()->Bind(7);
		
		m_Shader->setInt("colorTexture", 7);


		Renderer::RenderNDCPlane();
		mfbo->UnBind();
		RenderCommand::EnableDepthTest();
	}

	void PassThroughPass::OnWindowResize() {
		RenderPass::s_Context.passThroughOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}

}
