#include "pch.h"
#include "VisualScript.h"

/*
PURPOSE: Creates a new script and set some variables
*/
bool VisualScript::CreateScript(std::string scriptId, std::string belongsTo, std::string belongsScene)
{
	this->scriptId = scriptId;
	this->belongsTo = belongsTo;
	this->belongsScene = belongsScene;
	return true;
}

/*
PURPOSE: Draws the script
TODO: This might be unnecessary,
	so might be removed
*/
void VisualScript::DrawScript()
{
}

/*
PURPOSE: Releases the script
*/
void VisualScript::ReleaseScript()
{
	//The variables is already being deleted
}

/*
PURPOSE: Executes begin node of this script
*/
void VisualScript::ExecuteBeginScript()
{
	auto beginNode = nodes.find(0); //0 for Begin Entrypoint
	if (beginNode == nodes.end()) {
		Logger::Log("W", "Begin Node not found!");
		return;
	}
	beginNode->second.logicNode->Execute();
}

/*
PURPOSE: Creates a json content of this visual script
*/
nlohmann::json VisualScript::ToJson()
{
	nlohmann::json j;

	//Script properties
	j["id"] = scriptId;
	j["belongs-to"] = belongsTo;
	j["belongs-scene"] = belongsScene;

	//All nodes
	for (auto& node : nodes) {
		nlohmann::json nodeJson = node.second.logicNode->ToJson();
		nodeJson["id"] = node.second.id;
		j["nodes"].push_back(nodeJson);
	}

	//All links
	for (auto& link : links) {
		nlohmann::json l;
		l["first"] = link.second.first;
		l["second"] = link.second.second;
		l["id"] = link.first;
		j["links"].push_back(l);
	}

	j["next-id"] = nextId;

	return j;
}

/*
PURPOSE: Sets the visual script from its json
*/
void VisualScript::FromJson(nlohmann::json json, std::unordered_map<std::string, std::shared_ptr<Node>>& types)
{
	//Script properties
	scriptId = json["id"];
	belongsTo = json["belongs-to"];
	belongsScene = json["belongs-scene"];

	//Load each node from its json and insert it to "nodes"
	for (auto& nodeJson : json["nodes"]) {
		std::string nodeType = nodeJson.value("type", "");
		if (nodeType == "")
			continue;
		auto typeIter = types.find(nodeType);
		if (typeIter == types.end())
			continue;
		int nodeId = nodeJson.value("id", 100000);
		auto type = typeIter->second->clone();

		type->FromJson(nodeJson);

		if (nodeType == "GetThisEntity") {
			GetThisEntity* entityNode = dynamic_cast<GetThisEntity*>(type.get());

			if (entityNode) {
				Scene* scene = SceneManager::GetInstance().GetSceneById(belongsScene).get();
				if (scene) {
					entityNode->entity = scene->GetEntityManager()->GetEntityById(belongsTo).get();
				}
			}
		}

		//Add a new node
		NodeVisual nodeVisual;
		nodeVisual.logicNode = type;

		nodeVisual.id = nodeId++;

		//Set pin ids
		for (auto& pin : type->inputPins) {
			pin->id = nodeId++;

			nodeVisual.inputIds.push_back(pin->id);
		}

		for (auto& pin : type->outputPins) {
			pin->id = nodeId++;

			nodeVisual.outputIds.push_back(pin->id);
		}

		nodes.insert({ nodeVisual.id, nodeVisual });
	}

	//Load each link, insert it to "links" and set pins
	for (auto& link : json["links"]) {
		int first = link.value("first", -1);
		int second = link.value("second", -1);
		int id = link.value("id", -1);
		if (first != -1 && second != -1 && id != -1) {
			Pin* from = FindPinById(first);
			Pin* to = FindPinById(second);

			if (from && to && from->type == to->type) {
				to->connectedTo = from;
				from->connectedTo = to;
				links.insert({ nextId++, {from->id, to->id} });
			}
		}
	}

	//Load next id
	nextId = json.value("next-id", 1000);
}

/*
PURPOSE: Gets pin from id
*/
Pin* VisualScript::FindPinById(int id)
{
	for (const auto& iter : nodes) {
		const NodeVisual* vis = &iter.second;

		//Check for input pins
		for (auto& pin : vis->logicNode->inputPins) {
			if (pin->id == id)
				return pin.get();
		}

		//Check for output pins
		for (auto& pin : vis->logicNode->outputPins) {
			if (pin->id == id)
				return pin.get();
		}
	}
	return nullptr;
}
