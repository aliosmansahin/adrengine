#include "pch.h"
#include "VisualScript.h"

/*
TODO: Move them to somewhere else like "common"
*/
enum TabType {
	SceneEditor,
	VisualScriptEditor
};

struct Tab {
	std::string id;
	TabType tabType;
};

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
	compiled.clear();
	//The variables is already being deleted
}

/*
PURPOSE: Calls compiled functions
*/
void VisualScript::RunCompiled()
{
	for (auto& code : compiled) {
		code();
	}
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
		j["nodes"].push_back(node.second->ToJson());
	}

	//All links
	for (auto& link : links) {
		nlohmann::json l;
		l["first"] = link.second.first;
		l["second"] = link.second.second;
		l["id"] = link.first;
		j["links"].push_back(l);
	}

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
		auto type = typeIter->second->clone();
		type->FromJson(nodeJson);

		nodes.insert({ type->GetId(), type });
	}

	//Load each link and insert it to "links"
	for (auto& link : json["links"]) {
		int first = link.value("first", -1);
		int second = link.value("second", -1);
		int id = link.value("id", -1);
		if (first != -1 && second != -1 && id != -1) {
			links.insert({ id, {first, second} });
		}
	}

	//Compile the script after loading
	CompileScript();
}

/*
PURPOSE: Compiles the script
*/
std::string VisualScript::CompileScript()
{
	//Some stuff
	std::unordered_map<int, int> organizedNodes;

	//Look for begin node
	auto begin = nodes.find(0);
	if (begin == nodes.end())
		return "Compile: Begin Entrypoint could not found";

	//Look for return node
	auto returnNode = nodes.find(2);
	if (returnNode == nodes.end())
		return "Compile: Return could not found";

	bool returnConnected = false;

	
	//Organize nodes by links
	for (auto& link : links) {
		int first = link.second.first;
		int second = link.second.second;

		auto beforeNode = nodes.find(first - 1); //-1 output //-2 input
		auto afterNode = nodes.find(second - 2);

		organizedNodes.insert(std::pair<int, int>(beforeNode->first, afterNode->first));

		//Check if the return node is connected to another node
		if (second == 2 + 2) { //Return Id 
			returnConnected = true;
		}
	}

	//Check the begin node and end node are connected
	int id = 0;
	std::unordered_map<int, int>::iterator organizedNode = organizedNodes.find(id);
	if (organizedNode == organizedNodes.end())
		return "Compile: Begin Entrypoint wasn't connected to any node";

	if (!returnConnected)
		return "Compile: Return wasn't connected to any node";

	//Compile each organized node and push compiled code to the vector
	compiled.clear();

	while (true) {
		auto nodeIter = nodes.find(organizedNode->second);
		if (nodeIter->second->GetType() == "Return")
			break;
		compiled.push_back(nodeIter->second->Compile());
		organizedNode = organizedNodes.find(organizedNode->second);
	}

	//Return nothing if compilation was successful
	return "";
}
