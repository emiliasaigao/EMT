#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class LutGenPass : public RenderPass {
	public:
		LutGenPass(const Ref<Scene>& scene);
		~LutGenPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() {}
	};
}