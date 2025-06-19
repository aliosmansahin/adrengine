#include "pch.h"
#include "GetThisEntity.h"

/*
PURPOSE: Draws GetThisEntity node
*/
VISUALSCRIPTMANAGER_API void GetThisEntity::Draw(NodeVisual* nodeVisual)
{
    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->name.c_str());
        ImNodes::EndOutputAttribute();
    }
}

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetThisEntity::Evaluate(Pin* pin)
{
    return entity;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void GetThisEntity::SetPins()
{
    outputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Output, this),
    };
}