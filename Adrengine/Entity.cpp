#include "Entity.h"

bool Entity::CreateEntity(std::shared_ptr<EntityParams> params)
{
    this->params = params;
    return true;
}

void Entity::DeleteEntity()
{
}

void Entity::Update()
{
}

void Entity::Draw()
{
}

EntityParams* Entity::GetEntityParams()
{
    return params.get();
}

nlohmann::json Entity::ToJson()
{
    nlohmann::json j;

    if (params) {
        j = params->ToJson();  // EntityParams'ý JSON'a çevir
    }
    return j;
}
