#pragma once

#include <variant>
#include <string>
#include <any>
#include <iostream>

#include "interfaces/IVisualScript/IPin/IPin.h"

/*
	Pin is some kind of connection point which allows us to connect nodes between each other
	A pin has a type for the value,
		only pins that match their value type can be connected
		(Any type can be connected to any pin type)
	A pin has a direction, only an input pin and an output pin can be connected
*/
class Pin : public IPin
{
public:
	Pin(std::string name, PinType type, PinDirection direction, std::shared_ptr<INode> parentNode) : name(name), type(type), direction(direction), parentNode(parentNode) {};
	virtual ~Pin() = default;

	PinType GetType() const override { return type; }
	PinDirection GetDirection() const override { return direction; }
	std::string GetName() const override { return name; }

	int GetId() const override { return id; }
	void SetId(int newId) override { id = newId; }

	std::shared_ptr<INode> GetParentNode() const override { return parentNode; }
	void SetParentNode(std::shared_ptr<INode> node) override { parentNode = node; }

	std::shared_ptr<IPin> GetConnectedPin() const override { return connectedTo; }
	void SetConnectedPin(std::shared_ptr<IPin> pin) override { connectedTo = pin; }

private:
	PinType type;
	PinDirection direction;
	std::string name;
	int id = 0;

	std::shared_ptr<INode> parentNode = nullptr;
	std::shared_ptr<IPin> connectedTo = nullptr;
};