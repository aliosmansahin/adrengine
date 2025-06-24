#pragma once

#include "Node.h"

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

class CastToObject : public Node
{
public:
	//execution
	VISUALSCRIPTMANAGER_API Value				  Evaluate(Pin* pin) override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "CastToObject";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<Node> clone() override {
		return std::make_shared<CastToObject>(*this);
	}
};

