#pragma once

#include <glm/glm.hpp>

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

enum class CollisionType {
	AABB,
	Sphere
};

class CollisionShape
{
public:
	CollisionType type;

	//Each collision type
	union {
		struct {
			glm::vec3 halfExtents;
		} aabb;

		struct {
			float radius;
		} sphere;
	};
};