#pragma once
#include "EMT/Core.h"
#include <glm/glm.hpp>
#include "AABB.h"
#include "EMT/Renderer/BVH.h"
#include "EMT/Renderer/Ray.h"

namespace EMT {
	class Triangle {
	public:
		Triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, const glm::mat4* transform = nullptr)
			: m_p0(p0), m_p1(p1), m_p2(p2) {
			m_Transform = transform;
			m_e1 = m_p1 - m_p0;
			m_e2 = m_p2 - m_p0;
			m_Normal = glm::normalize(glm::cross(m_e1, m_e2));
		}

		Intersection GetIntersection(const Ray& ray);

		inline const AABB& Triangle::GetAABB() { return Union(AABB(m_p0, m_p1), m_p2); }
		inline void SetBVHNode(Ref<BVHBuildNode<Triangle>> node) {
			m_BVH_Node = node;
		}
		inline void SetTransform(const glm::mat4& transform) { m_Transform = &transform; }

		glm::vec3 m_p0, m_p1, m_p2;		// 三点坐标
	private:
		glm::vec3 m_e1, m_e2;			// 两边向量
		//glm::vec2 m_t0, m_t1, m_t2;		// 三点纹理坐标，暂时不需要？
		glm::vec3 m_Normal;				// 三角形法向量
		Ref<BVHBuildNode<Triangle>> m_BVH_Node;
		const glm::mat4* m_Transform;
	};
}