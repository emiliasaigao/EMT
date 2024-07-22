#include "emtpch.h"
#include "PointLight.h"

namespace EMT {
	PointLight::PointLight(float intensity, const glm::vec3& lightColor, float attenuationRadius, const glm::vec3& lightPos)
		: BaseLight(intensity,lightColor), mAttenuationRadius(attenuationRadius), mLightPos(lightPos) {
	}


	void PointLight::SetupUniforms(const Ref<Shader>& shader, int currentLightIndex) {
		shader->setVec3f(("pointLights[" + std::to_string(currentLightIndex) + "].position").c_str(), mLightPos);
		shader->setFloat(("pointLights[" + std::to_string(currentLightIndex) + "].intensity").c_str(), mIntensity);
		shader->setVec3f(("pointLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), mLightColor);
		shader->setFloat(("pointLights[" + std::to_string(currentLightIndex) + "].attenuationRadius").c_str(), mAttenuationRadius);
	}

	void PointLight::OnImGuiRender() {
		BaseLight::OnImGuiRender();
		ImGui::DragFloat("AttenuationRadius", &mAttenuationRadius, DRAG_SPEED, 1, 10);
		ImGui::DragFloat3("WorldPos", &mLightPos[0], DRAG_SPEED);
	}
}
