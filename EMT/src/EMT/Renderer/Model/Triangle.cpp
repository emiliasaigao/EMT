#include "emtpch.h"
#include "Triangle.h"

namespace EMT {
    Intersection Triangle::GetIntersection(const Ray& ray) {
        Intersection inter;
        if (m_Transform && *m_Transform != glm::mat4(1)) {
            const glm::mat4& M = *m_Transform;
            glm::vec4 v1_transformed = M * glm::vec4(m_p0, 1.f);
            glm::vec4 v2_transformed = M * glm::vec4(m_p1, 1.f);
            glm::vec4 v3_transformed = M * glm::vec4(m_p2, 1.f);
            m_p0 = glm::vec3(v1_transformed / v1_transformed.w);
            m_p1 = glm::vec3(v2_transformed / v2_transformed.w);
            m_p2 = glm::vec3(v3_transformed / v3_transformed.w);

            m_e1 = m_p1 - m_p0;
            m_e2 = m_p2 - m_p0;
            m_Normal = glm::normalize(glm::cross(m_e1, m_e2));
        }

        if (glm::dot(ray.direction, m_Normal) > 0)
            return inter;
        double u, v, t_tmp = 0;
        glm::vec3 pvec = glm::cross(ray.direction, m_e2);
        double det = glm::dot(m_e1, pvec);
        if (fabs(det) < EPSILON) return inter;

        double det_inv = 1. / det;
        glm::vec3 tvec = ray.origin - m_p0;
        u = glm::dot(tvec, pvec) * det_inv;
        if (u < 0 || u > 1)
            return inter;
        glm::vec3 qvec = glm::cross(tvec, m_e1);
        v = glm::dot(ray.direction, qvec) * det_inv;
        if (v < 0 || u + v > 1)
            return inter;
        t_tmp = glm::dot(m_e2, qvec) * det_inv;
        if (t_tmp < 0) {
            return inter;
        }
        inter.happened = true;
        inter.coords = ray(t_tmp);
        inter.normal = m_Normal;
        inter.distance = t_tmp;
        // 交点的model由上层函数指定
        return inter;
    }
}
