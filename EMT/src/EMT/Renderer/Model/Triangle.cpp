#include "emtpch.h"
#include "Triangle.h"

namespace EMT {
    Intersection Triangle::GetIntersection(Ray ray) {
        Intersection inter;

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
