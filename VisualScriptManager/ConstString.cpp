#include "pch.h"
#include "ConstString.h"

/*
PURPOSE: Draws begin node
*/
void ConstString::Draw(std::shared_ptr<NodeVisual> nodeVisual)
{
    ImGui::SetNextItemWidth(100);
    ImGui::InputText("Text", buf, sizeof(buf));

    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->GetType() == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->GetName().c_str());
        ImNodes::EndOutputAttribute();
    }
}

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
Value ConstString::Evaluate(std::shared_ptr<IPin> pin)
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
	std::shared_ptr<INode> thisNode = shared_from_this();

    outputPins = {
        std::make_shared<Pin>("Value", PinType::String, PinDirection::Output, thisNode),
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
