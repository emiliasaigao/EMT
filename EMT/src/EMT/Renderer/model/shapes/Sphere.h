#pragma once
#include "../Mesh.h"
#include "glm/gtc/constants.hpp"
namespace EMT {
	class Sphere : public Mesh
	{
	public:
		Sphere(int xSegments = 30, int ySegments = 30);
		~Sphere();
	};
}


