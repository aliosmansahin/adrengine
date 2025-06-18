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
	return nlohmann::json();
}

/*
PURPOSE: Sets a node from its json
*/
bool ConstString::FromJson(nlohmann::json json)
{
	return false;
}
