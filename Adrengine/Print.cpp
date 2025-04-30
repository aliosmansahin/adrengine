#include "Print.h"

void Print::Draw()
{
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2(x, y));
        first = false;
    }
    ImNodes::BeginNode(id);
    
    ImNodes::BeginNodeTitleBar();
    ImGui::Text("Print");
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(id + 2, ImNodesPinShape_TriangleFilled);
    ImGui::Text("");
    ImNodes::EndInputAttribute();
    ImGui::SameLine();

    ImNodes::BeginOutputAttribute(id + 1, ImNodesPinShape_TriangleFilled);
    ImGui::Text("");
    ImNodes::EndOutputAttribute();

    ImNodes::BeginInputAttribute(id + 3, ImNodesPinShape_CircleFilled);
    ImGui::SetNextItemWidth(100);
    ImGui::InputText("Text", buf, sizeof(buf));
    ImNodes::EndInputAttribute();

    ImNodes::EndNode();

}

void Print::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

std::function<void()> Print::Compile()
{
    PrintInput* input = new PrintInput();
    buf[sizeof(buf) - 1] = '\0';
    input->text = buf;
    return Call(input);
}

std::function<void()> Print::Call(Input* input)
{
	PrintInput* printInput = dynamic_cast<PrintInput*>(input);
	std::function<void()> func = [printInput]() {
		std::cout << printInput->text << std::endl;
		};
	return func;
}
