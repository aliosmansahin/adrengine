#include "EntityParams.h"
#include "Entity.h"

nlohmann::json EntityParams::ToJson() {
	nlohmann::json j;
	j["x"] = x;
	j["y"] = y;
	j["z"] = z;
	j["sx"] = sx;
	j["sy"] = sy;
	j["sz"] = sz;
	j["rx"] = rx;
	j["ry"] = ry;
	j["rz"] = rz;
	j["id"] = id;
	j["scriptId"] = scriptId;
	j["name"] = name;
	if (parent)
		j["parentId"] = parentId;
	j["type"] = GetType();
	return j;
}

void EntityParams::FromJson(const nlohmann::json& j) {
	x = j.value("x", 0.0f);
	y = j.value("y", 0.0f);
	z = j.value("z", 0.0f);
	sx = j.value("sx", 1.0f);
	sy = j.value("sy", 1.0f);
	sz = j.value("sz", 1.0f);
	rx = j.value("rx", 0.0f);
	ry = j.value("ry", 0.0f);
	rz = j.value("rz", 0.0f);
	id = j.value("id", "");
	name = j.value("name", "");
	scriptId = j.value("scriptId", "");
	parentId = j.value("parentId", "");
}