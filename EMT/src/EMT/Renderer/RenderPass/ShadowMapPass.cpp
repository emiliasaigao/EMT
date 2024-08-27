#include "emtpch.h"
#include "ShadowMapPass.h"
#include <glm/gtx/norm.hpp>

namespace EMT {
	ShadowMapPass::ShadowMapPass(const Ref<Scene>& scene)
		:RenderPass(scene) {
		m_Shader = Shader::Create("../EMT/assets/shader/shadow/CSMGen.vert", "../EMT/assets/shader/shadow/CSMGen.frag", "../EMT/assets/shader/shadow/CSMGen.geo");
		RenderPass::s_Context.shadowOutput.fbo = FrameBuffer::Create(SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);

		TextureSettings depthStencilTextureSettings;
		depthStencilTextureSettings.TextureFormat = EMT_DEPTH_COMPONENT32F; 
		depthStencilTextureSettings.TextureWrapSMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureWrapTMode = EMT_CLAMP_TO_BORDER;
		depthStencilTextureSettings.TextureMagnificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.TextureMinificationFilterMode = EMT_NEAREST;
		depthStencilTextureSettings.HasBorder = true;	
		depthStencilTextureSettings.HasMips = false;

		RenderPass::s_Context.shadowOutput.fbo->AddDepthStencilTextureArray(depthStencilTextureSettings, EMT_DEPTH_COMPONENT, EMT_FLOAT, EMT_DEPTH_ATTACHMENT);
		RenderPass::s_Context.shadowOutput.fbo->SetUpFrameBuffer();
	}

	void ShadowMapPass::Draw() {
		// bind shadowmap framebuffer
		auto mfbo = RenderPass::s_Context.shadowOutput.fbo;
		mfbo->Bind();
		mfbo->Clear();

		RenderCommand::SetViewport(0, 0, mfbo->GetWidth(), mfbo->GetHeight());

		// setup
		Ref<Camera> camera = m_Scene->GetCamera();
		Ref<LightManager> lightManager = m_Scene->GetLightManager();
		glm::vec3 directionalLightDir = glm::normalize(lightManager->GetDirectionalLightDirection(0));

		float lambda = 0.8;
		esgstl::vector<std::pair<float, float>> frustum(EMT::CSCADED_SIZE);
		float n = 0.1f;
		float f = 100.f;
		frustum[0].first = n;
		for (int i = 1; i < EMT::CSCADED_SIZE; ++i) {
			frustum[i - 1].second = frustum[i].first = lambda * n * std::pow(f / n, double(i) / double(EMT::CSCADED_SIZE))
				+ (1.0f - lambda) * (n + (float(i) / EMT::CSCADED_SIZE) * (f - n));
		}
		frustum[CSCADED_SIZE-1].second = f;

		m_Shader->Bind();
		auto LightPVmatrix = getLightPVMatrix(frustum, directionalLightDir);
		for (int i = 0; i < 4; ++i) {
			m_Shader->setMat4f(std::string("uLightVPMatrix[") + std::to_string(i).data() + std::string("]"), LightPVmatrix[i]);
		}
		RenderCommand::EnableDepthTest();
		RenderCommand::ChangeDepthFunc(EMT::RendererAPI::CompareFunc::Less);
		Renderer::Render(m_Scene, m_Shader, false);

		RenderPass::s_Context.shadowOutput.lightSpaceMatrices = LightPVmatrix;
		RenderPass::s_Context.shadowOutput.frustum = frustum;

	}

	esgstl::vector<glm::mat4> ShadowMapPass::getLightPVMatrix(esgstl::vector<std::pair<float, float>>& frustum, const glm::vec3& lightDir) {
		esgstl::vector<glm::mat4> LightVPMatrix;

		// ndc标准设备空间的八个顶点
		esgstl::vector<glm::vec3> ndcPos = { glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, -1.0f, -1.0f) ,
									glm::vec3(-1.0f, 1.0f, -1.0f) ,glm::vec3(1.0f, 1.0f, -1.0f) ,
									glm::vec3(-1.0f, -1.0f, 1.0f) ,glm::vec3(1.0f, -1.0f, 1.0f) ,
									glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec3(1.0f, 1.0f, 1.0f) };

		// 找到这八个顶点的世界坐标，需要乘上view和Projection的逆
		glm::mat4 cameraView = m_Scene->GetCamera()->getViewMatrix();
		for (int i = 0; i < EMT::CSCADED_SIZE; ++i) {
			glm::mat4 cameraProjection = glm::perspective(glm::radians(m_Scene->GetCamera()->Zoom),
															float(RenderPass::s_Context.windowWidth) / float(RenderPass::s_Context.windowHeight),
															frustum[i].first, frustum[i].second);
			glm::mat4 inv_PV = glm::inverse(cameraProjection * cameraView);
			esgstl::vector<glm::vec4> corners;

			for (int j = 0; j < 8; ++j) {
				glm::vec4 corner = inv_PV * glm::vec4(ndcPos[i], 1.f);
				corners.push_back(corner / corner.w);
			}
			// 根据8个顶点找到一个最大的光源空间的AABB
			float near_len = glm::length2(corners[3] - corners[0]);

			float far_len = glm::length2(corners[7] - corners[4]);

			float len = frustum[i].second - frustum[i].first;

			float x = (len / 2.0f) + ((far_len - near_len) / (8 * len));

			float radius = std::sqrt(x * x + near_len * 0.25f);

			glm::vec3 frustumCenter = glm::vec3(glm::inverse(cameraView) * glm::vec4(0.0, 0.0, -(frustum[i].first + x), 1.0));
			const auto lightView = glm::lookAt(
				frustumCenter + lightDir,
				frustumCenter,
				glm::vec3(0.0f, 1.0f, 0.0f)
			);

			const glm::mat4 lightProjection = glm::ortho(-2.f * radius, 2.f * radius, -2.f * radius, 2.f * radius, -10.f * radius, 10.f * radius);
			LightVPMatrix.push_back(lightProjection * lightView);
		}

		return LightVPMatrix;
	}
}
