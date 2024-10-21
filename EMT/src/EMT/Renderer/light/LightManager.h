#pragma once
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"

namespace EMT {
	class LightManager {
	public:
		LightManager();
		~LightManager() {}

		void BindCurrentLights(const Ref<Shader>& shader);
		void AddDirectionalLight(const DirectionalLight& directionalLight);
		void AddPointLight(const PointLight& pointLight);
		void AddSpotLight(const SpotLight& spotLight);

		const glm::vec3& GetDirectionalLightDirection(unsigned int index);
		void OnImGuiRender();

	private:
		void InitBasicLights();

	private:
		esgstl::vector<DirectionalLight> mDirectionalLights;
		esgstl::vector<PointLight> mPointLights;
		esgstl::vector<SpotLight> mSpotLights;
	};
}