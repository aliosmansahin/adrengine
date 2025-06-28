#pragma once

#include <variant>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#ifdef PHYSICS_EXPORTS
#define PHYSICS_API __declspec(dllexport)
#else
#define PHYSICS_API __declspec(dllimport)
#endif

enum class CollisionType {
	OBB,
	Sphere
};

//Each collision type
struct OBB {
	glm::vec3 halfExtents = glm::vec3(1.0f);
	glm::vec3 center;
	glm::mat3 orientation;
	glm::vec3 velocity;
};

struct Sphere {
	float radius;
};

class CollisionShape
{
public:
	CollisionType type;

	std::variant<OBB, Sphere> shape;
};