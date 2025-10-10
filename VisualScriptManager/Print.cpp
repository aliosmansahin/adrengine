#include "pch.h"
#include "Print.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void Print::Execute()
{
    Value text = EvaluateInput(inputPins[1]);
    if (std::holds_alternative<std::string>(text)) {
        std::cout << std::get<std::string>(text) << std::endl;
    }

    std::shared_ptr<INode> next = GetNextExecNode(outputPins[0]);
    if(next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
void Print::SetPins()
{
	std::shared_ptr<INode> thisNode = shared_from_this();

    inputPins = {
           std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, thisNode),
           std::make_shared<Pin>("Text", PinType::String, PinDirection::Input, thisNode),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, thisNode),
    };
}