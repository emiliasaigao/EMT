#include "emtpch.h"
#include "BaseLight.h"

namespace EMT {
	BaseLight::BaseLight(float intensity, const glm::vec3& lightColor) :
		mIntensity(intensity), mLightColor(lightColor) {

	}

	void BaseLight::OnImGuiRender() {
		ImGui::DragFloat("Intensity", &mIntensity, DRAG_SPEED, 0.01, 10);
		ImGui::ColorEdit3("BaseColor", &mLightColor[0]);
	}
}
