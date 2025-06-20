#include "pch.h"
#include "CameraParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
ENTITYMANAGER_API void CameraParams::FromJson(const nlohmann::json& j, std::string& projectDir)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j, projectDir);

	//Set some properties
	projectionType = j.value("projection-type", (CameraProjection)PERPECTIVE);
	fov = j.value("fov", 70.0f);
}
