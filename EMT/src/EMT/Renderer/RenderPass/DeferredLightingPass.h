#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class DeferredLightingPass : public RenderPass {
	public:
		DeferredLightingPass(const Ref<Scene>& scene);
		~DeferredLightingPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}