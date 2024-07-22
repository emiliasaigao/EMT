#pragma once
#include "BaseLight.h"

namespace EMT {
	class DirectionalLight : public BaseLight {
	public:
		DirectionalLight(float intensity, const glm::vec3& lightColor, const glm::vec3& lightDir);
		virtual ~DirectionalLight() {}

		virtual void SetupUniforms(const Ref<Shader>& shader, int currentLightIndex) override;
		void OnImGuiRender()override;

		const glm::vec3& GetLightDir() const { 
			return mLightDir; 
		}

	private:
		glm::vec3 mLightDir;
	};
}