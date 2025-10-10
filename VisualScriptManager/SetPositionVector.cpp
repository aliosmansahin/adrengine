#include "pch.h"
#include "SetPositionVector.h"

#include "interfaces/IEntity/IEntity.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void SetPositionVector::Execute()
{
    Value entityValue = EvaluateInput(inputPins[1]);
    Value positionValue = EvaluateInput(inputPins[2]);

    if (std::holds_alternative<std::shared_ptr<IEntity>>(entityValue) && std::holds_alternative<glm::vec3>(positionValue)) {
        std::shared_ptr<IEntity> entity = std::get<std::shared_ptr<IEntity>>(entityValue);
        glm::vec3 vector = std::get<glm::vec3>(positionValue);

        entity->GetEntityParams()->SetRuntimePosition(vector);
    }

    std::shared_ptr<INode> next = GetNextExecNode(outputPins[0]);
    if (next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void SetPositionVector::SetPins()
{
	std::shared_ptr<INode> thisNode = shared_from_this();

    inputPins = {
        std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, thisNode),
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, thisNode),
        std::make_shared<Pin>("PositionVector", PinType::Vector3, PinDirection::Input, thisNode),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, thisNode),
    };
}