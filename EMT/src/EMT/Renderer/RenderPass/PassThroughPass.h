#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class PassThroughPass : public RenderPass {
	public:
		PassThroughPass(const Ref<Scene>& scene);
		~PassThroughPass() {}

		virtual void Draw() override;
	};
}
