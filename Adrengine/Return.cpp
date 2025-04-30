#include "Return.h"

void Return::Draw()
{
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2((float)x, (float)y));
        first = false;
    }
    ImNodes::BeginNode(id);
    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Return");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(id + 2, ImNodesPinShape_TriangleFilled);
    ImGui::Text("");
    ImNodes::EndInputAttribute();

    ImNodes::EndNode();
}

void Return::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

std::function<void()> Return::Compile()
{
    return std::function<void()>();
}

std::function<void()> Return::Call(Input* input)
{
    return std::function<void()>();
}

nlohmann::json Return::ToJson()
{
    nlohmann::json j;
    j["id"] = id;
    j["x"] = x;
    j["y"] = y;
    j["type"] = GetType();
    return j;
}

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
