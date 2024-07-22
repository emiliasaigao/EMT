#include "emtpch.h"
#include "DirectionalLight.h"

namespace EMT {
	DirectionalLight::DirectionalLight(float intensity, const glm::vec3& lightColor, const glm::vec3& lightDir)
		: BaseLight(intensity, lightColor), mLightDir(lightDir) {

	}

	void DirectionalLight::SetupUniforms(const Ref<Shader>& shader, int currentLightIndex) {
		shader->setVec3f(("dirLights[" + std::to_string(currentLightIndex) + "].direction").c_str(), mLightDir);
		shader->setFloat(("dirLights[" + std::to_string(currentLightIndex) + "].intensity").c_str(), mIntensity);
		shader->setVec3f(("dirLights[" + std::to_string(currentLightIndex) + "].lightColor").c_str(), mLightColor);
	}

	void DirectionalLight::OnImGuiRender() {
		BaseLight::OnImGuiRender();
		ImGui::DragFloat3("Dir", &mLightDir[0], DRAG_SPEED, -1, 1);
	}

}
