#include "pch.h"
#include "MakeVector3.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value MakeVector3::Evaluate(Pin* pin)
{
    Value xValue = EvaluateInput(inputPins[0].get());
    Value yValue = EvaluateInput(inputPins[1].get());
    Value zValue = EvaluateInput(inputPins[2].get());

    if (!std::holds_alternative<float>(xValue)) {
        return std::monostate{};
    }
    if (!std::holds_alternative<float>(yValue)) {
        return std::monostate{};
    }
    if (!std::holds_alternative<float>(zValue)) {
        return std::monostate{};
    }

    float x = std::get<float>(xValue);
    float y = std::get<float>(yValue);
    float z = std::get<float>(zValue);

    glm::vec3 vector = glm::vec3(x, y, z);

    return vector;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void MakeVector3::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("X", PinType::Float, PinDirection::Input, this),
        std::make_shared<Pin>("Y", PinType::Float, PinDirection::Input, this),
        std::make_shared<Pin>("Z", PinType::Float, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("Vector3", PinType::Vector3, PinDirection::Output, this),
    };
}
