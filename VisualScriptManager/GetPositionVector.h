#pragma once

#include "Node.h"

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

class GetPositionVector : public Node
{
public:
	//execution
	VISUALSCRIPTMANAGER_API Value				  Evaluate(std::shared_ptr<IPin> pin) override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "GetPositionVector";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<INode> clone() override {
		return std::make_shared<GetPositionVector>(*this);
	}
};

