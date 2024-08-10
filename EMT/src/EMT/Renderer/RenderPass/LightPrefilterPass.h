#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class LightPreFilterPass : public RenderPass {
	public:
		LightPreFilterPass(const Ref<Scene>& scene);
		~LightPreFilterPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() {}
	};
}