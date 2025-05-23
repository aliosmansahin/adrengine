#include "pch.h"
#include "Print.h"

/*
PURPOSE: Draws print node
*/
void Print::Draw()
{
    //In first frame, set the node position
    if (first) {
        ImNodes::SetNodeScreenSpacePos(id, ImVec2((float)x, (float)y));
        first = false;
    }

    //Draw the node
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

    //Text input
    ImNodes::BeginInputAttribute(id + 3, ImNodesPinShape_CircleFilled);
    ImGui::SetNextItemWidth(100);
    ImGui::InputText("Text", buf, sizeof(buf));
    ImNodes::EndInputAttribute();

    ImNodes::EndNode();

}

/*
PURPOSE: Sets the position of the node
*/
void Print::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

/*
PURPOSE: This function is called from the visual script manager to compile the node.
    Returned function will be stored in a compilation vector
*/
std::function<void()> Print::Compile()
{
    //Create an input for this node and create a function to execute when user plays the scene
    PrintInput* input = new PrintInput();
    buf[sizeof(buf) - 1] = '\0';
    input->text = buf;
    return Call(input);
}

/*
PURPOSE: Creates a function to execute when user plays the scene,
    this function will be stored in "compiled" vector
*/
std::function<void()> Print::Call(Input* input)
{
    //This node prints a text
	PrintInput* printInput = dynamic_cast<PrintInput*>(input);
	std::function<void()> func = [printInput]() {
		std::cout << printInput->text << std::endl;
		};

    delete printInput;
	return func;
}

/*
PURPOSE: Creates a json from the node
*/
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

/*
PURPOSE: Sets a node from its json
*/
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
