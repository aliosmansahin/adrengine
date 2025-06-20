#include "pch.h"
#include "ToString.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value ToString::Evaluate(Pin* pin)
{
    //Get input
    Value input = EvaluateInput(inputPins[0].get());

    //Check if it is an entity
    if (std::holds_alternative<float>(input))
        return ArithmeticToString(std::get<float>(input));
    if (std::holds_alternative<int>(input))
        return ArithmeticToString(std::get<int>(input));

    return "";
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void ToString::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("Value", PinType::Any, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("String", PinType::String, PinDirection::Output, this),
    };
}
