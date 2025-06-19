#include "pch.h"
#include "GetPositionVector.h"

/*
PURPOSE: Draws GetPositionVector node
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::Draw(NodeVisual* nodeVisual)
{
    for (size_t i = 0; i < inputPins.size(); ++i) {
        ImNodes::BeginInputAttribute(nodeVisual->inputIds[i], inputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(inputPins[i]->name.c_str());
        ImNodes::EndInputAttribute();
    }

    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->name.c_str());
        ImNodes::EndOutputAttribute();
    }
}

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetPositionVector::Evaluate(Pin* pin)
{
    Value entityValue = EvaluateInput(inputPins[0].get());

    if (!std::holds_alternative<Entity*>(entityValue)) {
        return std::monostate{};
    }

    Entity* entity = std::get<Entity*>(entityValue);

    glm::vec3 vector = glm::vec3(entity->GetEntityParams()->x, entity->GetEntityParams()->y, entity->GetEntityParams()->z);

    return vector;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("PositionVector", PinType::Vector3, PinDirection::Output, this),
    };
}