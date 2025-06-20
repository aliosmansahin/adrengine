#include "pch.h"
#include "Print.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void Print::Execute()
{
    Value text = EvaluateInput(inputPins[1].get());
    if (std::holds_alternative<std::string>(text)) {
        std::cout << std::get<std::string>(text) << std::endl;
    }

    Node* next = GetNextExecNode(outputPins[0].get());
    if(next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
void Print::SetPins()
{
    inputPins = {
           std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, this),
           std::make_shared<Pin>("Text", PinType::String, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, this),
    };
}