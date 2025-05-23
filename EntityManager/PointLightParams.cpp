#include "pch.h"
#include "PointLightParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
void PointLightParams::FromJson(const nlohmann::json& j)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j);

	//Set some properties
	constant = j.value("constant", 1.0f);
	linear = j.value("linear", 1.0f);
	quadratic = j.value("quadratic", 1.0f);
}
