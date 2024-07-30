#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class ForwardLightingPass : public RenderPass {
	public:
		ForwardLightingPass(const Ref<Scene>& scene);
		~ForwardLightingPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}