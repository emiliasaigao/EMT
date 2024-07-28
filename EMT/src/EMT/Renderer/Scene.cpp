#include "emtpch.h"
#include "Scene.h"

namespace EMT {
	Scene::Scene(int sceneID) :
		mCamera(new Camera()),
		mLightManager(new LightManager()) {

		Init(sceneID);
	}

	void Scene::Init(int sceneID) {
		
		Ref<Model> cube1 = std::make_shared<Model>(Cube());
		mModels.push_back(cube1);
		cube1->SetPosition(glm::vec3(7.0, 3.0f, -5.0f));
		cube1->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));

		TextureSettings srgbTextureSettings;
		srgbTextureSettings.IsSRGB = true;
		cube1->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("assets/texture/wood.png", &srgbTextureSettings));


		Ref<Model> cube2 = std::make_shared<Model>(Cube());
		cube2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		cube2->SetScale(glm::vec3(30.0f, 1.0f, 30.0f));
		mModels.push_back(cube2);
		cube2->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("assets/texture/white.png", &srgbTextureSettings));


		Ref<Model> cube3 = std::make_shared<Model>(Cube());
		cube3->SetPosition(glm::vec3(10.0f, 3.0f, 3.0f));
		cube3->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
		mModels.push_back(cube3);
		cube3->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("assets/texture/white.png", &srgbTextureSettings));

		Ref<Model> sphere1 = std::make_shared<Model>(Sphere());
		mModels.push_back(sphere1);
		sphere1->SetPosition(glm::vec3(-10.0, 3.0f, 0.0f));
		sphere1->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("assets/texture/wood.png", &srgbTextureSettings));

#pragma region gunSetting
		//Ref<Model> gun = std::make_shared<Model>("res/cerberus/meshes/cerberus.obj");
		//mModels.push_back(gun);
		//gun->SetPosition(glm::vec3(2.0f, 0.5f, 0.0f));
		//gun->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
		//gun->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("res/cerberus/materials/cerberus_albedo.png", &srgbTextureSettings));
		//gun->GetMesh(0)->GetMaterial()->SetNormalMap(TextureLoader::Load2DTexture("res/cerberus/materials/cerberus_normal.png"));
		//gun->GetMesh(0)->GetMaterial()->SetRoughnessMap(TextureLoader::Load2DTexture("res/cerberus/materials/cerberus_rough.png"));
		//gun->GetMesh(0)->GetMaterial()->SetMetallicMap(TextureLoader::Load2DTexture("res/cerberus/materials/cerberus_metal.png"));
		//gun->GetMesh(0)->GetMaterial()->SetAmbientOcclusionMap(TextureLoader::Load2DTexture("res/cerberus/materials/cerberus_ao.png"));
#pragma endregion
		//skeleton animation
	/*	Ref<Model> vampir = std::make_shared<Model>("res/vampire/dancing_vampire.dae", true);
		vampir->LoadAnimation("res/vampire/dancing_vampire.dae");
		vampir->SetScale(glm::vec3(0.05f));
		mModels.push_back(vampir);*/

		//Ref<Model> sponza = std::make_shared<Model>("res/sponza/sponza.obj");
		//Ref<Model> gameboy = std::make_shared<Model>("res/gameboy/scene.gltf");
		//Ref<Model> vikingsword = std::make_shared<Model>("res/vikingsword/scene.gltf");
		//Ref<Model> chess = std::make_shared<Model>("res/Chess/scene.gltf");
		//mModels.push_back(gameboy);
		//mModels.push_back(chess);
		//mModels.push_back(vikingsword);
		//mModels.push_back(sponza);
		//chess->SetPosition(glm::vec3(5.0, 5.0f, 5.0f));
		//chess->SetScale(glm::vec3(0.035f, 0.035f, 0.035f));
		//vikingsword->SetPosition(glm::vec3(5.0, 5.0f, 0.0f));
		//vikingsword->SetScale(glm::vec3(0.035f, 0.035f, 0.035f));
		//gameboy->SetPosition(glm::vec3(0.0, 5.0f, 0.0f));
		//gameboy->SetScale(glm::vec3(0.015f, 0.015f, 0.015f));

		//sponza->SetPosition(glm::vec3(0.02f, 0.02f, -50.02f));
		//sponza->SetScale(glm::vec3(0.015f, 0.015f, 0.015f));




		//TODO: skybox
		std::vector<std::string> skyboxFilePaths;
		skyboxFilePaths.push_back("assets/skybox/right.png");
		skyboxFilePaths.push_back("assets/skybox/left.png");
		skyboxFilePaths.push_back("assets/skybox/top.png");
		skyboxFilePaths.push_back("assets/skybox/bottom.png");
		skyboxFilePaths.push_back("assets/skybox/back.png");
		skyboxFilePaths.push_back("assets/skybox/front.png");
		mSkybox = std::make_shared<Skybox>(skyboxFilePaths);
	}

}
