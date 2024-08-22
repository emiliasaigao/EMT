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
	private:
		std::vector<glm::mat4> getLightPVMatrix(std::vector<std::pair<float, float>>& frustum, const glm::vec3& lightDir);
	};
}