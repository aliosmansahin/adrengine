#include "pch.h"
#include "SeparateVector3.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value SeparateVector3::Evaluate(std::shared_ptr<IPin> pin)
{
    //Get input
    Value input = EvaluateInput(inputPins[0]);

    //Check if it is a glm::vec3
    if (!std::holds_alternative<glm::vec3>(input))
        return 0.0f;
    
    //Get glm::vec3
    glm::vec3 vector = std::get<glm::vec3>(input);

    //Return value for the pin that is requested
    if (pin->GetName() == "X")
        return vector.x;
    if (pin->GetName() == "Y")
        return vector.y;
    if (pin->GetName() == "Z")
        return vector.z;

    return 0.0f;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void SeparateVector3::SetPins()
{
	std::shared_ptr<INode> thisNode = shared_from_this();

    inputPins = {
        std::make_shared<Pin>("Vector", PinType::Vector3, PinDirection::Input, thisNode),
    };
    outputPins = {
        std::make_shared<Pin>("X", PinType::Float, PinDirection::Output, thisNode),
        std::make_shared<Pin>("Y", PinType::Float, PinDirection::Output, thisNode),
        std::make_shared<Pin>("Z", PinType::Float, PinDirection::Output, thisNode),
    };
}
