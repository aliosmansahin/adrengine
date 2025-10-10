#pragma once

#include <memory>
#include <vector>

#include "../../../nlohmann_json/json.hpp"

#include "../IPin/IPin.h"

class INode;

struct NodeVisual {
	std::shared_ptr<INode> logicNode;
	int id;
	std::vector<int> inputIds;
	std::vector<int> outputIds;
};

class INode
{
public:
	virtual void			BeginDraw(std::shared_ptr<NodeVisual> nodeVisual) = 0;
	virtual void			Draw(std::shared_ptr<NodeVisual> nodeVisual) = 0;
	virtual void			EndDraw() = 0;

	virtual std::shared_ptr<INode> clone() = 0;

	virtual void            Execute() = 0;

	virtual std::string		GetType() = 0;

	virtual std::vector<std::shared_ptr<IPin>>& GetInputPins() = 0;
	virtual std::vector<std::shared_ptr<IPin>>& GetOutputPins() = 0;

	virtual void			SetPos(int x, int y) = 0;
	virtual void			SetPins() = 0;
	
	virtual nlohmann::json	ToJson() = 0;
	virtual bool			FromJson(nlohmann::json json) = 0;
};

