#include "emtpch.h"
#include "IrradianceCubeMapGenPass.h"

namespace EMT {
	IrradianceCubeMapGenPass::IrradianceCubeMapGenPass(const Ref<Scene>& scene)
		:RenderPass(scene){
		m_Shader = Shader::Create("../EMT/assets/shader/pbr/IrradianceMapGen.vert", "../EMT/assets/shader/pbr/IrradianceMapGen.frag");
		RenderPass::s_Context.irradianceMapOutput.fbo = FrameBuffer::Create(64, 64);
		
		RenderPass::s_Context.irradianceMapOutput.fbo->AddDepthStencilRBO(EMT_DEPTH_COMPONENT24, EMT_DEPTH_ATTACHMENT);


		//!!!!! 百思不得其解，为什么addColorTexture行，setColorTexture就不行，明明是一模一样的逻辑
		// 原来是setColorTexture最后写了个UnBind()，所以说别理所应当地吧UnBind和Bind成对的用啊喂！
		//RenderPass::s_Context.irradianceMapOutput.fbo->AddColorTexture(colorTextureSettings, EMT_RGB, EMT_FLOAT, EMT_COLOR_ATTACHMENT0);
		RenderPass::s_Context.irradianceMapOutput.fbo->SetUpFrameBuffer();

		CubemapSettings settings;
		settings.TextureFormat = EMT_RGB16F;
		RenderPass::s_Context.irradianceMapOutput.irradianceCubemap = Cubemap::Create(settings);
		for (int i = 0; i < 6; ++i) {
			RenderPass::s_Context.irradianceMapOutput.irradianceCubemap->GenerateCubemapFace(EMT_TEXTURE_CUBE_MAP_POSITIVE_X + i, 64, 64, EMT_RGB, EMT_FLOAT);
		}

	}

	void IrradianceCubeMapGenPass::Draw() {
		auto mfbo = RenderPass::s_Context.irradianceMapOutput.fbo;
		auto mIrrCubmap = RenderPass::s_Context.irradianceMapOutput.irradianceCubemap;
		mfbo->Bind();

		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[] = {
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		m_Shader->Bind();
		m_Shader->setInt("envCubemap", 0);
		m_Shader->setMat4f("projection", captureProjection);
		m_Scene->GetSkybox()->m_Cubemap->Bind(0);

		RenderCommand::SetViewport(0, 0, 64, 64);
		for (unsigned int i = 0; i < 6; ++i) {
			m_Shader->setMat4f("view", captureViews[i]);
			mfbo->SetColorTexture(EMT_COLOR_ATTACHMENT0, EMT_TEXTURE_CUBE_MAP_POSITIVE_X + i, mIrrCubmap->GetCubemapID(), 0);
			mfbo->Clear();
			Renderer::RenderCube();

			// 为了能显示渲染出的立方体贴图，再渲染一遍到各自的六个面的纹理上
			mfbo->SetColorTexture(EMT_COLOR_ATTACHMENT0, EMT_TEXTURE_2D, mIrrCubmap->GetCubemapFaceTexture(i)->GetTextureId(), 0);
			mfbo->Clear();
			Renderer::RenderCube();
			//mIrrCubmap->GetCubemapFaceTexture(i)->CopyDataFormFBO2D(0, 0, 0, 0, 0, mIrrCubmap->GetWidth(), mIrrCubmap->GetHeight());
		}
		mfbo->UnBind();
	}

}
