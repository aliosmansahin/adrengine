#pragma once

#include <variant>
#include <string>
#include <any>
#include <memory>
#include <glm/glm.hpp>

//Forward declarations
class IEntity;
class IObject;
class INode;

//Properties for pins
enum class PinType { Exec, Bool, Int, Float, String, Entity, Object, Vector3, Any };
enum PinDirection { Input, Output };

//Value will be used in the connections to transfer data between nodes
using Value = std::variant<std::monostate, bool, int, float, std::string, std::shared_ptr<IEntity>, std::shared_ptr<IObject>, glm::vec3>;

class IPin
{
public:
	virtual ~IPin() = default;
	virtual PinType			GetType() const = 0;
	virtual PinDirection	GetDirection() const = 0;
	virtual std::string		GetName() const = 0;

	virtual int				GetId() const = 0;
	virtual void			SetId(int id) = 0;

	virtual std::shared_ptr<INode> GetParentNode() const = 0;
	virtual void			SetParentNode(std::shared_ptr<INode> node) = 0;

	virtual std::shared_ptr<IPin> GetConnectedPin() const = 0;
	virtual void			SetConnectedPin(std::shared_ptr<IPin> pin) = 0;
};