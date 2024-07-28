#include "emtpch.h"
#include "Model.h"

namespace EMT {
	unsigned int Model::noNameNum = 0;

	Model::Model(const std::string& path) {
		LoadModel(path);
	}

	Model::Model(const Mesh& mesh) {
		mMeshes.push_back(mesh);
		mName = "None_";
		mName += std::to_string(noNameNum);
		noNameNum++;
	}

	Model::Model(const std::vector<Mesh>& meshes) {
		mMeshes = meshes;
		mName = "None_";
		mName += std::to_string(noNameNum);
		noNameNum++;
	}


	void Model::Draw(const Ref<Shader>& shader, bool isUseMaterial) const {

		for (unsigned int i = 0; i < mMeshes.size(); ++i) {

			if (isUseMaterial) {
				mMeshes[i].m_Material->BindMaterial(shader);

				// check whether to use normal mapping
				if (mMeshes[i].HasTangents())
					shader->setInt("useNormalMapping", 1);
				else
					shader->setInt("useNormalMapping", 0);
			}
			mMeshes[i].Draw();
		}
	}

	void Model::LoadModel(const std::string& path) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			EMT_CORE_ERROR("ERROR::ASSIMP:: {0}",import.GetErrorString());
			return;
		}

		mDirectory = path.substr(0, path.find_last_of('/'));
		mName = mDirectory.substr(mDirectory.find_last_of('/') + 1, mDirectory.size());

		ProcessNode(scene->mRootNode, scene);
	}


	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			// 获取节点中的网格数据
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			mMeshes.push_back(ProcessMesh(mesh, scene));
		}

		// 递归处理子节点
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		// 开辟顶点数据所需空间
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<unsigned int> indices;

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
			for (int i = 0; i < mMeshes.size(); i++) {
				if (ImGui::TreeNode((std::string("mesh_") + std::to_string(i)).c_str()))
				{
					mMeshes[i].OnImGuiRender();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

	}

}
