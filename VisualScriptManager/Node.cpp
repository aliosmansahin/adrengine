#include "pch.h"
#include "Node.h"

/*
PURPOSE: Gets InExec pin of the next node
*/
VISUALSCRIPTMANAGER_API	Node* Node::GetNextExecNode(Pin* execOutputPin) {
	if (execOutputPin->connectedTo)
		return execOutputPin->connectedTo->parentNode;
	return nullptr;
}

/*
PURPOSE: Begin the node
*/
VISUALSCRIPTMANAGER_API void Node::BeginDraw(NodeVisual* nodeVisual)
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
    ImGui::Text(GetType().c_str());
    ImNodes::EndNodeTitleBar();
}

/*
PURPOSE: Draws print node
*/
VISUALSCRIPTMANAGER_API void Node::Draw(NodeVisual* nodeVisual)
{
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
}

/*
PURPOSE: End the node
*/
VISUALSCRIPTMANAGER_API void Node::EndDraw()
{
    ImNodes::EndNode();
}

/*
PURPOSE: Sets the position of the node
*/
VISUALSCRIPTMANAGER_API void Node::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
}

/*
PURPOSE: Runs this node
*/
VISUALSCRIPTMANAGER_API void Node::Execute()
{
}

/*
PURPOSE: Base Evaluate function,
	Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value Node::Evaluate(Pin* pin)
{
	return std::monostate{};
}

/*
PURPOSE: Gets value from input pin
*/
VISUALSCRIPTMANAGER_API Value Node::EvaluateInput(Pin* pin)
{
    if (!pin || !pin->connectedTo) return std::monostate{};

    Node* source = pin->connectedTo->parentNode;
    return source->Evaluate(pin->connectedTo);
}

/*
PURPOSE: Creates a json from the node
*/
VISUALSCRIPTMANAGER_API nlohmann::json Node::ToJson()
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
VISUALSCRIPTMANAGER_API bool Node::FromJson(nlohmann::json json)
{
    x = json.value("x", 0);
    y = json.value("y", 0);

    SetPins();

    return true;
}
