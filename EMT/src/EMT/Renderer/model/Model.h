#pragma once
#include "glm/glm.hpp"
#include "Mesh.h"
#include "EMT/Renderer/Texture/TextureLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "AssimpGLMHelper.h"


namespace EMT {
	class Model
	{
	public:
		Model(){}
		explicit Model(const std::string& path);
		explicit Model(const Mesh& mesh);
		explicit Model(const esgstl::vector<Mesh>& meshes);
		Model(Model&& model);
		Model(const Model& model);
		Model& operator=(Model&& model);
		Model& operator=(const Model& model);
		~Model() {}

		void Draw(const Ref<Shader>& shader, const std::array<Plane, 6>& frustum, bool isUseMaterial) const;
		void OnImGuiRender();
		esgstl::vector<Mesh*> GetViewableMeshes(const std::array<Plane, 6>& frustum) const;
		void UpdateBVH();
		glm::mat4 GetModelMatrix() const;

		// Getters and Setters
		inline const glm::vec3& GetCenter() const { return mCenter; }
		inline const glm::vec3& GetPosition() const { return mPosition; }
		inline const glm::vec3& GetScale() const { return mScale; }
		inline const float& GetRotation() const { return mRotation; }
		inline const glm::vec3& GetRotateAxis() const { return mRotateAxis; }
		inline esgstl::vector<Mesh>& GetMeshes() const { return m_BVH->GetPrimitives(); }
		inline Mesh* GetMesh(int index = 0) const { return &(m_BVH->GetPrimitives()[index]); }
		inline const std::string& GetName() { return mName; }
		inline const AABB& GetAABB() const { return m_BVH->WorldBound(); }
		Intersection GetIntersection(const Ray& ray) {
			auto res = m_BVH->Intersect(ray);
			if (res.happened) {
				res.model = this;
			}
			return res;
		}

		inline void SetPosition(const glm::vec3& pos) { 
			hasTransformed = true;
			mPosition = pos; 
		}
		inline void SetScale(const glm::vec3& scale) { 
			hasTransformed = true;
			mScale = scale; 
		}
		inline void SetRotation(const float rotation) {
			hasTransformed = true;
			mRotation = rotation; 
		}
		inline void SetRotateAxis(const glm::vec3& axis) {
			hasTransformed = true;
			mRotateAxis = axis; 
		}
		inline void SetBVHNode(Ref<BVHBuildNode<Model>> node) {
			m_BVH_Node = node;
		}

	public:
		// 相对原始模型是否发生了变换
		bool hasTransformed = false;
		bool IsSelected = false;

	private:
		void LoadModel(const std::string& path);

		void ProcessNode(aiNode* node, const aiScene* scene, esgstl::vector<Mesh>& meshes);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		Ref<Texture> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, bool isSRGB);


	private:
		std::string mDirectory;
		std::string mName;

		// model parameters
		glm::vec3 mCenter = glm::vec3(0.0f, 0.0f, 0.0f);		// center is from the original model data
		glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);	// position is what we want to set
		glm::vec3 mScale = glm::vec3(1.0f, 1.0f, 1.0f); ;
		float mRotation = 0.0f;	 glm::vec3 mRotateAxis = glm::vec3(0.0f, 0.0f, 1.0f);
		static unsigned int noNameNum;

		Ref<BVHAccel<Mesh>> m_BVH;
		Ref<BVHBuildNode<Model>> m_BVH_Node;
	};
}