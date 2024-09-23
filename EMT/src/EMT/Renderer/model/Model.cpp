#include "emtpch.h"
#include "Model.h"

namespace EMT {
	unsigned int Model::noNameNum = 0;

	Model::Model(const std::string& path) {
		LoadModel(path);
	}

	Model::Model(const Mesh& mesh) {
		
		m_BVH = std::make_shared<BVHAccel<Mesh>>(std::initializer_list<Mesh>{mesh});
		mName = "None_";
		mName += std::to_string(noNameNum).c_str();
		noNameNum++;
	}

	Model::Model(const esgstl::vector<Mesh>& meshes) {
		m_BVH = std::make_shared<BVHAccel<Mesh>>(meshes);
		mName = "None_";
		mName += std::to_string(noNameNum).c_str();
		noNameNum++;
	}

	Model::Model(Model&& model)
		:mDirectory(std::move(model.mDirectory)),
		mName(std::move(model.mName)),
		mCenter(std::move(model.mCenter)),
		mPosition(std::move(model.mPosition)),
		mScale(std::move(model.mScale)),
		mRotateAxis(std::move(model.mRotateAxis)),
		m_BVH(std::move(model.m_BVH)){
	}

	Model::Model(const Model& model)
		:mDirectory(model.mDirectory),
		mName(model.mName),
		mCenter(model.mCenter),
		mPosition(model.mPosition),
		mScale(model.mScale),
		mRotation(model.mRotation),
		mRotateAxis(model.mRotateAxis),
		m_BVH(model.m_BVH){
	}

	Model& Model::operator=(Model&& model) {
		Model tmp(std::move(model));
		std::swap(*this, tmp);
		return *this;
	}

	Model& Model::operator=(const Model& model) {
		mDirectory = model.mDirectory;
		mName = model.mName;
		mCenter = model.mCenter;
		mPosition = model.mPosition;
		mScale = model.mScale;
		mRotation = model.mRotation;
		mRotateAxis = model.mRotateAxis;
		m_BVH = model.m_BVH;
		return *this;
	}


	void Model::Draw(const Ref<Shader>& shader, const std::array<Plane, 6>& frustum, bool isUseMaterial) const {
		//auto meshes = GetViewableMeshes(frustum);
		auto meshes = GetMeshes();
		for (unsigned int i = 0; i < meshes.size(); ++i) {

			if (isUseMaterial) {
				meshes[i].m_Material->BindMaterial(shader);

				// check whether to use normal mapping
				if (meshes[i].HasTangents())
					shader->setInt("useNormalMapping", 1);
				else
					shader->setInt("useNormalMapping", 0);
			}
			meshes[i].Draw();
		}
	}

