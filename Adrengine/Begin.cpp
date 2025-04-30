#include "Begin.h"

void Begin::Draw()
{
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2((float)x, (float)y));
        first = false;
    }
    ImNodes::BeginNode(id);
    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Begin Entrypoint");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginOutputAttribute(id + 1, ImNodesPinShape_TriangleFilled);
    ImGui::Text("");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
}

void Begin::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

std::function<void()> Begin::Compile()
{
	return std::function<void()>();
}

std::function<void()> Begin::Call(Input* input)
{
	return std::function<void()>();
}

nlohmann::json Begin::ToJson()
{
    nlohmann::json j;
    j["id"] = id;
    j["x"] = x;
    j["y"] = y;
    j["type"] = GetType();
    return j;
}

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
