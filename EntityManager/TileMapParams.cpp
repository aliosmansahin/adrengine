#include "pch.h"
#include "TileMapParams.h"

/*
PURPOSE: Sets properties of the entity from its json
*/
void TileMapParams::FromJson(const nlohmann::json& j)
{
	/*
		Its kind of "super" function.
		Which calls the function of the base class.
		In this function, we are overriding the function, so we need it
	*/
	EntityParams::FromJson(j);
}