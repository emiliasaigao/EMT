#include "emtpch.h"
#include "Scene.h"
#include <queue>

namespace EMT {
	Scene::Scene(int sceneID) :
		mCamera(new Camera()),
		mLightManager(new LightManager()) {

		Init(sceneID);
	}

	esgstl::vector<Model*> Scene::GetViewableModels() {
		esgstl::vector<Model*> viewableModels;
		flushFrustumPlanes();
		auto frustum = GetFrustumPlanes();
		auto root = mBVH->root;
		std::queue<Ref<BVHBuildNode<Model>>> queue;
		queue.push(root);
		while (!queue.empty()) {
			auto node = queue.front();
			queue.pop();
			// 如果相交，则进一步探寻子节点
			if (node->bounds.OverlapFrustum(frustum)) {
				// 如果已经是叶子节点，则将叶子节点的model加入结果集中
				if (node->left == nullptr && node->right == nullptr)
					viewableModels.push_back(node->object);
				else {
					// 否则向下探寻子节点的相交关系
					queue.push(node->left);
					queue.push(node->right);
				}
			}
		}
		return viewableModels;
	}

	void Scene::flushFrustumPlanes()
	{
		auto viewProjMatrix = mCamera->getProjectionMatrix() * mCamera->getViewMatrix();
		std::array<Plane, 6>& planes = mFrustumPlanes;

		// 提取六个平面方程 (a, b, c, d) 对应 ax + by + cz + d = 0

		// 右侧平面: 第4行 - 第1行
		planes[0].normal.x = viewProjMatrix[0][3] - viewProjMatrix[0][0];
		planes[0].normal.y = viewProjMatrix[1][3] - viewProjMatrix[1][0];
		planes[0].normal.z = viewProjMatrix[2][3] - viewProjMatrix[2][0];
		planes[0].distance = viewProjMatrix[3][3] - viewProjMatrix[3][0];

		// 左侧平面: 第4行 + 第1行
		planes[1].normal.x = viewProjMatrix[0][3] + viewProjMatrix[0][0];
		planes[1].normal.y = viewProjMatrix[1][3] + viewProjMatrix[1][0];
		planes[1].normal.z = viewProjMatrix[2][3] + viewProjMatrix[2][0];
		planes[1].distance = viewProjMatrix[3][3] + viewProjMatrix[3][0];

		// 顶部平面: 第4行 - 第2行
		planes[2].normal.x = viewProjMatrix[0][3] - viewProjMatrix[0][1];
		planes[2].normal.y = viewProjMatrix[1][3] - viewProjMatrix[1][1];
		planes[2].normal.z = viewProjMatrix[2][3] - viewProjMatrix[2][1];
		planes[2].distance = viewProjMatrix[3][3] - viewProjMatrix[3][1];

		// 底部平面: 第4行 + 第2行
		planes[3].normal.x = viewProjMatrix[0][3] + viewProjMatrix[0][1];
		planes[3].normal.y = viewProjMatrix[1][3] + viewProjMatrix[1][1];
		planes[3].normal.z = viewProjMatrix[2][3] + viewProjMatrix[2][1];
		planes[3].distance = viewProjMatrix[3][3] + viewProjMatrix[3][1];

		// 远平面: 第4行 - 第3行
		planes[4].normal.x = viewProjMatrix[0][3] - viewProjMatrix[0][2];
		planes[4].normal.y = viewProjMatrix[1][3] - viewProjMatrix[1][2];
		planes[4].normal.z = viewProjMatrix[2][3] - viewProjMatrix[2][2];
		planes[4].distance = viewProjMatrix[3][3] - viewProjMatrix[3][2];

		// 近平面: 第4行 + 第3行
		planes[5].normal.x = viewProjMatrix[0][3] + viewProjMatrix[0][2];
		planes[5].normal.y = viewProjMatrix[1][3] + viewProjMatrix[1][2];
		planes[5].normal.z = viewProjMatrix[2][3] + viewProjMatrix[2][2];
		planes[5].distance = viewProjMatrix[3][3] + viewProjMatrix[3][2];
	}

