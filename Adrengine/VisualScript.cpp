#include "VisualScript.h"

bool VisualScript::CreateScript(std::string scriptId, std::string belongsTo, std::string belongsScene)
{
	this->scriptId = scriptId;
	this->belongsTo = belongsTo;
	this->belongsScene = belongsScene;
	return true;
}

void VisualScript::DrawScript()
{
}

void VisualScript::ReleaseScript()
{
	
}

void VisualScript::RunCompiled()
{
	for (auto& code : compiled) {
		code();
	}
}

nlohmann::json VisualScript::ToJson()
{
	nlohmann::json j;
	j["id"] = scriptId;
	j["belongs-to"] = belongsTo;
	j["belongs-scene"] = belongsScene;

	for (auto& node : nodes) {
		j["nodes"].push_back(node.second->ToJson());
	}

	for (auto& link : links) {
		nlohmann::json l;
		l["first"] = link.second.first;
		l["second"] = link.second.second;
		l["id"] = link.first;
		j["links"].push_back(l);
	}

	return j;
}

void VisualScript::FromJson(nlohmann::json json)
{
	scriptId = json["id"];
	belongsTo = json["belongs-to"];
	belongsScene = json["belongs-scene"];

	for (auto& nodeJson : json["nodes"]) {
		std::string nodeType = nodeJson.value("type", "");
		if (nodeType == "")
			continue;
		auto typeIter = VisualScriptManager::GetInstance().types.find(nodeType);
		if (typeIter == VisualScriptManager::GetInstance().types.end())
			continue;
		auto type = typeIter->second->clone();
		type->FromJson(nodeJson);

		nodes.insert({ type->GetId(), type });
	}

	for (auto& link : json["links"]) {
		int first = link.value("first", -1);
		int second = link.value("second", -1);
		int id = link.value("id", -1);
		if (first != -1 && second != -1 && id != -1) {
			links.insert({ id, {first, second} });
		}
	}

	VisualScriptManager::GetInstance().CompileScript(this);
}
