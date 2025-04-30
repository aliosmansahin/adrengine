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
	// add codes
	return j;
}

void VisualScript::FromJson(nlohmann::json json)
{
	scriptId = json["id"];
	belongsTo = json["belongs-to"];
	belongsScene = json["belongs-scene"];
}
