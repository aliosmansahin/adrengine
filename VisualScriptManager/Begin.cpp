#include "pch.h"
#include "Begin.h"

/*
PURPOSE: Draws begin node
*/
void Begin::Draw()
{
    //In first frame, set the node position
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2((float)x, (float)y));
        first = false;
    }

    //Draw the node
    ImNodes::BeginNode(id);
    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Begin Entrypoint");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginOutputAttribute(id + 1, ImNodesPinShape_TriangleFilled);
    ImGui::Text("");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
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
PURPOSE: This function is called from the visual script manager to compile the node.
    Returned function will be stored in a compilation vector
    Begin node will do nothing
*/
std::function<void()> Begin::Compile()
{
	return std::function<void()>();
}

/*
PURPOSE: Begin node has no input or output,
    so this function doesn't have to be called
*/
std::function<void()> Begin::Call(Input* input)
{
	return std::function<void()>();
}

/*
PURPOSE: Creates a json from the node
*/
nlohmann::json Begin::ToJson()
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
bool Begin::FromJson(nlohmann::json json)
{
    int id = json.value("id", -1);
    if (id == -1)
        return false;

    this->id = id;
    x = json.value("x", 0);
    y = json.value("y", 0);
    return true;
}
