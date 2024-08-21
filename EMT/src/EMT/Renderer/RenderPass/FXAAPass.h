#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"
#include "EMT/Renderer/Renderer.h"

namespace EMT {
	class FXAAPass : public RenderPass {
	public:
		FXAAPass(const Ref<Scene>& scene);
		~FXAAPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}