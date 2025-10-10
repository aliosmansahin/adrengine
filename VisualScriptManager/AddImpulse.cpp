#include "pch.h"
#include "AddImpulse.h"

#include "interfaces/IEntity/IObject/IObject.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void AddImpulse::Execute()
{
    Value objectValue = EvaluateInput(inputPins[1]);
    Value impulseValue = EvaluateInput(inputPins[2]);

    if (std::holds_alternative<std::shared_ptr<IObject>>(objectValue) && std::holds_alternative<glm::vec3>(impulseValue)) {
        std::shared_ptr<IObject> object = std::get<std::shared_ptr<IObject>>(objectValue);
        glm::vec3 impulse = std::get<glm::vec3>(impulseValue);

		object->AddImpulse(impulse); //This is inactive for now, because physics engine is changed
    }

    std::shared_ptr<INode> next = GetNextExecNode(outputPins[0]);
    if (next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void AddImpulse::SetPins()
{
    std::shared_ptr<INode> thisNode = shared_from_this();

    inputPins = {
           std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, thisNode),
           std::make_shared<Pin>("Object", PinType::Object, PinDirection::Input, thisNode),
           std::make_shared<Pin>("ImpulseVector3", PinType::Vector3, PinDirection::Input, thisNode),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, thisNode),
    };
}
