#pragma once
#pragma once
#include "BaseLight.h"

namespace EMT {
	class SpotLight : public BaseLight {
	public:
		SpotLight(float intensity, const glm::vec3& lightColor, float attenuationRadius,
			const glm::vec3& lightPos, const glm::vec3& lightDir, float innerCutOffAngle, float outerCutOffAngle);
		virtual ~SpotLight() {}

		virtual void SetupUniforms(const Ref<Shader>& shader, int currentLightIndex) override;
		void OnImGuiRender()override;
	private:
		float mAttenuationRadius;
		float mInnerCutOff, mOuterCutOff;
		glm::vec3 mLightPos, mLightDir;
	};
}