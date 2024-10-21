#pragma once
#include "RenderPass.h"
#include "EMT/EMTEnum.h"
#include "EMT/Renderer/Renderer.h"
#include "EMT/Renderer/ComputeShader.h"


namespace EMT {
	struct LightFrustumInfo {
		esgstl::vector<glm::mat4> lightPVMatrices;
		esgstl::vector<float> frustumSizes;
	};

	class ShadowMapPass : public RenderPass {
	public:
		ShadowMapPass(const Ref<Scene>& scene);
		~ShadowMapPass() {}

		virtual void Draw() override;
	private:
		LightFrustumInfo getLightFrustumInfo(const esgstl::vector<std::pair<float, float>>& frustum, const glm::vec3& lightDir);
	private:
		Ref<ComputeShader> m_SATGenShader;
		Ref<Texture> m_DepthSAT;
	};
}