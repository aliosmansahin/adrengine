#include "pch.h"
#include "Return.h"

/*
PURPOSE: Draws return node
*/
void Return::Draw()
{
    //In first frame, set the node position
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2((float)x, (float)y));
        first = false;
    }

    //Draw the node
    ImNodes::BeginNode(id);
    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Return");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(id + 2, ImNodesPinShape_TriangleFilled);
    ImGui::Text("");
    ImNodes::EndInputAttribute();

    ImNodes::EndNode();
}

/*
PURPOSE: Sets the position of the node
*/
void Return::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

/*
PURPOSE: This function is called from the visual script manager to compile the node.
    Returned function will be stored in a compilation vector
    Return node will do nothing
*/
std::function<void()> Return::Compile()
{
    return std::function<void()>();
}

/*
PURPOSE: Return node has no input or output,
    so this function doesn't have to be called
*/
std::function<void()> Return::Call(Input* input)
{
    return std::function<void()>();
}

/*
PURPOSE: Creates a json from the node
*/
nlohmann::json Return::ToJson()
{
    nlohmann::json j;
    j["id"] = id;
    j["x"] = x;
    j["y"] = y;
    j["type"] = GetType();
    return j;
}

/*
PURPOSE: Sets a node from its json
*/
bool Return::FromJson(nlohmann::json json)
{
    int id = json.value("id", -1);
    if (id == -1)
        return false;

    this->id = id;
    x = json.value("x", 0);
    y = json.value("y", 0);
    return true;
}
