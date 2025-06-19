#include "pch.h"
#include "Print.h"

/*
PURPOSE: Constructor
*/
Print::Print()
{
    title = "Print";
}

/*
PURPOSE: Draws print node
*/
VISUALSCRIPTMANAGER_API void Print::Draw(NodeVisual* nodeVisual)
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

    for (size_t i = 0; i < inputPins.size(); ++i) {
        ImNodes::BeginInputAttribute(nodeVisual->inputIds[i], inputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(inputPins[i]->name.c_str());
        ImNodes::EndInputAttribute();
    }

    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->type == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->name.c_str());
        ImNodes::EndOutputAttribute();
    }

    ImNodes::EndNode();
}

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void Print::Execute()
{
    Value text = EvaluateInput(inputPins[1].get());
    if (std::holds_alternative<std::string>(text)) {
        std::cout << std::get<std::string>(text) << std::endl;
    }

    Node* next = GetNextExecNode(outputPins[0].get());
    if(next) next->Execute();
}

/*
PURPOSE: Gets value from input pin
*/
Value Print::EvaluateInput(Pin* pin)
{
    if (!pin || !pin->connectedTo) return std::monostate{};

    Node* source = pin->connectedTo->parentNode;
    return source->Evaluate(pin->connectedTo);
}

/*
PURPOSE: Sets the position of the node
*/
VISUALSCRIPTMANAGER_API void Print::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

/*
PURPOSE: Sets pins for this node
*/
void Print::SetPins()
{
    inputPins = {
           std::make_shared<Pin>("InExec", PinType::Exec, PinDirection::Input, this),
           std::make_shared<Pin>("Text", PinType::String, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("OutExec", PinType::Exec, PinDirection::Output, this),
    };
}

/*
PURPOSE: Creates a json from the node
*/
VISUALSCRIPTMANAGER_API nlohmann::json Print::ToJson()
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
VISUALSCRIPTMANAGER_API bool Print::FromJson(nlohmann::json json)
{
    x = json.value("x", 0);
    y = json.value("y", 0);

    SetPins();

    return true;
}
