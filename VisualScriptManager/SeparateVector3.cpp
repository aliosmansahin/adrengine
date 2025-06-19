#include "pch.h"
#include "SeparateVector3.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value SeparateVector3::Evaluate(Pin* pin)
{
    //Get input
    Value input = EvaluateInput(inputPins[0].get());

    //Check if it is a glm::vec3
    if (!std::holds_alternative<glm::vec3>(input))
        return 0.0f;
    
    //Get glm::vec3
    glm::vec3 vector = std::get<glm::vec3>(input);

    //Return value for the pin that is requested
    if (pin->name == "X")
        return vector.x;
    if (pin->name == "Y")
        return vector.y;
    if (pin->name == "Z")
        return vector.z;

    return 0.0f;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void SeparateVector3::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("Vector", PinType::Vector3, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("X", PinType::Float, PinDirection::Output, this),
        std::make_shared<Pin>("Y", PinType::Float, PinDirection::Output, this),
        std::make_shared<Pin>("Z", PinType::Float, PinDirection::Output, this),
    };
}
