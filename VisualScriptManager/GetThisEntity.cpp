#include "pch.h"
#include "GetThisEntity.h"

/*
PURPOSE: Constructor
*/
GetThisEntity::GetThisEntity()
{
    title = "Get This Entity";
}

/*
PURPOSE: Draws GetThisEntity node
*/
VISUALSCRIPTMANAGER_API void GetThisEntity::Draw(NodeVisual* nodeVisual)
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
    x = (int)pos.x;
    y = (int)pos.y;

    ImNodes::BeginNodeTitleBar();
    ImGui::Text(title.c_str());
    ImNodes::EndNodeTitleBar();

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
VISUALSCRIPTMANAGER_API void GetThisEntity::Execute()
{
}

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetThisEntity::Evaluate(Pin* pin)
{
    return entity;
}

/*
PURPOSE: Sets the position of the node
*/
VISUALSCRIPTMANAGER_API void GetThisEntity::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
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

/*
PURPOSE: Creates a json from the node
*/
VISUALSCRIPTMANAGER_API nlohmann::json GetThisEntity::ToJson()
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
VISUALSCRIPTMANAGER_API bool GetThisEntity::FromJson(nlohmann::json json)
{
    x = json.value("x", 0);
    y = json.value("y", 0);

    SetPins();

    return true;
}
