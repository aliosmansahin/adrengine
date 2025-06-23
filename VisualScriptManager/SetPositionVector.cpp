#include "pch.h"
#include "SetPositionVector.h"

#include "interfaces/IEntity/IEntity.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void SetPositionVector::Execute()
{
    Value entityValue = EvaluateInput(inputPins[1].get());
    Value positionValue = EvaluateInput(inputPins[2].get());

    if (std::holds_alternative<IEntity*>(entityValue) && std::holds_alternative<glm::vec3>(positionValue)) {
        IEntity* entity = std::get<IEntity*>(entityValue);
        glm::vec3 vector = std::get<glm::vec3>(positionValue);

        entity->GetEntityParams()->SetPosition(vector);
    }

    Node* next = GetNextExecNode(outputPins[0].get());
    if (next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void SetPositionVector::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, this),
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, this),
        std::make_shared<Pin>("PositionVector", PinType::Vector3, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, this),
    };
}