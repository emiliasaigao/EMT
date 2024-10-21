#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"
#include "EMT/Renderer/Renderer.h"

namespace EMT {
	class SSAOGenPass : public RenderPass {
	public:
		SSAOGenPass(const Ref<Scene>& scene);
		~SSAOGenPass() {}

		virtual void Draw() override;
		virtual void OnWindowResize() override;
	private:
		esgstl::vector<glm::vec3> ssaoKernel;
		esgstl::vector<glm::vec3> ssaoNoise;
		Ref<Texture> noiseTexture;
	};
}