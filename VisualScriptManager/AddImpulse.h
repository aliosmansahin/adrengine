#pragma once

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

#include "Node.h"

class AddImpulse : public Node
{
public:
	//execution
	VISUALSCRIPTMANAGER_API void				  Execute() override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "AddImpulse";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<INode> clone() override {
		return std::make_shared<AddImpulse>(*this);
	}
};

