#include "pch.h"
#include "EntityParams.h"
#include "Entity.h"

#include "interfaces/IVisualScriptManager/IVisualScriptManager.h"
#include "ServiceLocator.h"

/*
PURPOSE: Sets runtime values to editor values
*/
ENTITYMANAGER_API void EntityParams::ResetRuntimeValues() {
	runtimePosition = editorPosition;
	runtimeRotation = editorRotation;
	runtimeScale = editorScale;
}

/*
PURPOSE: Creates json content from properties of the entity
*/
nlohmann::json EntityParams::ToJson() {
	nlohmann::json j;
	j["x"] = editorPosition.x;
	j["y"] = editorPosition.y;
	j["z"] = editorPosition.z;
	j["sx"] = editorScale.x;
	j["sy"] = editorScale.y;
	j["sz"] = editorScale.z;
	j["rx"] = editorRotation.x;
	j["ry"] = editorRotation.y;
	j["rz"] = editorRotation.z;
	j["id"] = id;
	if(script)
		j["scriptId"] = script->GetScriptId();
	j["name"] = name;
	if (parent)
		j["parentId"] = parentId;
	j["type"] = GetType();
	return j;
}

/*
PURPOSE: Sets properties of the entity from its json
*/
void EntityParams::FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) {
	editorPosition.x = j.value("x", 0.0f);
	editorPosition.y = j.value("y", 0.0f);
	editorPosition.z = j.value("z", 0.0f);
	runtimePosition.x = editorPosition.x;
	runtimePosition.y = editorPosition.y;
	runtimePosition.z = editorPosition.z;

	editorScale.x = j.value("sx", 1.0f);
	editorScale.y = j.value("sy", 1.0f);
	editorScale.z = j.value("sz", 1.0f);
	runtimeScale.x = editorScale.x;
	runtimeScale.y = editorScale.y;
	runtimeScale.z = editorScale.z;
	
	editorRotation.x = j.value("rx", 0.0f);
	editorRotation.y = j.value("ry", 0.0f);
	editorRotation.z = j.value("rz", 0.0f);
	runtimeRotation.x = editorRotation.x;
	runtimeRotation.y = editorRotation.y;
	runtimeRotation.z = editorRotation.z;

	id = j.value("id", "");
	name = j.value("name", "");

	//TODO: Load own script
	std::string scriptId = j.value("scriptId", "");
	
	if (!scriptId.empty()) {
		script = ServiceLocator::Get<IVisualScriptManager>()->LoadScript(scriptId, scene);
	}

	parentId = j.value("parentId", "");
}