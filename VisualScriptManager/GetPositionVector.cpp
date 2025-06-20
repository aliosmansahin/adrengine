#include "pch.h"
#include "GetPositionVector.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetPositionVector::Evaluate(Pin* pin)
{
    Value entityValue = EvaluateInput(inputPins[0].get());

    if (!std::holds_alternative<Entity*>(entityValue)) {
        return std::monostate{};
    }

    Entity* entity = std::get<Entity*>(entityValue);

    //glm::vec3 vector = glm::vec3(entity->GetEntityParams()->x, entity->GetEntityParams()->y, entity->GetEntityParams()->z);
    glm::vec3 vector = glm::vec3(1.0f);

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