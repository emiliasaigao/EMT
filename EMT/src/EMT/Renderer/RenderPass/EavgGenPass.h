#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class EavgGenPass : public RenderPass {
	public:
		EavgGenPass(const Ref<Scene>& scene);
		~EavgGenPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() {}
	};
}