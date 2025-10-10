#include "pch.h"
#include "Node.h"

/*
PURPOSE: Gets InExec pin of the next node
*/
VISUALSCRIPTMANAGER_API	std::shared_ptr<INode> Node::GetNextExecNode(std::shared_ptr<IPin> execOutputPin) {
	if (execOutputPin->GetConnectedPin())
		return execOutputPin->GetConnectedPin()->GetParentNode();
	return nullptr;
}

/*
PURPOSE: Begin the node
*/
VISUALSCRIPTMANAGER_API void Node::BeginDraw(std::shared_ptr<NodeVisual> nodeVisual)
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
VISUALSCRIPTMANAGER_API void Node::Draw(std::shared_ptr<NodeVisual> nodeVisual)
{
    for (size_t i = 0; i < inputPins.size(); ++i) {
        ImNodes::BeginInputAttribute(nodeVisual->inputIds[i], inputPins[i]->GetType() == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(inputPins[i]->GetName().c_str());
        ImNodes::EndInputAttribute();
    }

    for (size_t i = 0; i < outputPins.size(); ++i) {
        ImNodes::BeginOutputAttribute(nodeVisual->outputIds[i], outputPins[i]->GetType() == PinType::Exec ? ImNodesPinShape_TriangleFilled : ImNodesPinShape_CircleFilled);
        ImGui::Text(outputPins[i]->GetName().c_str());
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
VISUALSCRIPTMANAGER_API Value Node::Evaluate(std::shared_ptr<IPin> pin)
{
	return std::monostate{};
}

/*
PURPOSE: Gets value from input pin
*/
VISUALSCRIPTMANAGER_API Value Node::EvaluateInput(std::shared_ptr<IPin> pin)
{
    if (!pin || !pin->GetConnectedPin()) return std::monostate{};

    std::shared_ptr<Node> source = std::dynamic_pointer_cast<Node>(pin->GetConnectedPin()->GetParentNode());
    return source->Evaluate(pin->GetConnectedPin());
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
