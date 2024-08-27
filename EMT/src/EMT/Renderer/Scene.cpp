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
		cube1->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/texture/wood.png", &srgbTextureSettings));


		Ref<Model> cube2 = std::make_shared<Model>(Cube());
		cube2->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		cube2->SetScale(glm::vec3(30.0f, 0.5f, 30.0f));
		mModels.push_back(cube2);
		cube2->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_Color.png", &srgbTextureSettings));
		cube2->GetMesh(0)->GetMaterial()->SetRoughnessMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_Roughness.png"));
		cube2->GetMesh(0)->GetMaterial()->SetNormalMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_NormalGL.png"));


		Ref<Model> sphere1 = std::make_shared<Model>(Sphere());
		mModels.push_back(sphere1);
		sphere1->SetPosition(glm::vec3(-10.0, 3.0f, 0.0f));
		sphere1->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/texture/wood.png", &srgbTextureSettings));

		Ref<Model> seele = std::make_shared<Model>("../EMT/assets/model/seele/seele.fbx");
		seele->SetScale(glm::vec3(3.f, 3.f, 3.f));
		seele->SetPosition(glm::vec3(0.f, 2.f, 0.f));
		seele->SetRotateAxis(glm::vec3(1.f, 0.f, 0.f));
		seele->SetRotation(-90.f);
		for (int i = 0; i < seele->GetMeshes()->size(); ++i) {
			seele->GetMesh(i)->GetMaterial()->SetRoughnessMap(TextureLoader::Load2DTexture("../EMT/assets/texture/white.png"));
			seele->GetMesh(i)->GetMaterial()->SetNormalMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_NormalGL.png"));
		}
		mModels.push_back(seele);

		Ref<Model> gun = std::make_shared<Model>("../EMT/assets/model/Cerberus/Cerberus_LP.FBX");
		gun->GetMesh()->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_A.tga", &srgbTextureSettings));
		gun->GetMesh()->GetMaterial()->SetMetallicMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_M.tga"));
		gun->GetMesh()->GetMaterial()->SetNormalMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_N.tga"));
		gun->GetMesh()->GetMaterial()->SetRoughnessMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_R.tga"));
		gun->GetMesh()->GetMaterial()->SetAmbientOcclusionMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Raw/Cerberus_AO.tga"));
		gun->SetScale(glm::vec3(0.05f, 0.05f, 0.05f));
		gun->SetPosition(glm::vec3(0.f, 2.f, 0.f));
		gun->SetRotateAxis(glm::vec3(1.f, 0.f, 0.f));
		gun->SetRotation(-90.f);
		mModels.push_back(gun);


		//TODO: skybox
		esgstl::vector<std::string> skyboxFilePaths;
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/right.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/left.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/top.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/bottom.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/back.png");
		skyboxFilePaths.push_back("../EMT/assets/skybox/night_city/front.png");
		mSkybox = std::make_shared<Skybox>(skyboxFilePaths);
	}

}
