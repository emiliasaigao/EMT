#pragma once
#include <limits>
#include <array>
#include "../Ray.h"
#include "glm/glm.hpp"

namespace EMT {
    // 平面结构体，用法向量和到原点的距离表示
    struct Plane {
        glm::vec3 normal;  // 平面的法向量
        float distance;    // 平面到原点的距离

        // 用于归一化平面
        inline void normalize() {
            float length = glm::length(normal);
            if (length > 0.0f) {
                normal /= length;
                distance /= length;
            }
        }
    };


    // 轴对齐包围盒
    class AABB {
    public:
        glm::vec3 pMin, pMax; // two points to specify the bounding box
        AABB() {
            constexpr float minNum = std::numeric_limits<float>::lowest();
            constexpr float maxNum = std::numeric_limits<float>::max();
            pMax = glm::vec3(minNum, minNum, minNum);
            pMin = glm::vec3(maxNum, maxNum, maxNum);
        }
        AABB(const glm::vec3 p) : pMin(p), pMax(p) {}
        AABB(const glm::vec3 p1, const glm::vec3 p2) {
            pMin = glm::vec3(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
            pMax = glm::vec3(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
        }

        glm::vec3 Diagonal() const { return pMax - pMin; }

        // 最长边 0-x, 1-y, 2-z
        int maxExtent() const {
            glm::vec3 d = Diagonal();
            if (d.x > d.y && d.x > d.z)
                return 0;
            else if (d.y > d.z)
                return 1;
            else
                return 2;
        }

        // 计算表面积
        double SurfaceArea() const {
            glm::vec3 d = Diagonal();
            return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
        }

        inline glm::vec3 Centroid() const { return 0.5f * pMin + 0.5f * pMax; }
        AABB Intersect(const AABB& b) {
            return AABB(glm::vec3(fmax(pMin.x, b.pMin.x), fmax(pMin.y, b.pMin.y),
                fmax(pMin.z, b.pMin.z)),
                glm::vec3(fmin(pMax.x, b.pMax.x), fmin(pMax.y, b.pMax.y),
                    fmin(pMax.z, b.pMax.z)));
        }

        bool IntersectP(const Ray& ray, const glm::vec3& invDir, const std::array<int, 3>& dirIsNeg) const {
            // 注意，disIsNeg是记录光线方向正负的数组
            // std::array<int, 3> dirIsNeg = 
            // { int(ray.direction.x > 0),int(ray.direction.y > 0),int(ray.direction.z > 0) };
            // 因为如果光线方向是正的，那么它肯定先过坐标值较小的那个轴平面
            // 下面的逻辑都是基于光线方向是正的情况的，所以如果实际光线方向为负，则需要交换一下tmin和tmax

            glm::vec3 tmin = (pMin - ray.origin) * invDir;
            glm::vec3 tmax = (pMax - ray.origin) * invDir;

            for (int i = 0; i < 3; i++) {
                if (!dirIsNeg[i]) std::swap(tmin[i], tmax[i]);
            }

            float texit = std::min(tmax.x, std::min(tmax.y, tmax.z));
            float tenter = std::max(tmin.x, std::max(tmin.y, tmin.z));
            return tenter <= texit && texit > 0;
        }

        glm::vec3 Offset(const glm::vec3& p) const {
            glm::vec3 o = p - pMin;
            if (pMax.x > pMin.x)
                o.x /= pMax.x - pMin.x;
            if (pMax.y > pMin.y)
                o.y /= pMax.y - pMin.y;
            if (pMax.z > pMin.z)
                o.z /= pMax.z - pMin.z;
            return o;
        }

        bool Overlaps(const AABB& b1, const AABB& b2) {
            bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
            bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
            bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
            return (x && y && z);
        }

        bool Inside(const glm::vec3& p, const AABB& b) {
            return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y &&
                p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
        }

        bool OverlapFrustum(const std::array<Plane, 6>& frustum) {
            for (int i = 0; i < 6; ++i) {
                auto posVertex = getPositiveVertex(frustum[i].normal);
                //auto negVertex = getNegativeVertex(frustum[i].normal);
                // 只要包围盒在视锥体某一个面的外侧，则直接返回false
                if (glm::dot(frustum[i].normal, posVertex) + frustum[i].distance < 0) return false;
            }
            return true;
        }

        inline const glm::vec3& operator[](int i) const {
            return (i == 0) ? pMin : pMax;
        }

        void Transform(const glm::mat4& transform) {
            // AABB 的 8 个顶点
            glm::vec3 vertices[8] = {
                pMin,
                glm::vec3(pMax.x, pMin.y, pMin.z),
                glm::vec3(pMin.x, pMax.y, pMin.z),
                glm::vec3(pMin.x, pMin.y, pMax.z),
                glm::vec3(pMax.x, pMax.y, pMin.z),
                glm::vec3(pMin.x, pMax.y, pMax.z),
                glm::vec3(pMax.x, pMin.y, pMax.z),
                pMax
            };

            // 初始化变换后的最小点和最大点
            glm::vec3 newMin = glm::vec3(std::numeric_limits<float>::max());
            glm::vec3 newMax = glm::vec3(std::numeric_limits<float>::lowest());

            // 对 8 个顶点应用变换矩阵
            for (int i = 0; i < 8; ++i) {
                glm::vec4 transformedVertex = transform * glm::vec4(vertices[i], 1.0f);
                glm::vec3 transformedPos = glm::vec3(transformedVertex); // 忽略 w 分量

                // 更新新的 AABB 的最小和最大点
                newMin = glm::min(newMin, transformedPos);
                newMax = glm::max(newMax, transformedPos);
            }

            pMax = newMax;
            pMin = newMin;
        }

    private:
        // 计算最远点（Positive Vertex）
        glm::vec3 getPositiveVertex(const glm::vec3& normal) const {
            return glm::vec3(
                (normal.x > 0) ? pMax.x : pMin.x,
                (normal.y > 0) ? pMax.y : pMin.y,
                (normal.z > 0) ? pMax.z : pMin.z
            );
        }

        // 计算最近点（Negative Vertex）
        glm::vec3 getNegativeVertex(const glm::vec3& normal) const {
            return glm::vec3(
                (normal.x > 0) ? pMin.x : pMax.x,
                (normal.y > 0) ? pMin.y : pMax.y,
                (normal.z > 0) ? pMin.z : pMax.z
            );
        }
    };

    inline AABB Union(const AABB& b1, const AABB& b2) {
        AABB ret;
        ret.pMin = glm::min(b1.pMin, b2.pMin);
        ret.pMax = glm::max(b1.pMax, b2.pMax);
        return ret;
    }

    inline AABB Union(const AABB& b, const glm::vec3& p) {
        AABB ret;
        ret.pMin = glm::min(b.pMin, p);
        ret.pMax = glm::max(b.pMax, p);
        return ret;
    }

    inline AABB Transform(const AABB& b, const glm::mat4& transform) {
        AABB res = b;
        res.Transform(transform);
        return res;
    }
}