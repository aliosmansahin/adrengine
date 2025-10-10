#pragma once

#include "Node.h"

#include "interfaces/IVisualScript/INode/IGetThisEntity/IGetThisEntity.h"

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

class GetThisEntity : public Node, public IGetThisEntity
{
public:
	//execution
	VISUALSCRIPTMANAGER_API Value				  Evaluate(std::shared_ptr<IPin> pin) override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "GetThisEntity";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<INode> clone() override {
		return std::make_shared<GetThisEntity>(*this);
	}

	void SetEntity(std::shared_ptr<IEntity> entity) override {
		this->entity = entity;
	}

private:
	//Entity ptr
	std::shared_ptr<IEntity> entity = nullptr;
};

