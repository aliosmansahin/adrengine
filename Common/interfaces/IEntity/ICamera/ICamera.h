#pragma once

#include "../IEntity.h"
#include "ICameraParams.h"

class ICamera : public virtual IEntity
{
public:
	virtual std::pair<float, float> GetYawPitch() = 0;

	virtual void AddPosition(glm::vec3 posAdd, bool isPlaying) = 0;
	virtual void AddRotation(float _yaw, float _pitch, bool isPlaying) = 0;

	virtual void SetPosition(glm::vec3 newPos, bool isPlaying) = 0;
	virtual void SetRotation(float _yaw, float _pitch, bool isPlaying) = 0;

	virtual CameraProjection GetProjectionType() = 0;
	virtual float GetFOV() = 0;
};

