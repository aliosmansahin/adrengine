#include "pch.h"
#include "DirectionalLightParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
void DirectionalLightParams::FromJson(const nlohmann::json& j)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j);

	//Set some properties
	direction.x = j.value("direction-x", 1.0f);
	direction.y = j.value("direction-y", 1.0f);
	direction.z = j.value("direction-z", 1.0f);
}
