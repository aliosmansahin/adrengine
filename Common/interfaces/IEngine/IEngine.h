#pragma once

#include <memory>

#include "../IEntityManager/IEntityManager.h"

#include "../IPhysics/IPhysics.h"

struct GLFWwindow;

class IEngine
{
public:
	virtual ~IEngine() = default;

	//main funcs
	virtual bool InitEngine(GLFWwindow* window) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void CloseEngine() = 0;

	//Creating objects
	virtual std::shared_ptr<IEntityManager> CreateEntityManager() = 0;
	virtual std::shared_ptr<IEntity> CreateEntity(std::string entityType) = 0;
	virtual std::shared_ptr<IScene> CreateScene() = 0;
	virtual std::shared_ptr<IPhysics> CreatePhysics() = 0;
	virtual std::shared_ptr<IRigidBody> CreateRigidBody() = 0;

	//Getters
	virtual std::unordered_map<std::string, std::pair<std::shared_ptr<IEntity>, std::shared_ptr<IEntityParams>>>& GetEntityTypes() = 0;
	virtual std::pair<int, int> GetScreenSize() = 0;
	virtual std::pair<float, float> GetFPSandMS() = 0;
	virtual std::pair<int, int> GetMainWindowSize() = 0;
};

