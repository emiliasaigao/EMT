#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class DebugPass : public RenderPass {
	public:
		DebugPass(const Ref<Scene>& scene);
		~DebugPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}