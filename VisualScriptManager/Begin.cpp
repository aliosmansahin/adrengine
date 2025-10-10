#include "pch.h"
#include "Begin.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void Begin::Execute()
{
    std::shared_ptr<INode> next = GetNextExecNode(outputPins[0]);
    if(next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
void Begin::SetPins()
{
	std::shared_ptr<INode> thisNode = shared_from_this();

    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, thisNode),
    };
}