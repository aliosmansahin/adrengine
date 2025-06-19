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
    if (!std::holds_alternative<std::any>(input))
        return 0.0f;

    //Get glm::any
    std::any any = std::get<glm::vec3>(input);

    const std::type_info& ti = any.type();

    //Check if the value can be converted
    if(ti == typeid(int))
        return ArithmeticToString(std::any_cast<int>(any));

    if (ti == typeid(float))
        return ArithmeticToString(std::any_cast<float>(any));

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
