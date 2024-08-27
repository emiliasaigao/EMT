#pragma once

#include "EMT/Core.h"
#include "Camera.h"
#include "Skybox.h"
#include "light/LightManager.h"
#include "model/Model.h"
#include "model/shapes/Cube.h"
#include "model/shapes/Quad.h"
#include "model/shapes/Sphere.h"

namespace EMT {
	class Scene
	{
	public:
		Scene(int sceneID = 0);
		~Scene() {}

		inline Ref<Camera> GetCamera() { return mCamera; }
		inline Ref<Skybox>& GetSkybox() { return mSkybox; }
		inline Ref<LightManager> GetLightManager() { return mLightManager; }
		inline esgstl::vector<Ref<Model>>& GetModels() { return mModels; }

	public:
		void Init(int sceneID);

	private:
		Ref<Camera> mCamera;
		Ref<LightManager> mLightManager;
		Ref<Skybox> mSkybox;

		esgstl::vector<Ref<Model>> mModels;
	};
}