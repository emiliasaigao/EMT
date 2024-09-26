#pragma once
#include "EMT/Core.h"
#include "Ray.h"
#include "Model/AABB.h"
#include <stack>

namespace EMT {
    template <class T>
    struct BVHBuildNode {
        AABB srcBounds;
        AABB curBounds;
        Ref<BVHBuildNode<T>> parent;
        Ref<BVHBuildNode<T>> left;
        Ref<BVHBuildNode<T>> right;
        T* object;
        BVHBuildNode() :srcBounds(), curBounds(), parent(nullptr), left(nullptr), right(nullptr), object(nullptr) {}
    };

    template <class T>
    class BVHAccel {
    public:
        BVHAccel(const esgstl::vector<T>& p);
        BVHAccel(esgstl::vector<T>&& p);
        const AABB& WorldBound() const;
        ~BVHAccel() {}

        inline esgstl::vector<T>& GetPrimitives() { return m_Primitives; }

        void UpdateGlobal(const glm::mat4& transform);
        Intersection Intersect(const Ray& ray) const;
        bool IntersectP(const Ray& ray) const;

    public:
        Ref<BVHBuildNode<T>> root;

    private:
        struct BVHBuildTask {
            esgstl::vector<T*> objects;
            Ref<BVHBuildNode<T>> node;
        };
        // BVHAccel Private Methods
        Ref<BVHBuildNode<T>> recursiveBuild(esgstl::vector<T*> objects);
        Intersection getIntersection(const Ref<BVHBuildNode<T>>& node, const Ray& ray)const;

        // BVHAccel Private Data
        esgstl::vector<T> m_Primitives;
        glm::mat4 m_Transform = glm::mat4(1);
    };



    template<class T>
    BVHAccel<T>::BVHAccel(const esgstl::vector<T>& p)
        : m_Primitives(p) {
        esgstl::vector<T*> objectPtrs(m_Primitives.size());
        std::transform(m_Primitives.begin(), m_Primitives.end(), objectPtrs.begin(), [](T& object) {return &object;});
        root = recursiveBuild(objectPtrs);
    }

    template<class T>
    BVHAccel<T>::BVHAccel(esgstl::vector<T>&& p)
        : m_Primitives(std::move(p)) {
        esgstl::vector<T*> objectPtrs(m_Primitives.size());
        std::transform(m_Primitives.begin(), m_Primitives.end(), objectPtrs.begin(), [](T& object) {return &object;});
        root = recursiveBuild(objectPtrs);
    }

    template<class T>
    inline const AABB& BVHAccel<T>::WorldBound() const {
        return root->curBounds;
    }

    template<class T>
    inline Intersection BVHAccel<T>::Intersect(const Ray& ray) const {
        Intersection intersec;
        if (!root) return intersec;
        return getIntersection(root, ray);
    }

    template<class T>
    inline bool BVHAccel<T>::IntersectP(const Ray& ray) const {
        std::array<int, 3> dirIsNeg = { int(ray.direction.x > 0),int(ray.direction.y > 0),int(ray.direction.z > 0) };
        return root->bounds.IntersetcP(ray, ray.direction_inv, dirIsNeg);
    }

