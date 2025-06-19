#include "pch.h"
#include "ConstString.h"

/*
PURPOSE: Constructor
*/
ConstString::ConstString()
{
	title = "Const String";
}

/*
PURPOSE: Draws begin node
*/
void ConstString::Draw(NodeVisual* nodeVisual)
{
    //In the first frame, set the node position
    if (first) {
        ImNodes::SetNodeScreenSpacePos(nodeVisual->id, ImVec2((float)x, (float)y));
        first = false;
    }

    //Draw the node
    ImNodes::BeginNode(nodeVisual->id);

    //Save current position of the node
    ImVec2 pos = ImNodes::GetNodeScreenSpacePos(nodeVisual->id);
    x = pos.x;
    y = pos.y;

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(title.c_str());
    ImNodes::EndNodeTitleBar();

    ImGui::SetNextItemWidth(100);
    ImGui::InputText("Text", buf, sizeof(buf));

    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->name.c_str());
        ImNodes::EndOutputAttribute();
    }

    ImNodes::EndNode();
}

/*
PURPOSE: This function doesn't do anything
*/
VISUALSCRIPTMANAGER_API void ConstString::Execute()
{
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
PURPOSE: This function doesn't do anything
*/
Value ConstString::EvaluateInput(Pin* pin)
{
    return Value();
}

/*
PURPOSE: Sets the position of the node
*/
void ConstString::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
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
    nlohmann::json j;
    j["x"] = x;
    j["y"] = y;
    j["type"] = GetType();

    j["value"] = buf;
    return j;
}

/*
PURPOSE: Sets a node from its json
*/
bool ConstString::FromJson(nlohmann::json json)
{
    x = json.value("x", 0);
    y = json.value("y", 0);

    std::string value = json.value("value", "");

    if(value.size() + 1 <= sizeof(buf))
        strcpy_s(buf, sizeof(buf), value.c_str());

    SetPins();

    first = true;

    return true;
}
