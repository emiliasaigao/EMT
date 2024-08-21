#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"
#include "EMT/Renderer/Renderer.h"

namespace EMT {
	class SSAOBlurGenPass : public RenderPass {
	public:
		SSAOBlurGenPass(const Ref<Scene>& scene);
		~SSAOBlurGenPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}