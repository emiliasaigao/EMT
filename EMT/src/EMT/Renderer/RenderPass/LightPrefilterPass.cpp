#include "emtpch.h"
#include "LightPrefilterPass.h"

namespace EMT {
	LightPreFilterPass::LightPreFilterPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/pbr/LightPreFilter.vert", "../EMT/assets/shader/pbr/LightPreFilter.frag");

		RenderPass::s_Context.lPrefilterOutput.fbo = FrameBuffer::Create(128, 128);
		RenderPass::s_Context.lPrefilterOutput.fbo->AddDepthStencilRBO(EMT_DEPTH_COMPONENT24, EMT_DEPTH_ATTACHMENT);
		RenderPass::s_Context.lPrefilterOutput.fbo->SetUpFrameBuffer();

		CubemapSettings settings;
		settings.TextureFormat = EMT_RGB16F;
		settings.TextureMinificationFilterMode = EMT_LINEAR_MIPMAP_LINEAR;
		settings.HasMips = true;
		RenderPass::s_Context.lPrefilterOutput.lPreFilterCubemap = Cubemap::Create(settings);
		for (int i = 0; i < 6; ++i) {
			RenderPass::s_Context.lPrefilterOutput.lPreFilterCubemap->GenerateCubemapFace(EMT_TEXTURE_CUBE_MAP_POSITIVE_X + i, 128, 128, EMT_RGB, EMT_FLOAT);
		}
	}

	void LightPreFilterPass::Draw() {
		auto mfbo = RenderPass::s_Context.lPrefilterOutput.fbo;
		auto mLpreFCubemap = RenderPass::s_Context.lPrefilterOutput.lPreFilterCubemap;

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

		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			mfbo->ResizeFrameBuffer(mipWidth, mipHeight);
			RenderCommand::SetViewport(0, 0, mipWidth, mipHeight);
			

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			m_Shader->setFloat("uRoughness", roughness);
			for (unsigned int i = 0; i < 6; ++i) {
				m_Shader->setMat4f("view", captureViews[i]);
				mfbo->SetColorTexture(EMT_COLOR_ATTACHMENT0, EMT_TEXTURE_CUBE_MAP_POSITIVE_X + i, mLpreFCubemap->GetCubemapID(), mip);
				mfbo->Clear();
				
				Renderer::RenderCube();

				// 为了能显示渲染出的立方体贴图，再渲染一遍到各自的六个面的纹理上
				mfbo->SetColorTexture(EMT_COLOR_ATTACHMENT0, EMT_TEXTURE_2D, mLpreFCubemap->GetCubemapFaceTexID(i), mip);
				mfbo->Clear();
				Renderer::RenderCube();
			}
		}
	}

}
