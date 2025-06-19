#include "pch.h"
#include "GetPositionVector.h"

/*
PURPOSE: Constructor
*/
GetPositionVector::GetPositionVector()
{
	title = "Get Position Vector";
}

/*
PURPOSE: Draws GetPositionVector node
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::Draw(NodeVisual* nodeVisual)
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
PURPOSE: This function doesn't do anything
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::Execute()
{
}

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetPositionVector::Evaluate(Pin* pin)
{
    Value entityValue = EvaluateInput(inputPins[0].get());

    if (!std::holds_alternative<Entity*>(entityValue)) {
        return std::monostate{};
    }

    Entity* entity = std::get<Entity*>(entityValue);

    glm::vec3 vector = glm::vec3(entity->GetEntityParams()->x, entity->GetEntityParams()->y, entity->GetEntityParams()->z);

    return vector;
}

/*
PURPOSE: Gets value from input pin
*/
Value GetPositionVector::EvaluateInput(Pin* pin)
{
    if (!pin || !pin->connectedTo) return std::monostate{};

    Node* source = pin->connectedTo->parentNode;
    return source->Evaluate(pin->connectedTo);
}

/*
PURPOSE: Sets the position of the node
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::SetPos(int x, int y)
{
    this->x = x;
    this->y = y;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void GetPositionVector::SetPins()
{
    inputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Input, this),
    };
    outputPins = {
        std::make_shared<Pin>("PositionVector", PinType::Vector3, PinDirection::Output, this),
    };
}

/*
PURPOSE: Creates a json from the node
*/
VISUALSCRIPTMANAGER_API nlohmann::json GetPositionVector::ToJson()
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
VISUALSCRIPTMANAGER_API bool GetPositionVector::FromJson(nlohmann::json json)
{
    x = json.value("x", 0);
    y = json.value("y", 0);

    SetPins();

    return true;
}
