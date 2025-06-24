#include "pch.h"
#include "EntityParams.h"
#include "Entity.h"

#include "VisualScriptManager.h"

/*
PURPOSE: Sets runtime values to editor values
*/
ENTITYMANAGER_API void EntityParams::ResetRuntimeValues() {
	runtimePosition = editorPosition;
}

/*
PURPOSE: Creates json content from properties of the entity
*/
nlohmann::json EntityParams::ToJson() {
	nlohmann::json j;
	j["x"] = editorPosition.x;
	j["y"] = editorPosition.y;
	j["z"] = editorPosition.z;
	j["sx"] = sx;
	j["sy"] = sy;
	j["sz"] = sz;
	j["rx"] = rx;
	j["ry"] = ry;
	j["rz"] = rz;
	j["id"] = id;
	if(script)
		j["scriptId"] = script->scriptId;
	j["name"] = name;
	if (parent)
		j["parentId"] = parentId;
	j["type"] = GetType();
	return j;
}

/*
PURPOSE: Sets properties of the entity from its json
*/
void EntityParams::FromJson(const nlohmann::json& j, std::string& projectDir, IScene* scene) {
	editorPosition.x = j.value("x", 0.0f);
	editorPosition.y = j.value("y", 0.0f);
	editorPosition.z = j.value("z", 0.0f);
	runtimePosition.x = editorPosition.x;
	runtimePosition.y = editorPosition.y;
	runtimePosition.z = editorPosition.z;
	sx = j.value("sx", 1.0f);
	sy = j.value("sy", 1.0f);
	sz = j.value("sz", 1.0f);
	rx = j.value("rx", 0.0f);
	ry = j.value("ry", 0.0f);
	rz = j.value("rz", 0.0f);
	id = j.value("id", "");
	name = j.value("name", "");

	//TODO: Load own script
	std::string scriptId = j.value("scriptId", "");
	
	if(!scriptId.empty())
		script = VisualScriptManager::GetInstance().LoadScript(scriptId, projectDir, scene);

	parentId = j.value("parentId", "");
}