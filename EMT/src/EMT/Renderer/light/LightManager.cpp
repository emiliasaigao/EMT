#include "emtpch.h"
#include "LightManager.h"

namespace EMT {
	LightManager::LightManager() {
		InitBasicLights();
	}


	void LightManager::BindCurrentLights(const Ref<Shader>& shader) {
		shader->setVec3i("numDirPointSpotLights", glm::ivec3(mDirectionalLights.size(), mPointLights.size(), mSpotLights.size()));

		for (unsigned int i = 0; i < mDirectionalLights.size(); i++)
			mDirectionalLights[i].SetupUniforms(shader, i);

		for (unsigned int i = 0; i < mSpotLights.size(); i++)
			mSpotLights[i].SetupUniforms(shader, i);

		for (unsigned int i = 0; i < mPointLights.size(); i++)
			mPointLights[i].SetupUniforms(shader, i);
	}

	void LightManager::AddDirectionalLight(const DirectionalLight& directionalLight) {
		mDirectionalLights.push_back(directionalLight);
	}

	void LightManager::AddPointLight(const PointLight& pointLight) {
		mPointLights.push_back(pointLight);
	}

	void LightManager::AddSpotLight(const SpotLight& spotLight) {
		mSpotLights.push_back(spotLight);
	}

	const glm::vec3& LightManager::GetDirectionalLightDirection(unsigned int index) {
		return mDirectionalLights[index].GetLightDir();
	}

	void LightManager::OnImGuiRender() {
		if (ImGui::TreeNode("DirLight")) {
			for (int i = 0; i < mDirectionalLights.size(); i++) {
				if (ImGui::TreeNode((std::string("DirLight ") + std::to_string(i).data()).c_str())) {
					mDirectionalLights[i].OnImGuiRender();
					ImGui::TreePop();
				}

			}
			ImGui::TreePop();
		}
		//more------------------------------------------
		if (ImGui::TreeNode("PointLight")) {
			for (int i = 0; i < mPointLights.size(); i++) {
				if (ImGui::TreeNode((std::string("PointLight ") + std::to_string(i).data()).c_str())) {
					mPointLights[i].OnImGuiRender();
					ImGui::TreePop();
				}

			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("SpotLight")) {
			for (int i = 0; i < mSpotLights.size(); i++) {
				if (ImGui::TreeNode((std::string("SpotLight ") + std::to_string(i).data()).c_str())) {
					mSpotLights[i].OnImGuiRender();
					ImGui::TreePop();
				}

			}
			ImGui::TreePop();
		}

	}

	void LightManager::InitBasicLights() {
		// Setup some lights for the scene
		DirectionalLight directionalLight1(3.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.3f, 1.0f, 0.35f));
		AddDirectionalLight(directionalLight1);
		//SpotLight spotlight1(100.0f, glm::vec3(1.0f, 1.0f, 0.0f), 50.0f, glm::vec3(0.0f, 9.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.0f)));
		//AddSpotLight(spotlight1);

		//SpotLight spotlight2(50.0f, glm::vec3(1.0f, 1.0f, 0.0f), 30.0f, glm::vec3(7.0, 7.0f, -50.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::cos(glm::radians(10.5f)), glm::cos(glm::radians(30.0f)));
		//AddSpotLight(spotlight2);

		PointLight pointLight1(10.0f, glm::vec3(1.0f, 0.0f, 1.0f), 30.0f, glm::vec3(5, 5.0f, 0.0f));
		//AddPointLight(pointLight1);
		//PointLight pointLight2(10.0f, glm::vec3(1.0f, 0.0f, 0.0f), 30.0f, glm::vec3(-10.0, 1.0f, -50.0f));
		//AddPointLight(pointLight2);
		//PointLight pointLight3(10.0f, glm::vec3(0.0f, 1.0f, 0.0f), 30.0f, glm::vec3(-5.0, 1.0f, -50.0f));
		//AddPointLight(pointLight3);
		//PointLight pointLight4(10.0f, glm::vec3(0.0f, 0.0f, 1.0f), 30.0f, glm::vec3(-0.0, 1.0f, -50.0f));
		//AddPointLight(pointLight4);
		/*PointLight pointLight5(10.0f, glm::vec3(1.0f, 0.0f, 1.0f), 30.0f, glm::vec3(-10.0, 1.0f, -45.0f));
		AddPointLight(pointLight5);
		PointLight pointLight6(20.0f, glm::vec3(1.0f, 1.0f, 1.0f), 100.0f, glm::vec3(-5.0, 15.0f, -50.0f));
		AddPointLight(pointLight6);
		PointLight pointLight7(10.0f, glm::vec3(1.0f, 0.25f, 0.0f), 30.0f, glm::vec3(120.0f, 77.0f, 100.0f));
		AddPointLight(pointLight7);*/
	}
}