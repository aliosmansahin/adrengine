#include "pch.h"
#include "ConstFloat.h"

/*
PURPOSE: Draws begin node
*/
VISUALSCRIPTMANAGER_API void ConstFloat::Draw(NodeVisual* nodeVisual)
{
    ImGui::SetNextItemWidth(100);
    ImGui::InputFloat("Value", &buf, 1.0f, 5.0f);

    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->name.c_str());
        ImNodes::EndOutputAttribute();
    }
}

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value ConstFloat::Evaluate(Pin* pin)
{
    return buf;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void ConstFloat::SetPins()
{
    outputPins = {
        std::make_shared<Pin>("Value", PinType::Float, PinDirection::Output, this),
    };
}

/*
PURPOSE: Creates a json from the node
*/
VISUALSCRIPTMANAGER_API nlohmann::json ConstFloat::ToJson()
{
    nlohmann::json j = Node::ToJson();

    j["value"] = buf;
    return j;
}

/*
PURPOSE: Sets a node from its json
*/
VISUALSCRIPTMANAGER_API bool ConstFloat::FromJson(nlohmann::json json)
{
    Node::FromJson(json);

    buf = json.value("value", 0.0f);

    return true;
}
