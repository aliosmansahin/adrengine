#pragma once

#include "../IEntityParams.h"

enum CameraProjection {
	PERPECTIVE,
	ORTHOGRAPHIC
};

class ICameraParams : public virtual IEntityParams
{
public:
	//Getters for parameters
	virtual CameraProjection GetProjectionType() = 0;
	virtual float			 GetFOV() = 0;

	//Setters for parameters
	virtual void SetProjectionType(CameraProjection newProjectionType) = 0;
	virtual void SetFOV(float fov) = 0;
};

