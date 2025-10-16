#pragma once

#include "Entity.h"
#include "Sprite2DParams.h"

#include "glad_wrapper.h"

#include "interfaces/IEntity/ISprite2D/ISprite2D.h"

using namespace adr;

#ifdef ENTITYMANAGER_EXPORTS
#define ENTITYMANAGER_API __declspec(dllexport)
#else
#define ENTITYMANAGER_API __declspec(dllimport)
#endif

class Sprite2D : public virtual Entity, public virtual ISprite2D
{
public:
	ENTITYMANAGER_API virtual ~Sprite2D() = default;

	//main functions
	ENTITYMANAGER_API bool CreateEntity(std::shared_ptr<IEntityParams> params) override;
	ENTITYMANAGER_API void DeleteEntity() override;
	ENTITYMANAGER_API void Update() override;
	ENTITYMANAGER_API void Draw(glm::vec3 currentSceneCameraPos) override;

	/*
	PURPOSE: Clones the entity and return it
	*/
	ENTITYMANAGER_API std::shared_ptr<IEntity> clone() const override {
		return std::make_shared<Sprite2D>(*this);
	}

	//properties
	ENTITYMANAGER_API std::shared_ptr<IEntityParams> GetEntityParams() override;

	//json
	ENTITYMANAGER_API nlohmann::json ToJson() override;

protected:
	//Drawing buffers
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

private:
	//properties
	std::shared_ptr<ISprite2DParams> params;
};

