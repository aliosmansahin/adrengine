#include "Begin.h"

void Begin::Draw()
{
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2(x, y));
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