	void Scene::Init(int sceneID) {

		esgstl::vector<Model> mModels;

		/*Ref<Model> cube1 = std::make_shared<Model>(Cube());
		mModels.push_back(cube1);
		cube1->SetPosition(glm::vec3(7.0, 3.0f, -5.0f));
		cube1->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));*/

		TextureSettings srgbTextureSettings;
		srgbTextureSettings.IsSRGB = true;
		//cube1->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/texture/wood.png", &srgbTextureSettings));


		Model cube2 = Model(Cube());
		/*cube2.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		cube2.SetScale(glm::vec3(30.0f, 0.5f, 30.0f));*/
		cube2.GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_Color.png", &srgbTextureSettings));
		//cube2->GetMesh(0)->GetMaterial()->SetRoughnessMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_Roughness.png"));
		cube2.GetMesh(0)->GetMaterial()->SetRoughnessMap(TextureLoader::GetDefaultRoughness());
		cube2.GetMesh(0)->GetMaterial()->SetNormalMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_NormalGL.png"));
		mModels.push_back(std::move(cube2));


		/*Ref<Model> sphere1 = std::make_shared<Model>(Sphere());
		mModels.push_back(sphere1);
		sphere1->SetPosition(glm::vec3(-10.0, 3.0f, 0.0f));
		sphere1->GetMesh(0)->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/texture/wood.png", &srgbTextureSettings));*/

		//Ref<Model> seele = std::make_shared<Model>("../EMT/assets/model/seele/seele.fbx");
		//seele->SetScale(glm::vec3(3.f, 3.f, 3.f));
		//seele->SetPosition(glm::vec3(0.f, 2.f, 0.f));
		//seele->SetRotateAxis(glm::vec3(1.f, 0.f, 0.f));
		//seele->SetRotation(-90.f);
		//for (int i = 0; i < seele->GetMeshes()->size(); ++i) {
		//	seele->GetMesh(i)->GetMaterial()->SetRoughnessMap(TextureLoader::Load2DTexture("../EMT/assets/texture/white.png"));
		//	seele->GetMesh(i)->GetMaterial()->SetNormalMap(TextureLoader::Load2DTexture("../EMT/assets/texture/Wood092_1K-PNG/Wood092_1K-PNG_NormalGL.png"));
		//}
		//mModels.push_back(seele);
		//Model marble_bust = Model("../EMT/assets/model/marble_bust/marble_bust_01_4k.fbx");
		//marble_bust.SetScale(glm::vec3(5.f, 5.f, 5.f));
		//marble_bust.SetPosition(glm::vec3(3.f, 2.f, 2.f));
		//marble_bust.SetRotateAxis(glm::vec3(1.f, 0.f, 0.f));
		//marble_bust.SetRotation(-90.f);

		//mModels.push_back(std::move(marble_bust));

		//Model gun = Model("../EMT/assets/model/Cerberus/Cerberus_LP.FBX");
		//gun.GetMesh()->GetMaterial()->SetAlbedoMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_A.tga", &srgbTextureSettings));
		//gun.GetMesh()->GetMaterial()->SetMetallicMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_M.tga"));
		//gun.GetMesh()->GetMaterial()->SetNormalMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_N.tga"));
		//gun.GetMesh()->GetMaterial()->SetRoughnessMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Cerberus_R.tga"));
		//gun.GetMesh()->GetMaterial()->SetAmbientOcclusionMap(TextureLoader::Load2DTexture("../EMT/assets/model/Cerberus/Textures/Raw/Cerberus_AO.tga"));
		//gun.SetScale(glm::vec3(0.05f, 0.05f, 0.05f));
		//gun.SetPosition(glm::vec3(-2.0f, 4.f, 0.f));
		//gun.SetRotateAxis(glm::vec3(1.f, 0.f, 0.f));
		//gun.SetRotation(-90.f);
		//mModels.push_back(std::move(gun));

		mBVH = std::make_shared<BVHAccel<Model>>(mModels);
		
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
