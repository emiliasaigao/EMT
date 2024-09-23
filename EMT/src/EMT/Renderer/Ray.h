#pragma once
#include "EMT/Core.h"
#include <glm/glm.hpp>

namespace EMT {
    class Model;

    struct Intersection {
        bool happened;
        glm::vec3 coords;
        glm::vec3 normal;
        double distance;
        Ref<Model> model;
        Intersection():happened(false), coords(),normal(),distance(std::numeric_limits<double>::max()),model(nullptr) {}
    };

    struct Ray {
        //Destination = origin + t*direction
        glm::vec3 origin;
        glm::vec3 direction, direction_inv;
        double t;//transportation time,
        double t_min, t_max;

        Ray(const glm::vec3& ori, const glm::vec3& dir, const double _t = 0.0) : origin(ori), direction(dir), t(_t) {
            direction_inv = glm::vec3(1. / direction.x, 1. / direction.y, 1. / direction.z);
            t_min = 0.0;
            t_max = std::numeric_limits<double>::max();
        }

        glm::vec3 operator()(double t) const { return origin + float(t) * direction; }

    };
}