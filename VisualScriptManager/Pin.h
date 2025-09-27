#pragma once

#include <variant>
#include <string>
#include <any>
#include <iostream>

#include "glm/glm.hpp"

//Forward declarations
class IEntity;
class IObject;
class Node;

//Properties for pins
enum class PinType { Exec, Bool, Int, Float, String, Entity, Object, Vector3, Any };
enum PinDirection { Input, Output };

//Value will be used in the connections to transfer data between nodes
using Value = std::variant<std::monostate, bool, int, float, std::string, IEntity*, IObject*, glm::vec3>;

/*
	Pin is some kind of connection point which allows us to connect nodes between each other
	A pin has a type for the value,
		only pins that match their value type can be connected
		(Any type can be connected to any pin type)
	A pin has a direction, only an input pin and an output pin can be connected
*/
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