#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class NormalViewablePass : public RenderPass {
	public:
		NormalViewablePass(const Ref<Scene>& scene);
		~NormalViewablePass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	};
}