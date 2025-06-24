#pragma once

#include <variant>
#include <string>
#include <any>
#include <iostream>

#include "glm/glm.hpp"

class IEntity;
class IObject;

enum class PinType { Exec, Bool, Int, Float, String, Entity, Object, Vector3, Any };
enum PinDirection { Input, Output };

using Value = std::variant<std::monostate, bool, int, float, std::string, IEntity*, IObject*, glm::vec3>;

class Node;

class Pin
{
public:
	Pin(std::string name, PinType type, PinDirection direction, Node* parentNode) : name(name), type(type), direction(direction), parentNode(parentNode) {};
	virtual ~Pin() = default;
	PinType type;
	PinDirection direction;
	std::string name;
	int id = 0;

	Node* parentNode = nullptr;
	Pin* connectedTo = nullptr;
};