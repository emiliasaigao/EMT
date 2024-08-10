#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class DeferredGeometryPass : public RenderPass {
	public:
		DeferredGeometryPass(const Ref<Scene>& scene);
		~DeferredGeometryPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}