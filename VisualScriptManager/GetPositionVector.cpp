#include "pch.h"
#include "GetPositionVector.h"

#include "interfaces/IEntity/IEntity.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetPositionVector::Evaluate(std::shared_ptr<IPin> pin)
{
    Value entityValue = EvaluateInput(inputPins[0]);

    if (!std::holds_alternative<std::shared_ptr<IEntity>>(entityValue)) {
        return std::monostate{};
    }

    std::shared_ptr<IEntity> entity = std::get<std::shared_ptr<IEntity>>(entityValue);

    glm::vec3 vector = entity->GetEntityParams()->GetPosition();

    return vector;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::SetPins()
{
	std::shared_ptr<INode> thisNode = shared_from_this();

    inputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, thisNode),
    };
    outputPins = {
        std::make_shared<Pin>("PositionVector", PinType::Vector3, PinDirection::Output, thisNode),
    };
}