    template<class T>
    Ref<BVHBuildNode<T>> BVHAccel<T>::recursiveBuild(esgstl::vector<T*> objects) {
        std::stack<BVHBuildTask> taskStack; // 用于模拟递归的栈
        Ref<BVHBuildNode<T>> root = std::make_shared<BVHBuildNode<T>>();
        taskStack.push(BVHBuildTask{ objects, root }); // 初始任务

        while (!taskStack.empty()) {
            auto task = taskStack.top();
            taskStack.pop();
            esgstl::vector<T*> objs = task.objects;
            Ref<BVHBuildNode<T>> node = task.node;
            // 计算总的包围盒
            AABB bounds;
            for (int i = 0; i < objs.size(); ++i)
                bounds = Union(bounds, objs[i]->GetAABB());
            // 如果只有一个对象了，则这是叶子节点
            if (objs.size() == 1) {
                node->srcBounds = objs[0]->GetAABB();
                node->curBounds = node->srcBounds;
                node->object = objs[0];
                node->object->SetBVHNode(node);
                node->left = nullptr;
                node->right = nullptr;
            }
            // 如果只有两个对象了，一边一个叶子节点
            else if (objs.size() == 2) {
                node->left = std::make_shared<BVHBuildNode<T>>();
                node->right = std::make_shared<BVHBuildNode<T>>();
                node->left->parent = node;
                node->right->parent = node;
                taskStack.push(BVHBuildTask{ {task.objects[0]}, node->left });
                taskStack.push(BVHBuildTask{ {task.objects[1]}, node->right });

                node->srcBounds = Union(node->left->srcBounds, node->right->srcBounds);
                node->curBounds = node->srcBounds;
            }
            // 否则，用表面积启发算法寻找相对最优子树构建
            else {
                // 注意这里是总质心包围盒，前面是总包围盒
                AABB centroidBounds;
                for (int i = 0; i < objs.size(); ++i) {
                    centroidBounds = Union(centroidBounds, objs[i]->GetAABB().Centroid());
                }
                double sum_area = centroidBounds.SurfaceArea();
                // 如果总质心包围盒的表面积为0，则说明所有对象的质心集中在一点或者两点之上，直接对半分就行了
                if (sum_area == 0) {
                    auto beginning = objs.begin();
                    auto middling = objs.begin() + objs.size() / 2;
                    auto ending = objs.end();
                    node->left = std::make_shared<BVHBuildNode<T>>();
                    node->right = std::make_shared<BVHBuildNode<T>>();
                    node->left->parent = node;
                    node->right->parent = node;
                    esgstl::vector<T*> leftshapes = esgstl::vector<T*>(beginning, middling);
                    esgstl::vector<T*> rightshapes = esgstl::vector<T*>(middling, ending);
                    taskStack.push(BVHBuildTask{ std::move(leftshapes), node->left });
                    taskStack.push(BVHBuildTask{ std::move(rightshapes), node->right });

                    node->srcBounds = bounds;
                    node->curBounds = node->srcBounds;
                    continue;
                }

                // 这个B决定了每次遍历多少种切法
                int B = 15;
                // 最小花费及对应的切法
                float minCost = std::numeric_limits<float>::max();
                esgstl::vector<T*> bestleftshapes, bestrightshapes;

                // 这里还是对长轴来一刀，当然，按理来说，可以对三个轴都来一刀，然后取最小花费的切法
                int dim = centroidBounds.maxExtent();
                switch (dim) {
                case 0:
                    std::sort(objs.begin(), objs.end(), [](auto f1, auto f2) {
                        return f1->GetAABB().Centroid().x <
                        f2->GetAABB().Centroid().x;
                        });
                    break;
                case 1:
                    std::sort(objs.begin(), objs.end(), [](auto f1, auto f2) {
                        return f1->GetAABB().Centroid().y <
                        f2->GetAABB().Centroid().y;
                        });
                    break;
                case 2:
                    std::sort(objs.begin(), objs.end(), [](auto f1, auto f2) {
                        return f1->GetAABB().Centroid().z <
                        f2->GetAABB().Centroid().z;
                        });
                    break;
                }

                // 遍历每种切法，寻找最小花费切法
                for (int j = 1; j < B; j++) {
                    auto beginning = objs.begin();
                    auto middling = objs.begin() + j * objs.size() / B;
                    auto ending = objs.end();
                    if (beginning == middling || middling == ending) continue;
                    esgstl::vector<T*> leftshapes = esgstl::vector<T*>(beginning, middling);
                    esgstl::vector<T*> rightshapes = esgstl::vector<T*>(middling, ending);

                    // 计算这种切法的两个新质心包围盒的表面积
                    AABB leftbounds, rightbounds;
                    for (int m = 0; m < leftshapes.size(); m++) {
                        leftbounds = Union(leftbounds, leftshapes[m]->GetAABB().Centroid());

                    }
                    for (int n = 0; n < rightshapes.size(); n++)
                        rightbounds = Union(rightbounds, rightshapes[n]->GetAABB().Centroid());
                    double l_area = leftbounds.SurfaceArea();
                    double r_area = rightbounds.SurfaceArea();

                    // 计算花费，与之前的最小花费比较，记录更小的那个
                    float cost = l_area / sum_area * leftshapes.size() + r_area / sum_area * rightshapes.size();
                    if (cost < minCost) {
                        minCost = cost;
                        bestleftshapes = leftshapes;
                        bestrightshapes = rightshapes;
                        EMT_ASSERT(objs.size() == (leftshapes.size() + rightshapes.size()), "BVH建树异常！");
                    }
                }

                node->left = std::make_shared<BVHBuildNode<T>>();
                node->right = std::make_shared<BVHBuildNode<T>>();
                node->left->parent = node;
                node->right->parent = node;
                taskStack.push(BVHBuildTask{ std::move(bestleftshapes), node->left });
                taskStack.push(BVHBuildTask{ std::move(bestrightshapes), node->right });

                node->srcBounds = bounds;
                node->curBounds = node->srcBounds;
            }
        }

        return root;
    }

    template<class T>
    Intersection BVHAccel<T>::getIntersection(const Ref<BVHBuildNode<T>>& node, const Ray& ray) const {
        // 前序遍历BVH树
        Intersection intersection;
        std::array<int, 3> dirIsNeg = { int(ray.direction.x > 0),int(ray.direction.y > 0),int(ray.direction.z > 0) };
        if (!node->curBounds.IntersectP(ray, ray.direction_inv, dirIsNeg)) return intersection;
        if (!(node->left) && !(node->right)) {
            return node->object->getIntersection(ray);
        }
        Intersection i1 = getIntersection(node->left, ray);
        Intersection i2 = getIntersection(node->right, ray);
        // 返回光的步进距离更小的那个
        return i1.distance > i2.distance ? i2 : i1;
    }

    template<class T>
    void BVHAccel<T>::UpdateGlobal(const glm::mat4& transform) {
        std::stack<Ref<BVHBuildNode<T>>> stc;
        stc.push(root);
        while (!stc.empty()) {
            auto node = stc.top();
            stc.pop();
            if (node) {
                stc.push(node);
                stc.push(nullptr);
                if (node->right) stc.push(node->right);
                if (node->left) stc.push(node->left);
            }
            else {
                node = stc.top();
                stc.pop();
                if (node->left == nullptr && node->right == nullptr) node->curBounds = Transform(node->srcBounds, transform);
                else node->curBounds = Union(node->left->curBounds, node->right->curBounds);
            }
        }
    }
}