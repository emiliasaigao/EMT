#include "emtpch.h"
#include "SpotLight.h"

namespace EMT {
	SpotLight::SpotLight(float intensity, const glm::vec3& lightColor, float attenuationRadius, const glm::vec3& lightPos, const glm::vec3& lightDir, float innerCutOffAngle, float outerCutOffAngle) :
		BaseLight(intensity, lightColor), mAttenuationRadius(attenuationRadius), mLightDir(lightDir),
		mLightPos(lightPos), mInnerCutOff(innerCutOffAngle), mOuterCutOff(outerCutOffAngle)
	{
	}

	void SpotLight::SetupUniforms(const Ref<Shader>& shader, int currentLightIndex)
	{
		shader->setVec3f(("spotLights[" + std::to_string(currentLightIndex) + "].position").c_str(), mLightPos);
		shader->setVec3f(("spotLights[" + std::to_string(currentLightIndex) + "].direction").c_str(), mLightDir);
		shader->setFloat(("spotLights[" + std::to_string(currentLightIndex) + "].intensity").c_str(), mIntensity);
		shader->setVec3f(("spotLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), mLightColor);
		shader->setFloat(("spotLights[" + std::to_string(currentLightIndex) + "].attenuationRadius").c_str(), mAttenuationRadius);
		shader->setFloat(("spotLights[" + std::to_string(currentLightIndex) + "].innerCutOff").c_str(), mInnerCutOff);
		shader->setFloat(("spotLights[" + std::to_string(currentLightIndex) + "].outerCutOff").c_str(), mOuterCutOff);
	}
	void SpotLight::OnImGuiRender() {
		BaseLight::OnImGuiRender();
		ImGui::DragFloat("AttenuationRadius", &mAttenuationRadius, DRAG_SPEED, 1, 10);
		ImGui::DragFloat("InnerCutoff", &mInnerCutOff, DRAG_SPEED, 0.1, 1.0);
		ImGui::DragFloat("OuterCutoff", &mOuterCutOff, DRAG_SPEED, 0.1, mInnerCutOff);
		ImGui::DragFloat3("WorldPos", &mLightPos[0], DRAG_SPEED);
		ImGui::DragFloat3("LightDir", &mLightDir[0], DRAG_SPEED, -1, 1);
	}
}