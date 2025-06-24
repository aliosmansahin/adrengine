#include "pch.h"
#include "AddImpulse.h"

#include "interfaces/IEntity/IObject/IObject.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void AddImpulse::Execute()
{
    Value objectValue = EvaluateInput(inputPins[1].get());
    Value impulseValue = EvaluateInput(inputPins[2].get());

    if (std::holds_alternative<IObject*>(objectValue) && std::holds_alternative<glm::vec3>(impulseValue)) {
        IObject* object = std::get<IObject*>(objectValue);
        glm::vec3 impulse = std::get<glm::vec3>(impulseValue);

        object->AddImpulse(impulse);
    }

    Node* next = GetNextExecNode(outputPins[0].get());
    if (next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void AddImpulse::SetPins()
{
    inputPins = {
           std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, this),
           std::make_shared<Pin>("Object", PinType::Object, PinDirection::Input, this),
           std::make_shared<Pin>("ImpulseVector3", PinType::Vector3, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, this),
    };
}
