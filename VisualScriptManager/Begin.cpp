#include "pch.h"
#include "Begin.h"

/*
PORPOSE: Constructor
*/
Begin::Begin()
{
    title = "Begin Entrypoint";
}

/*
PURPOSE: Draws begin node
*/
void Begin::Draw(NodeVisual* nodeVisual)
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

    ImNodes::EndNode();
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
PURPOSE: Sets the position of the node
*/
void Begin::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
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

/*
PURPOSE: Creates a json from the node
*/
nlohmann::json Begin::ToJson()
{
    nlohmann::json j;
    j["x"] = x;
    j["y"] = y;
    j["type"] = GetType();
    return j;
}

/*
PURPOSE: Sets a node from its json
*/
bool Begin::FromJson(nlohmann::json json)
{
    x = json.value("x", 0);
    y = json.value("y", 0);

    SetPins();

    return true;
}
