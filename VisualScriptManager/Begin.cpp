#include "pch.h"
#include "Begin.h"

/*
PURPOSE: Draws begin node
*/
void Begin::Draw(NodeVisual* nodeVisual)
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