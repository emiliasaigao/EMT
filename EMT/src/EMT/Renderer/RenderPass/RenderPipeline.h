#pragma once
#include "ShadowMapPass.h"
#include "ForwardLightingPass.h"
#include "PassThroughPass.h"

namespace EMT {
	class RenderPipeLine {
	public:
		RenderPipeLine(const Ref<Scene>& scene, int windowWidth, int windowHeight);
		~RenderPipeLine() {}
		void Draw();
		
		void OnWindowResize(int windowWidth, int windowHeight);
		Ref<FrameBuffer> GetLastRenderRes() { return RenderPass::s_Context.passThroughOutput.fbo; }
		RenderPassContext GetPassContext() { return RenderPass::s_Context; }
	private:
		Ref<Scene> m_Scene;
		std::vector<Ref<RenderPass>> m_RenderPasses;
	};
}