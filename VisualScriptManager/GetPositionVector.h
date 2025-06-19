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
	VISUALSCRIPTMANAGER_API GetPositionVector();

	//main functions
	VISUALSCRIPTMANAGER_API void				  Draw(NodeVisual* nodeVisual) override;
	VISUALSCRIPTMANAGER_API void				  Execute() override;
	VISUALSCRIPTMANAGER_API Value				  Evaluate(Pin* pin) override;

	Value EvaluateInput(Pin* pin);

	//getters and setters
	VISUALSCRIPTMANAGER_API void				  SetPos(int x, int y) override;
	VISUALSCRIPTMANAGER_API void				  SetPins() override;
	VISUALSCRIPTMANAGER_API std::string			  GetType() override {
		return "GetPositionVector";
	}
	VISUALSCRIPTMANAGER_API std::shared_ptr<Node> clone() override {
		return std::make_shared<GetPositionVector>(*this);
	}

	//json
	VISUALSCRIPTMANAGER_API nlohmann::json		  ToJson() override;
	VISUALSCRIPTMANAGER_API bool				  FromJson(nlohmann::json json) override;
private:
	//variables
	int x = 0, y = 0;
	bool first = true;
};