	void Model::LoadModel(const std::string& path) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			EMT_CORE_ERROR("ERROR::ASSIMP:: {0}",import.GetErrorString());
			return;
		}
		
		mDirectory = path.substr(0, path.find_last_of('/'));
		mName = mDirectory.substr(mDirectory.find_last_of('/') + 1, mDirectory.size());

		esgstl::vector<Mesh> meshes;
		ProcessNode(scene->mRootNode, scene, meshes);

		// TODO：构建BVH
		m_BVH = std::make_shared<BVHAccel<Mesh>>(std::move(meshes));
	}


	void Model::ProcessNode(aiNode* node, const aiScene* scene, esgstl::vector<Mesh>& meshes)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			// 获取节点中的网格数据
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh emtMesh = ProcessMesh(mesh, scene);
			meshes.push_back(emtMesh);
		}

		// 递归处理子节点
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, meshes);
		}
		
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// 开辟顶点数据所需空间
		esgstl::vector<glm::vec3> positions;
		esgstl::vector<glm::vec2> uvs;
		esgstl::vector<glm::vec3> normals;
		esgstl::vector<glm::vec3> tangents;
		esgstl::vector<glm::vec3> bitangents;
		esgstl::vector<unsigned int> indices;

		positions.reserve(mesh->mNumVertices);
		uvs.reserve(mesh->mNumVertices);
		normals.reserve(mesh->mNumVertices);
		tangents.reserve(mesh->mNumVertices);
		bitangents.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces * 3);


		// 处理每个顶点
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
			glm::vec2 textureCoord;
			// 如果有uv坐标的话
			if (mesh->mTextureCoords[0]) {
				// 一个顶点可能对应了至多八个uv坐标，这里只用第一个
				textureCoord.x = mesh->mTextureCoords[0][i].x;
				textureCoord.y = mesh->mTextureCoords[0][i].y;
			}
			else {
				textureCoord.x = 0.0f;
				textureCoord.y = 0.0f;
			}

			positions.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
			uvs.push_back(glm::vec2(textureCoord.x, textureCoord.y));
			normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
			tangents.push_back(glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z));
			bitangents.push_back(glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z));
		}

		// 遍历每个面获取索引数组
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j) {
				indices.push_back(face.mIndices[j]);
			}
		}

		Mesh newMesh(positions, uvs, normals, tangents, bitangents, indices);

		newMesh.SetupMesh();

		// 处理纹理
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// This set is only work for my sponza mtl file. be careful!
			newMesh.m_Material->SetAlbedoMap(LoadMaterialTexture(material, aiTextureType_DIFFUSE, true));
			newMesh.m_Material->SetNormalMap(LoadMaterialTexture(material, aiTextureType_NORMALS, false));
			newMesh.m_Material->SetAmbientOcclusionMap(LoadMaterialTexture(material, aiTextureType_AMBIENT, false));
			newMesh.m_Material->SetMetallicMap(LoadMaterialTexture(material, aiTextureType_SPECULAR, false));
			newMesh.m_Material->SetRoughnessMap(LoadMaterialTexture(material, aiTextureType_SHININESS, false));
			newMesh.m_Material->SetMixtureMap(LoadMaterialTexture(material, aiTextureType_UNKNOWN, false));

			if (material->GetTextureCount(aiTextureType_UNKNOWN) > 0)
				newMesh.m_Material->SeperateMixture(); // try to seperate mixture for gltf file only;
		}
		return newMesh;
	}

	Ref<Texture> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, bool isSRGB) {
		// 标准着色器每种类型只有一个纹理，多了的话报警告
		if (mat->GetTextureCount(type) > 2) {
			EMT_CORE_WARN("Mesh's default material contains more than 1 texture for the same type, which currently isn't supported by the standard shader");
		}

		if (mat->GetTextureCount(type) > 0) {
			aiString str;
			mat->GetTexture(type, 0, &str);

			// 我们规定材质与模型对象必须位于同一目录中
			// 纹理str是相对路径，因此与m_Directory连接可以获得全路径
			std::string fileToSearch = (mDirectory + "/" + std::string(str.C_Str())).c_str();

			TextureSettings textureSettings;
			textureSettings.IsSRGB = isSRGB;

			for (auto& c : fileToSearch)
				if (c == '\\') c = '/';
			return TextureLoader::Load2DTexture(fileToSearch, &textureSettings);
		}

		return nullptr;
	}

	void Model::OnImGuiRender() {
		//ImGui::Text(mDirectory.c_str());

		ImGui::Text((std::string("Center: (") + std::to_string(mCenter.x) + ", " + std::to_string(mCenter.y) + ", " + std::to_string(mCenter.z) + " )").c_str());
		ImGui::DragFloat3("Pos", &mPosition[0], DRAG_SPEED);
		ImGui::DragFloat3("Scale", &mScale[0], DRAG_SPEED);
		ImGui::DragFloat("Rotate_Angle", &mRotation, DRAG_SPEED * 10, -180, 180);
		ImGui::DragFloat3("RotateAxis", &mRotateAxis[0], DRAG_SPEED, -1, 1);
		if (ImGui::TreeNode("Meshes"))
		{
			auto mMeshes = GetMeshes();
			for (int i = 0; i < mMeshes.size(); i++) {
				if (ImGui::TreeNode((std::string("mesh_") + std::to_string(i).data()).c_str()))
				{
					mMeshes[i].OnImGuiRender();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

	}

	esgstl::vector<Mesh*> Model::GetViewableMeshes(const std::array<Plane, 6>& frustum) const
	{
		esgstl::vector<Mesh*> viewableMeshes;
		auto root = m_BVH->root;
		esgstl::queue<Ref<BVHBuildNode<Mesh>>> queue;
		queue.push(root);
		while (!queue.empty()) {
			auto node = queue.front();
			queue.pop();
			// 如果相交，则进一步探寻子节点
			if (!node->bounds.OverlapFrustum(frustum)) {
				// 如果已经是叶子节点，则将叶子节点的model加入结果集中
				if (node->left == nullptr && node->right == nullptr)
					viewableMeshes.push_back(node->object);
				else {
					// 否则向下探寻子节点的相交关系
					queue.push(node->left);
					queue.push(node->right);
				}
			}
		}
		return viewableMeshes;
	}

}
