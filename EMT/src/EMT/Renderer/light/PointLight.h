#pragma once
#include "BaseLight.h"

namespace EMT {
	class PointLight : public BaseLight {
	public:
		PointLight(float intensity, const glm::vec3& lightColor, float attenuationRadius, const glm::vec3& lightPos);
		virtual ~PointLight() {}

		virtual void SetupUniforms(const Ref<Shader>& shader, int currentLightIndex) override;
		void OnImGuiRender()override;
	private:
		float mAttenuationRadius;
		glm::vec3 mLightPos;
	};
}