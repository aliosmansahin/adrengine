#include "pch.h"
#include "VisualScript.h"

/*
PURPOSE: Creates a new script and set some variables
*/
bool VisualScript::CreateScript(std::string scriptId, std::string belongsEntity, std::string belongsScene)
{
	this->scriptId = scriptId;
	this->belongsScene = belongsScene;
	this->belongsEntity = belongsEntity;

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
	nodes.clear();
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
	beginNode->second->logicNode->Execute();
}

/*
PURPOSE: Creates a json content of this visual script
*/
nlohmann::json VisualScript::ToJson()
{
	nlohmann::json j;

	//Script properties
	j["id"] = scriptId;
	j["belongs-entity"] = belongsEntity;
	j["belongs-scene"] = belongsScene;

	//All nodes
	for (auto& node : nodes) {
		nlohmann::json nodeJson = node.second->logicNode->ToJson();
		nodeJson["id"] = node.second->id;
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
void VisualScript::FromJson(nlohmann::json json, std::unordered_map<std::string, std::shared_ptr<INode>>& types, std::shared_ptr<IScene> scene)
{
	//Script properties
	scriptId = json.value("id", "");
	belongsScene = json.value("belongs-scene", "");
	belongsEntity = json.value("belongs-entity", "");

	//Load next id
	nextId = json.value("next-id", 1000);

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
			std::shared_ptr<GetThisEntity> entityNode = std::dynamic_pointer_cast<GetThisEntity>(type);

			if (entityNode) {
				if (scene) {
					entityNode->SetEntity(scene->GetEntityManager()->GetEntityById(belongsEntity));
				}
			}
		}

		//Add a new node
		std::shared_ptr<NodeVisual> nodeVisual = std::make_shared<NodeVisual>();
		nodeVisual->logicNode = type;

		nodeVisual->id = nodeId++;

		//Set pin ids
		for (auto& pin : type->GetInputPins()) {
			pin->SetId(nodeId++);

			nodeVisual->inputIds.push_back(pin->GetId());
		}

		for (auto& pin : type->GetOutputPins()) {
			pin->SetId(nodeId++);

			nodeVisual->outputIds.push_back(pin->GetId());
		}

		nodes.insert({nodeVisual->id, nodeVisual});
	}

	//Load each link, insert it to "links" and set pins
	for (auto& link : json["links"]) {
		int first = link.value("first", -1);
		int second = link.value("second", -1);
		int id = link.value("id", -1);
		if (first != -1 && second != -1 && id != -1) {
			std::shared_ptr<IPin> from = FindPinById(first);
			std::shared_ptr<IPin> to = FindPinById(second);

			if (from && to && (from->GetType() == to->GetType() || from->GetType() == PinType::Any || to->GetType() == PinType::Any)) {
				to->SetConnectedPin(from);
				from->SetConnectedPin(to);
				links.insert({ nextId++, {from->GetId(), to->GetId()}});
			}
		}
	}
}

/*
PURPOSE: Gets pin from id
*/
std::shared_ptr<IPin> VisualScript::FindPinById(int id)
{
	for (const auto& iter : nodes) {
		const std::shared_ptr<NodeVisual> vis = iter.second;

		//Check for input pins
		for (auto& pin : vis->logicNode->GetInputPins()) {
			if (pin->GetId() == id)
				return pin;
		}

		//Check for output pins
		for (auto& pin : vis->logicNode->GetOutputPins()) {
			if (pin->GetId() == id)
				return pin;
		}
	}
	return nullptr;
}

/*
PURPOSE: Returns script id
*/
VISUALSCRIPTMANAGER_API std::string VisualScript::GetScriptId()
{
	return scriptId;
}

/*
PURPOSE: Returns belongs entity
*/
VISUALSCRIPTMANAGER_API std::string VisualScript::GetBelongsEntity()
{
	return belongsEntity;
}

/*
PURPOSE: Returns next id
*/
VISUALSCRIPTMANAGER_API int VisualScript::GetNextId()
{
	return nextId;
}

/*
PURPOSE: Returns all nodes
*/
VISUALSCRIPTMANAGER_API std::unordered_map<int, std::shared_ptr<NodeVisual>>& VisualScript::GetNodes()
{
	return nodes;
}

/*
PURPOSE: Returns all links
*/
VISUALSCRIPTMANAGER_API std::unordered_map<int, std::pair<int, int>>& VisualScript::GetLinks()
{
	return links;
}

/*
PURPOSE: Sets the next id
*/
VISUALSCRIPTMANAGER_API void VisualScript::SetNextId(int nextId)
{
	this->nextId = nextId;
}
