#include "pch.h"
#include "Print.h"

/*
PURPOSE: Draws print node
*/
VISUALSCRIPTMANAGER_API void Print::Draw(NodeVisual* nodeVisual)
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
VISUALSCRIPTMANAGER_API void Print::Execute()
{
    Value text = EvaluateInput(inputPins[1].get());
    if (std::holds_alternative<std::string>(text)) {
        std::cout << std::get<std::string>(text) << std::endl;
    }

    Node* next = GetNextExecNode(outputPins[0].get());
    if(next) next->Execute();
}

/*
PURPOSE: Sets pins for this node
*/
void Print::SetPins()
{
    inputPins = {
           std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, this),
           std::make_shared<Pin>("Text", PinType::String, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, this),
    };
}