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
