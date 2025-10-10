#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "../../nlohmann_json/json.hpp"

#include "../IVisualScript/IVisualScript.h"

class IScene;
class IEntity;

class IEntityParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	virtual std::string GetType() = 0;

	/*
	PURPOSE: Getter and setter for entity id
	*/
	virtual std::string GetId() = 0;
	virtual void		SetId(std::string newId) = 0;

	/*
	PURPOSE: Getter and setter for entity name
	*/
	virtual std::string	GetName() = 0;
	virtual void		SetName(std::string newName) = 0;

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	virtual std::shared_ptr<IEntityParams> clone() const = 0;

	/*
	PURPOSE: When user click stop button,
		this function is called to set runtime values to editors values
	*/
	virtual void ResetRuntimeValues() = 0;

	/*
	PURPOSE: Json seriaization/deserialization
	*/
	virtual nlohmann::json ToJson() = 0;
	virtual void		   FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) = 0;

	/*
	PURPOSE: Returns the position vector of the entity
	*/
	virtual glm::vec3 GetPosition() const = 0;

	/*
	PURPOSE: Set the position vector of the entity
	*/
	virtual void SetRuntimePosition(glm::vec3 vector) = 0;
	virtual void SetEditorPosition(glm::vec3 vector) = 0;

	/*
	PURPOSE: Returns the rotation vector of the entity
	*/
	virtual glm::vec3 GetRotation() const = 0;

	/*
	PURPOSE: Set the rotation vector of the entity
	*/
	virtual void SetRuntimeRotation(glm::vec3 vector) = 0;
	virtual void SetEditorRotation(glm::vec3 vector) = 0;

	/*
	PURPOSE: Returns the scale vector of the entity
	*/
	virtual glm::vec3 GetScale() const = 0;

	/*
	PURPOSE: Set the scale vector of the entity
	*/
	virtual void SetRuntimeScale(glm::vec3 vector) = 0;
	virtual void SetEditorScale(glm::vec3 vector) = 0;

	/*
	PURPOSE: Returns the visual script of the entity
	*/
	virtual std::shared_ptr<IVisualScript> GetVisualScript() = 0;

	/*
	PURPOSE: Sets visual script object
	*/
	virtual void SetVisualScript(std::shared_ptr<IVisualScript> script) = 0;

	/*
	PURPOSE: Getters and setters for parent
	*/
	virtual std::shared_ptr<IEntity> GetParent() = 0;
	virtual void SetParent(std::shared_ptr<IEntity> newParent) = 0;
	virtual std::string GetParentId() = 0;
	virtual void SetParentId(std::string newId) = 0; 

	/*
	PURPOSE: Returns children entities
	*/
	virtual std::vector<std::shared_ptr<IEntity>>& GetChildren() = 0;

	virtual ~IEntityParams() = default;
};