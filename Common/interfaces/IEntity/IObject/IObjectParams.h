#pragma once

#include "../IEntityParams.h"

struct Mesh;
class RigidBody;

class IObjectParams : public virtual IEntityParams
{
public:
	virtual ~IObjectParams() = default;

	virtual std::shared_ptr<Mesh> GetMesh() = 0;
	virtual std::string GetObjId() = 0;

	virtual void SetMesh(std::shared_ptr<Mesh> mesh) = 0;
	virtual void SetObjId(std::string objId) = 0;
};

