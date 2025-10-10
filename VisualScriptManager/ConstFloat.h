#pragma once

#include "Node.h"

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

class ConstFloat : public Node
{
public:
	//drawing
	VISUALSCRIPTMANAGER_API void				  Draw(std::shared_ptr<NodeVisual> nodeVisual) override;

	//execution
	VISUALSCRIPTMANAGER_API Value				  Evaluate(std::shared_ptr<IPin> pin) override;

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "ConstFloat";
	}

	//clone
	VISUALSCRIPTMANAGER_API std::shared_ptr<INode> clone() override {
		return std::make_shared<ConstFloat>(*this);
	}

	//json
	VISUALSCRIPTMANAGER_API nlohmann::json		  ToJson() override;
	VISUALSCRIPTMANAGER_API bool				  FromJson(nlohmann::json json) override;
private:
	float buf = 0.0f;
};

