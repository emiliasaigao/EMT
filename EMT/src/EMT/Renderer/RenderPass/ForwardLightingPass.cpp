#include "emtpch.h"
#include "ForwardLightingPass.h"

namespace EMT {
	ForwardLightingPass::ForwardLightingPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/BlinnPhong.vert", "../EMT/assets/shader/BlinnPhong.frag");

		RenderPass::s_Context.lightOutput.fbo = FrameBuffer::Create(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);

		// add attachment and check whether success by call CreateFrameBuffer
		TextureSettings colorTextureSettings;
		colorTextureSettings.TextureFormat = EMT_RGBA32F;
		colorTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_EDGE;
		colorTextureSettings.TextureMagnificationFilterMode = EMT_LINEAR;
		colorTextureSettings.TextureMinificationFilterMode = EMT_LINEAR;
		colorTextureSettings.HasMips = false;
		RenderPass::s_Context.lightOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGBA, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);

		TextureSettings depthStencilTextureSettings;
		depthStencilTextureSettings.TextureFormat = EMT_DEPTH24_STENCIL8;
		depthStencilTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.HasBorder = true;	// use default (1,1,1,1) border color
		depthStencilTextureSettings.HasMips = false;
		RenderPass::s_Context.lightOutput.fbo->AddDepthStencilTexture(depthStencilTextureSettings, EMT_DEPTH_STENCIL, EMT_UNSIGNED_INT_24_8, EMT_DEPTH_STENCIL_ATTACHMENT);

		//mLightingFBO->AddDepthStencilRBO(EMT_DEPTH24_STENCIL8, EMT_DEPTH_STENCIL_ATTACHMENT);
		RenderPass::s_Context.lightOutput.fbo->SetUpFrameBuffer();
	}

	void ForwardLightingPass::Draw() {
		auto mfbo = RenderPass::s_Context.lightOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();

		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());
		RenderCommand::EnableMultiSample();

		Ref<Camera> camera = m_Scene->GetCamera();
		Ref<LightManager> lightManager = m_Scene->GetLightManager();
		// get shadowmap pass output and bind shadowmap to texture unit 0 
		glm::mat4 lightSpaceMatrix = RenderPass::s_Context.shadowOutput.lightSpaceMatrix;
		RenderPass::s_Context.shadowOutput.fbo->GetDepthStencilTexture()->Bind(0);


		m_Shader->Bind();
		m_Shader->setInt("shadowMap", 0);
		m_Shader->setMat4f("lightSpaceMatrix", lightSpaceMatrix);

		// set view, projection matrix, note the model matrix is set in modelrenderer
		m_Shader->setMat4f("view", camera->getViewMatrix());
		m_Shader->setVec3f("viewPos", camera->Position);
		m_Shader->setMat4f("projection", camera->getProjectionMatrix());

		// set a directional light
		m_Shader->setVec3f("directionalLightDir", glm::vec3(1.0f, 1.0f, 1.0f));

		RenderCommand::EnableDepthTest();
		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::DepthFunc::Less);
		Renderer::Render(m_Scene, m_Shader, true);
		Renderer::RenderSkybox(m_Scene);
	}

	void ForwardLightingPass::OnWindowResize() {
		RenderPass::s_Context.lightOutput.fbo->ResizeFrameBuffer(RenderPass::s_Context.windowWidth, RenderPass::s_Context.windowHeight);
	}
}
