#include "emtpch.h"
#include "PassThroughPass.h"

namespace EMT {
	PassThroughPass::PassThroughPass(const Ref<Scene>& scene)
		:RenderPass(scene){
		
		m_Shader = Shader::Create("assets/shader/passThrough.vert", "assets/shader/passThrough.frag");
		RenderPass::m_Context.passThroughOutput.fbo = FrameBuffer::Create(RenderPass::m_Context.windowWidth, RenderPass::m_Context.windowHeight);
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGBA32F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::m_Context.passThroughOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::m_Context.passThroughOutput.fbo->SetUpFrameBuffer();
	}

	void PassThroughPass::Draw() {
		// disable depth and multisample
		
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_MULTISAMPLE);

		//// detect window size change and resize it when necessary
		//if (DetectWindowSizeChange(mfbo->GetWidth(), mfbo->GetHeight()))
		//	mfbo->ResizeFrameBuffer(WindowManager::Instance()->GetWidth(), WindowManager::Instance()->GetHeight());

		//// bind lighting framebuffer
		//mfbo->Bind();
		//mfbo->Clear();
		//glViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		//// setup
		//ModelRenderer* modelRenderer = mScene->GetModelRenderer();
		//Camera* camera = mScene->GetCamera();
		//LightManager* lightManager = mScene->GetLightManager();

		//// bind shader
		//mPassThroughShader->Bind();

		//// set ssr and deferred lighting
		//if (ssrInput.ssrFBO != nullptr)
		//{
		//	ssrInput.ssrFBO->GetColourTexture()->bind(6);
		//	mPassThroughShader->SetUniform("ssrTexture", 6);
		//}
		//colorFBO->GetColourTexture()->bind(7);
		//mPassThroughShader->SetUniform("colorTexture", 7);


		//modelRenderer->NDC_Plane.Draw();
		//mfbo->UnBind();
		//glEnable(GL_DEPTH_TEST);
	}

}
