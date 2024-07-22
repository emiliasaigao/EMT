#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shader.h"
#include <imgui/imgui.h>

namespace EMT {
	class BaseLight {
	public:
		BaseLight(float intensity, const glm::vec3& lightColor);
		virtual ~BaseLight() {}

		virtual void SetupUniforms(const Ref<Shader>& shader, int currentLightIndex) = 0;
		virtual void OnImGuiRender();
	protected:
		float mIntensity;
		glm::vec3 mLightColor;
	};
}

