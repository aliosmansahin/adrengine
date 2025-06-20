#include "pch.h"
#include "GetPositionVector.h"

#include "interfaces/IEntity/IEntity.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetPositionVector::Evaluate(Pin* pin)
{
    Value entityValue = EvaluateInput(inputPins[0].get());

    if (!std::holds_alternative<IEntity*>(entityValue)) {
        return std::monostate{};
    }

    IEntity* entity = std::get<IEntity*>(entityValue);

    glm::vec3 vector = entity->GetEntityParams()->GetPosition();

    return vector;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("PositionVector", PinType::Vector3, PinDirection::Output, this),
    };
}