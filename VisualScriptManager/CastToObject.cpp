#include "pch.h"
#include "CastToObject.h"

#include "interfaces/IEntity/IObject/IObject.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value CastToObject::Evaluate(std::shared_ptr<IPin> pin)
{
    Value entityValue = EvaluateInput(inputPins[0]);

    if (!std::holds_alternative<std::shared_ptr<IEntity>>(entityValue)) {
        std::monostate{};
    }

    std::shared_ptr<IEntity> entity = std::get<std::shared_ptr<IEntity>>(entityValue);

    std::shared_ptr<IObject> casted = std::dynamic_pointer_cast<IObject>(entity);

    if (!casted)
        return std::monostate{};

    return casted;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void CastToObject::SetPins()
{
	std::shared_ptr<INode> thisNode = shared_from_this();

    inputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, thisNode),
    };

    outputPins = {
        std::make_shared<Pin>("Object", PinType::Object, PinDirection::Output, thisNode),
    };
}
