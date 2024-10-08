#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"
#include "EMT/Renderer/Renderer.h"

namespace EMT {
	class PassThroughPass : public RenderPass {
	public:
		PassThroughPass(const Ref<Scene>& scene);
		~PassThroughPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}
