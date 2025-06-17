#include "pch.h"
#include "CameraParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
ENTITYMANAGER_API void CameraParams::FromJson(const nlohmann::json& j)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j);

	//Set some properties
	projectionType = j.value("projection-type", (CameraProjection)PERPECTIVE);
}
