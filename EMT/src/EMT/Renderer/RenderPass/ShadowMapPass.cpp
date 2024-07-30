#include "emtpch.h"
#include "ShadowMapPass.h"


namespace EMT {
	ShadowMapPass::ShadowMapPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("assets/shader/shadowMapGenerate.vert", "assets/shader/shadowMapGenerate.frag");
		RenderPass::s_Context.shadowOutput.fbo = FrameBuffer::Create(SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);
		
		TextureSettings depthStencilTextureSettings;
		depthStencilTextureSettings.TextureFormat = EMT_DEPTH_COMPONENT32; // Only use depth, cause we don't need stencil in shadowmap pass.
		depthStencilTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.HasBorder = true;	// use default (1,1,1) border color
		depthStencilTextureSettings.HasMips = false;

		RenderPass::s_Context.shadowOutput.fbo->AddDepthStencilTexture(depthStencilTextureSettings, EMT_DEPTH_COMPONENT, EMT_FLOAT, EMT_DEPTH_ATTACHMENT);
		RenderPass::s_Context.shadowOutput.fbo->SetUpFrameBuffer();
	}

	void ShadowMapPass::Draw() {
		// bind shadowmap framebuffer
		auto mfbo = RenderPass::s_Context.shadowOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();

		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		// setup
		//ModelRenderer* modelRenderer = mScene->GetModelRenderer();
		Ref<Camera> camera = m_Scene->GetCamera();
		Ref<LightManager> lightManager = m_Scene->GetLightManager();
		glm::vec3 directionalLightDir = lightManager->GetDirectionalLightDirection(0);

		// view setup
		m_Shader->Bind();
		glm::vec3 dirLightShadowmapLookAtPos = camera->Position + (glm::normalize(camera->Front * 15.0f));
		//note the direction of DirLight is inverted, thus here is  + 
		glm::vec3 dirLightShadowmapEyePos = dirLightShadowmapLookAtPos + (glm::normalize(directionalLightDir) * 35.0f);
		glm::mat4 directionalLightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, SHADOWMAP_NEAR_PLANE, SHADOWMAP_FAR_PLANE);
		glm::mat4 directionalLightView = glm::lookAt(dirLightShadowmapEyePos, dirLightShadowmapLookAtPos, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 directionalLightViewProjMatrix = directionalLightProjection * directionalLightView;
		m_Shader->setMat4f("lightSpaceMatrix", directionalLightViewProjMatrix);

		// add models to renderer
		RenderCommand::EnableDepthTest();
		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::DepthFunc::Less);
		Renderer::Render(m_Scene, m_Shader, false);

		RenderPass::s_Context.shadowOutput.lightSpaceMatrix = directionalLightViewProjMatrix;

	}

}
