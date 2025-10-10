#include "pch.h"
#include "ObjectParams.h"
#include "AssetDatabase.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
void ObjectParams::FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j, projectDir, scene);

	//Set some properties
	objId = j.value("obj-id", "");

	//Set the mesh of the entity from its id
	this->mesh = AssetDatabase::GetInstance().GetMesh(objId);
}
