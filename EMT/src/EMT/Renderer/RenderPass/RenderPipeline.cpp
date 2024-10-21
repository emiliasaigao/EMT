#include "emtpch.h"
#include "RenderPipeline.h"

namespace EMT {
	RenderPipeLine::RenderPipeLine(const Ref<Scene>& scene, int windowWidth, int windowHeight) : m_Scene(scene) {
		RenderPass::s_Context.windowWidth = windowWidth;
		RenderPass::s_Context.windowHeight = windowHeight;
		m_PreRenderPasses.push_back(std::make_shared<IrradianceCubeMapGenPass>(scene));
		m_PreRenderPasses.push_back(std::make_shared<LightPreFilterPass>(scene));
		m_PreRenderPasses.push_back(std::make_shared<LutGenPass>(scene));
		m_PreRenderPasses.push_back(std::make_shared<EavgGenPass>(scene));
		PreDraw();
		
		m_RenderPasses.push_back(std::make_shared<ShadowMapPass>(scene));
		//m_RenderPasses.push_back(std::make_shared<DebugPass>(scene));
		//m_RenderPasses.push_back(std::make_shared<ForwardLightingPass>(scene));
		m_RenderPasses.push_back(std::make_shared<DeferredGeometryPass>(scene));
		m_RenderPasses.push_back(std::make_shared<DeferredLightingPass>(scene));
		m_RenderPasses.push_back(std::make_shared<SSAOGenPass>(scene));
		m_RenderPasses.push_back(std::make_shared<SSAOBlurGenPass>(scene));
		m_RenderPasses.push_back(std::make_shared<SSRPass>(scene));
		m_RenderPasses.push_back(std::make_shared<OutlineGenPass>(scene));
		m_RenderPasses.push_back(std::make_shared<PassThroughPass>(scene));
		m_RenderPasses.push_back(std::make_shared<FXAAPass>(scene));
		RenderCommand::EnableCubeMapSeampless();
	}

	void RenderPipeLine::Draw() {
		for (int i = 0; i < m_RenderPasses.size(); ++i) {
			m_RenderPasses[i]->Draw();
		}
	}

	void RenderPipeLine::PreDraw() {
		for (int i = 0; i < m_PreRenderPasses.size(); ++i) {
			m_PreRenderPasses[i]->Draw();
		}
	}

	void RenderPipeLine::OnTrySelectModel(int mouseX, int mouseY) {
		float ndcX = (2.0f * mouseX) / RenderPass::s_Context.windowWidth - 1.0f;
		float ndcY = 1.0f - (2.0f * mouseY) / RenderPass::s_Context.windowHeight;

		glm::vec4 clipCoords = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
		glm::mat4 projectionMatrix = m_Scene->GetCamera()->getProjectionMatrix();
		glm::mat4 viewMatrix = m_Scene->GetCamera()->getViewMatrix();

		glm::mat4 invProjectionMatrix = glm::inverse(projectionMatrix);
		glm::vec4 viewCoords = invProjectionMatrix * clipCoords;
		viewCoords = glm::vec4(viewCoords.x, viewCoords.y, -1.0f, 0.0f);

		glm::mat4 invViewMatrix = glm::inverse(viewMatrix);
		glm::vec4 rayWorld = invViewMatrix * viewCoords;
		glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
		glm::vec3 rayOrigin = m_Scene->GetCamera()->Position;
		
		if (RenderPass::s_Context.outlineGenOutput.model) RenderPass::s_Context.outlineGenOutput.model->IsSelected = false;
		RenderPass::s_Context.outlineGenOutput.model = m_Scene->GetSelectedModel(Ray(rayOrigin, rayDirection));
	}

	void RenderPipeLine::OnWindowResize(int windowWidth, int windowHeight) {
		RenderPass::s_Context.windowWidth = windowWidth;
		RenderPass::s_Context.windowHeight = windowHeight;
		for (int i = 0; i < m_RenderPasses.size(); ++i) {
			m_RenderPasses[i]->OnWindowResize();
		}
	}
}
