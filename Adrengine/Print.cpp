#include "Print.h"

void Print::Draw()
{
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2((float)x, (float)y));
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

nlohmann::json Print::ToJson()
{
    nlohmann::json j;
    j["id"] = id;
    j["x"] = x;
    j["y"] = y;
    j["type"] = GetType();
    j["text"] = std::string(buf);
    return j;
}

bool Print::FromJson(nlohmann::json json)
{
    int id = json.value("id", -1);
    if (id == -1)
        return false;

    this->id = id;
    x = json.value("x", 0);
    y = json.value("y", 0);
    std::string text = json.value("text", "");
    strncpy_s(buf, sizeof(buf), text.c_str(), _TRUNCATE);
    buf[31] = '\0';

    return true;
}
