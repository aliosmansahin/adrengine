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

	////All nodes
	//for (auto& node : nodes) {
	//	j["nodes"].push_back(node.second->ToJson());
	//}

	////All links
	//for (auto& link : links) {
	//	nlohmann::json l;
	//	l["first"] = link.second.first;
	//	l["second"] = link.second.second;
	//	l["id"] = link.first;
	//	j["links"].push_back(l);
	//}

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
	//for (auto& nodeJson : json["nodes"]) {
	//	std::string nodeType = nodeJson.value("type", "");
	//	if (nodeType == "")
	//		continue;
	//	auto typeIter = types.find(nodeType);
	//	if (typeIter == types.end())
	//		continue;
	//	auto type = typeIter->second->clone();
	//	type->FromJson(nodeJson);

	//	nodes.insert({ type->GetId(), type });
	//}

	////Load each link and insert it to "links"
	//for (auto& link : json["links"]) {
	//	int first = link.value("first", -1);
	//	int second = link.value("second", -1);
	//	int id = link.value("id", -1);
	//	if (first != -1 && second != -1 && id != -1) {
	//		links.insert({ id, {first, second} });
	//	}
	//}
}
