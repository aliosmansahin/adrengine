#pragma once

#include "EntityParams.h"
#include "AssetDatabase.h"

#include "interfaces/IEntity/IObject/IObjectParams.h"

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class ObjectParams : public virtual EntityParams, public virtual IObjectParams
{
public:
	/*
	PURPOSE: Returns the type of the entity
	*/
	ENTITYMANAGER_API std::string GetType() override {
		return "Object";
	}

	/*
	PURPOSE: Clones the current entity and returns it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> clone() const override {
		return std::make_shared<ObjectParams>(*this);
	}

	/*
	PURPOSE: Creates json content from properties of the entity
	*/
	ENTITYMANAGER_API nlohmann::json ToJson() override {
		/*
			Its kind of "super" function.
			Which calls the function of the base class.
			In this function, we are overriding the function, so we need it
		*/
		auto j = EntityParams::ToJson();

		//Save some properties
		j["type"] = GetType();
		j["obj-id"] = objId;

		return j;
	};

	//json
	ENTITYMANAGER_API void FromJson(const nlohmann::json& j, std::string& projectDir, std::shared_ptr<IScene> scene) override;

	//Getters
	ENTITYMANAGER_API std::shared_ptr<Mesh> GetMesh() override {
		return mesh;
	};
	ENTITYMANAGER_API std::string GetObjId() override {
		return objId;
	};

	//Setters
	ENTITYMANAGER_API void SetMesh(std::shared_ptr<Mesh> mesh) override {
		this->mesh = mesh;
	};
	ENTITYMANAGER_API void SetObjId(std::string objId) override {
		this->objId = objId;
	};

private:
	//Variables for the Object
	std::string objId = "";
	std::shared_ptr<Mesh> mesh = nullptr;
};