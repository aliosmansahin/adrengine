#pragma once

#include <variant>
#include "Entity.h"

enum class PinType { Exec, Bool, Int, Float, String };
enum PinDirection { Input, Output };

using Value = std::variant<std::monostate, bool, int, float, std::string, Entity*>;

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