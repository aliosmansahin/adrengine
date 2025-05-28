#include "pch.h"
#include "LightParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
void LightParams::FromJson(const nlohmann::json& j)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j);

	//Set some properties
	color.x = j.value("color-x", 1.0f);
	color.y = j.value("color-y", 1.0f);
	color.z = j.value("color-z", 1.0f);
}
