#pragma once
#include "ShadowMapPass.h"
#include "ForwardLightingPass.h"
#include "PassThroughPass.h"
#include "IrradianceCubeMapGenPass.h"
#include "LightPrefilterPass.h"
#include "LutGenPass.h"
#include "EavgGenPass.h"
#include "DeferredGeometryPass.h"
#include "DeferredLightingPass.h"
#include "SSRPass.h"
#include "SSAOGenPass.h"
#include "SSAOBlurGenPass.h"
#include "FXAAPass.h"
#include "OutlineGenPass.h"
#include "DebugPass.h"
#include "NormalViewablePass.h"

namespace EMT {
	class RenderPipeLine {
	public:
		RenderPipeLine(const Ref<Scene>& scene, int windowWidth, int windowHeight);
		~RenderPipeLine() {}
		void Draw();
		void PreDraw();
		
		void OnWindowResize(int windowWidth, int windowHeight);
		void OnTrySelectModel(int mouseX, int mouseY);
		Ref<FrameBuffer> GetLastRenderRes() { return RenderPass::s_Context.postprocOutput.fbo; }
		RenderPassContext& GetPassContext() { return RenderPass::s_Context; }
	private:

	private:
		Ref<Scene> m_Scene;
		esgstl::vector<Ref<RenderPass>> m_RenderPasses;
		esgstl::vector<Ref<RenderPass>> m_PreRenderPasses;
	};
}