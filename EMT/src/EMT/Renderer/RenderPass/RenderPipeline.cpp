#include "emtpch.h"
#include "RenderPipeline.h"

namespace EMT {
	RenderPipeLine::RenderPipeLine(const Ref<Scene>& scene, int windowWidth, int windowHeight) {
		RenderPass::s_Context.windowWidth = windowWidth;
		RenderPass::s_Context.windowHeight = windowHeight;
		m_RenderPasses.push_back(std::make_shared<ShadowMapPass>(scene));
		m_RenderPasses.push_back(std::make_shared<ForwardLightingPass>(scene));
		m_RenderPasses.push_back(std::make_shared<PassThroughPass>(scene));
		RenderCommand::EnableCubeMapSeampless();
	}

	void RenderPipeLine::Draw() {
		for (int i = 0; i < m_RenderPasses.size(); ++i) {
			m_RenderPasses[i]->Draw();
		}
	}
	
	void RenderPipeLine::OnWindowResize(int windowWidth, int windowHeight) {
		RenderPass::s_Context.windowWidth = windowWidth;
		RenderPass::s_Context.windowHeight = windowHeight;
		for (int i = 0; i < m_RenderPasses.size(); ++i) {
			m_RenderPasses[i]->OnWindowResize();
		}
	}
}
