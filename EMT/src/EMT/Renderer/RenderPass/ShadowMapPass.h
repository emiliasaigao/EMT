#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"
#include "EMT/Renderer/Renderer.h"

namespace EMT {
	class ShadowMapPass : public RenderPass {
	public:
		ShadowMapPass(const Ref<Scene>& scene);
		~ShadowMapPass() {}

		virtual void Draw() override;
	};
}