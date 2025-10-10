#include "pch.h"
#include "GetThisEntity.h"

/*
PURPOSE: Returns result of this node, other nodes can access it with this function
*/
VISUALSCRIPTMANAGER_API Value GetThisEntity::Evaluate(std::shared_ptr<IPin> pin)
{
    return entity;
}

/*
PURPOSE: Sets pins for this node
*/
VISUALSCRIPTMANAGER_API void GetThisEntity::SetPins()
{
    std::shared_ptr<INode> thisNode = shared_from_this();

    outputPins = {
        std::make_shared<Pin>("Entity", PinType::Entity, PinDirection::Output, thisNode),
    };
}