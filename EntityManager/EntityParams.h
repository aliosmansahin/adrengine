#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "interfaces/IEntity/IEntityParams.h"
#include "interfaces/IEntity/IEntity.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

/*
	We don't have to have implementation of the Entity and VisualScript class
	Because we will use them as a pointer, also we won't use functions of them
*/
class IVisualScript;

class EntityParams : public virtual IEntityParams {
public:
	//Destructor
	ENTITYMANAGER_API virtual ~EntityParams() = default;

	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API virtual std::string GetType() override {
		return "Entity";
	}

	/*
	PURPOSE: Getter and setter for entity id
	*/
	ENTITYMANAGER_API virtual std::string GetId() override {
		return id;
	}
	ENTITYMANAGER_API void SetId(std::string newId) override {
		id = newId;
	};

	/*
	PURPOSE: Getter and setter for entity name
	*/
	ENTITYMANAGER_API std::string GetName() override {
		return name;
	};
	ENTITYMANAGER_API void		  SetName(std::string newName) override {
		name = newName;
	};

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API virtual std::shared_ptr<IEntityParams> clone() const override {
		return std::make_shared<EntityParams>(*this);
	}

	//runtime
	ENTITYMANAGER_API void ResetRuntimeValues() override;

	/*
	PURPOSE: Returns the position vector of the entity
	*/
	ENTITYMANAGER_API glm::vec3 GetPosition() const override {
		return runtimePosition;
	}

	/*
	PURPOSE: Sets the position vector of the entity
	*/
	ENTITYMANAGER_API void SetRuntimePosition(glm::vec3 vector) override {
		runtimePosition = vector;
	}
	ENTITYMANAGER_API void SetEditorPosition(glm::vec3 vector) override {
		editorPosition = vector;
		runtimePosition = vector;
	}

	/*
	PURPOSE: Returns the rotation vector of the entity
	*/
	ENTITYMANAGER_API glm::vec3 GetRotation() const override {
		return runtimeRotation;
	}

	/*
	PURPOSE: Sets the rotation vector of the entity
	*/
	ENTITYMANAGER_API void SetRuntimeRotation(glm::vec3 vector) override {
		runtimeRotation = vector;
	}
	ENTITYMANAGER_API void SetEditorRotation(glm::vec3 vector) override {
		editorRotation = vector;
		runtimeRotation = vector;
	}

	/*
	PURPOSE: Returns the scale vector of the entity
	*/
	ENTITYMANAGER_API glm::vec3 GetScale() const override {
		return runtimeScale;
	};

	/*
	PURPOSE: Set the scale vector of the entity
	*/
	ENTITYMANAGER_API void SetRuntimeScale(glm::vec3 vector) override {
		runtimeScale = vector;
	};
	ENTITYMANAGER_API void SetEditorScale(glm::vec3 vector) override {
		editorScale = vector;
		runtimeScale = vector;
	};

	/*
	PURPOSE: Returns visual script object
	*/
	ENTITYMANAGER_API std::shared_ptr<IVisualScript> GetVisualScript() override {
		return script;
	}
	/*
	PURPOSE: Sets visual script object
	*/
	ENTITYMANAGER_API void SetVisualScript(std::shared_ptr<IVisualScript> script) override {
		this->script = script;
	}

	/*
	PURPOSE: Getters and setter for parent
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntity> GetParent() override {
		return parent;
	};
	ENTITYMANAGER_API void SetParent(std::shared_ptr<IEntity> newParent) override {
		parent = newParent;
	};
	ENTITYMANAGER_API std::string GetParentId() override {
		return parentId;
	};
	ENTITYMANAGER_API void SetParentId(std::string newId) override {
		parentId = newId;
	};

	/*
	PURPOSE: Returns children entities
	*/
	ENTITYMANAGER_API std::vector<std::shared_ptr<IEntity>>& GetChildren() override {
		return children;
	};

	//json
	ENTITYMANAGER_API virtual nlohmann::json ToJson() override;
	ENTITYMANAGER_API virtual void			 FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

private:
	//Some properties for the entity
	std::string id = "";
	std::string name = "";

	//Stores the parent entity
	std::shared_ptr<IEntity> parent;
	std::string parentId = "";

	//Stores children
	std::vector<std::shared_ptr<IEntity>> children;

	//Stores the script of the entity
	std::shared_ptr<IVisualScript> script;

private:
	glm::vec3 runtimePosition = glm::vec3(0.0f);
	glm::vec3 editorPosition = glm::vec3(0.0f);
	glm::vec3 runtimeRotation = glm::vec3(0.0f);
	glm::vec3 editorRotation = glm::vec3(0.0f);
	glm::vec3 runtimeScale = glm::vec3(1.0f);
	glm::vec3 editorScale = glm::vec3(1.0f);
};