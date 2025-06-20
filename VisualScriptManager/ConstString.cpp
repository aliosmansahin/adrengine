#include "pch.h"
#include "ConstString.h"

/*
PURPOSE: Draws begin node
*/
void ConstString::Draw(NodeVisual* nodeVisual)
{
    ImGui::SetNextItemWidth(100);
    ImGui::InputText("Text", buf, sizeof(buf));

    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->name.c_str());
        ImNodes::EndOutputAttribute();
    }
}

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
Value ConstString::Evaluate(Pin* pin)
{
    buf[sizeof(buf) - 1] = '\0';
    std::string value = buf;
    return value;
}

/*
PURPOSE: Sets pins for this node
*/
void ConstString::SetPins()
{
    outputPins = {
        std::make_shared<Pin>("Value", PinType::String, PinDirection::Output, this),
    };
}

/*
PURPOSE: Creates a json from the node
*/
nlohmann::json ConstString::ToJson()
{
    nlohmann::json j = Node::ToJson();
    
    j["value"] = buf;
    return j;
}

/*
PURPOSE: Sets a node from its json
*/
bool ConstString::FromJson(nlohmann::json json)
{
    Node::FromJson(json);

    std::string value = json.value("value", "");

    if(value.size() + 1 <= sizeof(buf))
        strcpy_s(buf, sizeof(buf), value.c_str());

    return true;
}
