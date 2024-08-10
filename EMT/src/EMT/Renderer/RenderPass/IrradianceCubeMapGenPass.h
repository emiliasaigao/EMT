#pragma once
#include "RenderPass.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/EMTEnum.h"

namespace EMT {
	class IrradianceCubeMapGenPass : public RenderPass {
	public:
		IrradianceCubeMapGenPass(const Ref<Scene>& scene);
		~IrradianceCubeMapGenPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() {}
	};
}