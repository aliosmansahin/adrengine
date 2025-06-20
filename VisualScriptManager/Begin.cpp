#include "pch.h"
#include "Begin.h"

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void Begin::Execute()
{
    Node* next = GetNextExecNode(outputPins[0].get());
    if(next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
void Begin::SetPins()
{
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, this),
    };
}