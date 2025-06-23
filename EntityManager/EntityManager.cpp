#include "pch.h"
#include "EntityManager.h"
#include "Graphics.h"
#include "VisualScript.h"

/*
PURPOSE: Initialize the entity manager
*/
bool EntityManager::InitEntityManager()
{
	Logger::Log("P", "Initalizing entity manager");

	return true;
}

/*
PURPOSE: Calculates all transforms of each lights,
	creates depth maps for lights,
	draws all entities and lights
*/
void EntityManager::DrawEntities(int window_width, int window_height, glm::vec3 currentSceneCameraPos, bool is3D)
{
	adr_glEnable(GL_DEPTH_TEST);
	adr_glEnable(GL_BLEND);
	adr_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	adr_glEnable(GL_CULL_FACE); //TODO: Will be changed to each object type
	int numLights = 0;
	if (is3D) {
		/*
			FIXME:

			Point light and spot light are not working properly with shadows,
			So, all shadows except directional light shadow will be disable until the bug was fixed

		*/

		const unsigned int SHADOW_WIDTH = 8196, SHADOW_HEIGHT = 8196;
		for (auto& entityIter : entities) {
			auto entity = entityIter.second.get();
			auto params = entity->GetEntityParams();


			if (params->GetType() == "SpotLight") {
				auto casted = dynamic_cast<SpotLight*>(entity);
				auto castedParams = dynamic_cast<SpotLightParams*>(params);
				if (casted && castedParams) {
					casted->index = numLights;//Set the index of the light. For example, 0. index of all lights

					//Use right shaders
					//ShaderManager::GetInstance().UseShaders(DEPTH);
					//glBindFramebuffer(GL_FRAMEBUFFER, casted->depthMapFBO);
					//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					//glClear(GL_DEPTH_BUFFER_BIT);
					//glCullFace(GL_FRONT);
					////Send the light matrix to the shader
					//ShaderManager::GetInstance().ApplyUniformMatrix("lightSpaceMatrix", casted->lightSpaceMatrix);
					//for (auto& entity : entities) {
					//	entity.second->Draw(currentSceneCameraPos);
					//}
					//glCullFace(GL_BACK);

					/*const int width = SHADOW_WIDTH;
					const int height = SHADOW_HEIGHT;
					float* depthData = new float[width * height];

					glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depthData);

					float minDepth = 1.0f, maxDepth = 0.0f;
					for (int i = 0; i < SHADOW_WIDTH * SHADOW_HEIGHT; ++i) {
						if (depthData[i] < minDepth) minDepth = depthData[i];
						if (depthData[i] > maxDepth) maxDepth = depthData[i];
					}

					// Normalize ve 8-bit e çevir
					std::vector<unsigned char> imageData(SHADOW_WIDTH * SHADOW_HEIGHT);
					for (int i = 0; i < SHADOW_WIDTH * SHADOW_HEIGHT; ++i) {
						imageData[i] = static_cast<unsigned char>(255 * (depthData[i] - minDepth) / (maxDepth - minDepth));
					}

					// PGM header ve veriyi yaz
					std::ofstream outFile("depthMap.pgm", std::ios::binary);
					outFile << "P5\n" << SHADOW_WIDTH << " " << SHADOW_HEIGHT << "\n255\n";
					outFile.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());
					outFile.close();

					delete[] depthData;

					glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

					numLights++;
				}
			}
			else if (params->GetType() == "DirectionalLight") {
				auto casted = dynamic_cast<DirectionalLight*>(entity);
				auto castedParams = dynamic_cast<DirectionalLightParams*>(params);
				if (casted && castedParams) {
					casted->index = numLights;//Set the index of the light. For example, 0. index of all lights

					//Use right shaders
					ShaderManager::GetInstance().UseShaders(Utils::DEPTH);
					adr_glBindFramebuffer(GL_FRAMEBUFFER, casted->depthMapFBO);

					adr_glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					adr_glClear(GL_DEPTH_BUFFER_BIT);
					adr_glCullFace(GL_FRONT);
					//Send the light matrix to the shader
					ShaderManager::GetInstance().ApplyUniformMatrix("lightSpaceMatrix", casted->lightSpaceMatrix);
					for (auto& entity : entities) {
						entity.second->Draw(currentSceneCameraPos);
					}
					adr_glCullFace(GL_BACK);
					adr_glBindFramebuffer(GL_FRAMEBUFFER, 0);

					numLights++;
				}
			}
			else if (params->GetType() == "PointLight") {
				auto casted = dynamic_cast<PointLight*>(entity);
				auto castedParams = dynamic_cast<PointLightParams*>(params);
				if (casted && castedParams) {
					casted->index = numLights; //Set the index of the light. For example, 0. index of all lights
					////Use right shaders
					//ShaderManager::GetInstance().UseShaders(DEPTH_CUBE);
					////Use DEPTH_CUBE shader which is for point lights and send the parameters and matrices to it

					//for (unsigned int face = 0; face < 6; ++face) {
					//	//Each face matrices
					//	std::string uniformName = "shadowMatrices[" + std::to_string(face) + "]";
					//	ShaderManager::GetInstance().ApplyUniformMatrix(uniformName.c_str(), casted->shadowTransforms[face]);
					//}

					////Parameters
					//ShaderManager::GetInstance().ApplyUniformVec3("lightPos", casted->realPos);
					//ShaderManager::GetInstance().ApplyUniformFloat("far_plane", casted->far_plane);

					//glBindFramebuffer(GL_FRAMEBUFFER, casted->depthMapFBO);
					//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					//glClear(GL_DEPTH_BUFFER_BIT);
					//glCullFace(GL_FRONT);

					//for (auto& entity : entities) {
					//	entity.second->Draw(currentSceneCameraPos);
					//}
					//glCullFace(GL_BACK);
					//glBindFramebuffer(GL_FRAMEBUFFER, 0);

					numLights++;
				}
			}
		}

		//Use SHADER_3D shaders which is for 3d drawing
		ShaderManager::GetInstance().UseShaders(Utils::SHADER_3D);
		ShaderManager::GetInstance().ApplyUniformInt("numLights", numLights);

		for (auto& entityIter : entities) {
			auto entity = entityIter.second.get();
			auto params = entity->GetEntityParams();

			if (params->GetType() == "SpotLight") {
				auto casted = dynamic_cast<SpotLight*>(entity);
				auto castedParams = dynamic_cast<SpotLightParams*>(params);
				if (casted && castedParams) {
					int i = casted->index;

					//Set light parameters
					std::string base = "lights[" + std::to_string(i) + "]";
					ShaderManager::GetInstance().ApplyUniformInt((base + ".type").c_str(), 2);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".position").c_str(), casted->realPos);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".direction").c_str(), glm::normalize(castedParams->direction));
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".color").c_str(), castedParams->color);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".cutOff").c_str(), castedParams->cutOff);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".outerCutOff").c_str(), castedParams->outerCutOff);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".constant").c_str(), castedParams->constant);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".linear").c_str(), castedParams->linear);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".quadratic").c_str(), castedParams->quadratic);

					//Bind depthMap as a texture
					GLuint textureUnit = i + (unsigned int)entities.size(); // 10'dan baþlat: çatýþmayý önler
					adr_glActiveTexture(GL_TEXTURE0 + textureUnit);
					adr_glBindTexture(GL_TEXTURE_2D, casted->depthMap);

					//ShaderManager::GetInstance().ApplyUniformInt(("shadowMaps[" + std::to_string(i) + "]").c_str(), textureUnit);
					ShaderManager::GetInstance().ApplyUniformMatrix(("lightSpaceMatrices[" + std::to_string(i) + "]").c_str(), casted->lightSpaceMatrix);
				}
			}

			else if (params->GetType() == "DirectionalLight") {
				auto casted = dynamic_cast<DirectionalLight*>(entity);
				auto castedParams = dynamic_cast<DirectionalLightParams*>(params);
				if (casted && castedParams) {
					int i = casted->index;

					//Set light parameters
					std::string base = "lights[" + std::to_string(i) + "]";
					ShaderManager::GetInstance().ApplyUniformInt((base + ".type").c_str(), 0);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".direction").c_str(), castedParams->direction);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".color").c_str(), castedParams->color);

					//Bind depthMap as a texture
					GLuint textureUnit = i + (unsigned int)entities.size();
					adr_glActiveTexture(GL_TEXTURE0 + textureUnit);//Each light has unique texture unit
					adr_glBindTexture(GL_TEXTURE_2D, casted->depthMap);

					//Send some stuff to the shader
					ShaderManager::GetInstance().ApplyUniformMatrix(("lightSpaceMatrices[" + std::to_string(i) + "]").c_str(), casted->lightSpaceMatrix);
					ShaderManager::GetInstance().ApplyUniformInt(("shadowMaps[" + std::to_string(i) + "]").c_str(), textureUnit);
				}
			}

			else if (params->GetType() == "PointLight") {
				auto casted = dynamic_cast<PointLight*>(entity);
				auto castedParams = dynamic_cast<PointLightParams*>(params);
				if (casted && castedParams) {
					int i = casted->index;

					//Set light parameters
					std::string base = "lights[" + std::to_string(i) + "]";
					ShaderManager::GetInstance().ApplyUniformInt((base + ".type").c_str(), 1);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".position").c_str(), casted->realPos);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".color").c_str(), castedParams->color);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".constant").c_str(), castedParams->constant);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".linear").c_str(), castedParams->linear);
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".quadratic").c_str(), castedParams->quadratic);

					//Bind depthMap as a texture
					GLuint textureUnit =  i + (unsigned int)entities.size(); //Add an offset to prevent any conflict

					adr_glActiveTexture(GL_TEXTURE0 + textureUnit);
					adr_glBindTexture(GL_TEXTURE_CUBE_MAP, casted->depthMap);

					//Send some stuff to the shader
					//ShaderManager::GetInstance().ApplyUniformInt(("shadowCubeMaps[" + std::to_string(i) + "]").c_str(), texUnit);
					ShaderManager::GetInstance().ApplyUniformFloat(("far_planes[" + std::to_string(i) + "]").c_str(), casted->far_plane);
				}
			}
		}
	}
	else {
		//Use SHADER_2D shaders which is for 2d drawing
		ShaderManager::GetInstance().UseShaders(Utils::SHADER_2D);
	}

	/*
		Start to draw.
		Draw to a frame buffer.
		This frame buffer will create a texture.
		This texture will be used by ImGui as a image to draw the scene
	*/
	Graphics::GetInstance().GetMainFramebuffer()->BindFramebuffer();
	Graphics::GetInstance().Clear();
	adr_glViewport(0, 0, window_width, window_height);

	/*
		Main drawing
		INFO: Here we are sorting entities and pass them into a vector,
		for 2d we want to draw blending objects ordered as their z-axis,
		for 3d we will calculate the distance between each object and camera
		otherwise blending might not work correctly
	*/
	if (is3D) {
		std::vector<std::shared_ptr<Entity>> sortedEntities;

		//Draw not blending objects
		for (auto& entity : entities) {
			//Check if the object is a blending object
			if(entity.second->GetEntityParams()->GetType() != "FlipBook" &&
				entity.second->GetEntityParams()->GetType() != "TileMap" &&
				entity.second->GetEntityParams()->GetType() != "Sprite2D")
				//Unless, draw the object
				entity.second->Draw(currentSceneCameraPos);
			else {
				//Pass the blending object into another vector
				sortedEntities.push_back(entity.second);
			}
		}

		//Sort objects
		std::sort(sortedEntities.begin(), sortedEntities.end(), [&currentSceneCameraPos](const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
			//calculate distance between camera and the object
			float disA = glm::distance(currentSceneCameraPos, glm::vec3(a->GetEntityParams()->x, a->GetEntityParams()->y, a->GetEntityParams()->z));
			float disB = glm::distance(currentSceneCameraPos, glm::vec3(b->GetEntityParams()->x, b->GetEntityParams()->y, b->GetEntityParams()->z));

			return disA < disB;
			});

		//Draw blending object as sorted order
		for (auto& entity : sortedEntities) {
			if (!(entity->GetEntityParams()->GetType() != "FlipBook" &&
				entity->GetEntityParams()->GetType() != "TileMap" &&
				entity->GetEntityParams()->GetType() != "Sprite2D"))
				entity->Draw(currentSceneCameraPos);
		}
	}
	else {
		//Sort all 2d entities and draw them
		std::vector<std::pair<std::string, std::shared_ptr<Entity>>> sortedEntities(entities.begin(), entities.end());

		std::sort(sortedEntities.begin(), sortedEntities.end(), [](const std::pair<std::string, std::shared_ptr<Entity>>& a, const std::pair<std::string, std::shared_ptr<Entity>>& b) {
			return a.second->GetEntityParams()->z < b.second->GetEntityParams()->z;
			});

		for (auto& entity : sortedEntities) {
			entity.second->Draw(currentSceneCameraPos);
		}
	}

	//Disable the frame buffer to draw ImGui image
	Graphics::GetInstance().GetMainFramebuffer()->UnbindFramebuffer();

	//Clear all sets
	for (int i = 0; i < numLights; ++i) {
		adr_glActiveTexture(GL_TEXTURE0 + i + 10);
		adr_glBindTexture(GL_TEXTURE_2D, 0);
		adr_glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	adr_glDisable(GL_CULL_FACE);
	adr_glDisable(GL_DEPTH_TEST);
}

/*
PURPOSE: Performs all of deleting entity actions and updates all entities
*/
void EntityManager::UpdateEntities(
	bool windowSceneFocused,
	bool windowSceneDeletePressed,
	bool& pendingDelete,
	std::string selectedId,
	std::function<void(std::string)> extraDeletingFunc,
	std::string& projectDir,
	std::string& sceneId,
	nlohmann::json& currentSceneJson,
	Camera*& gameCamera)
{
	//Perform deleting entity actions
	if ((windowSceneFocused && windowSceneDeletePressed) || pendingDelete) {
		//Check if the entity exists
		auto entityIter = entities.find(selectedId);

		if (entityIter == entities.end()) {
			std::string str = "There is not any entity which has given id \"";
			str += selectedId;
			str += "\"";
			Logger::Log("E", str.c_str());
		}

		//Get script id of the entity
		std::string scriptId = "";
		if (entityIter->second->GetEntityParams()->script)
			scriptId = entityIter->second->GetEntityParams()->script->scriptId;

		//Remove the entity
		RemoveEntity(entityIter->second.get(), projectDir, sceneId, currentSceneJson);

		//Erase it
		entities.erase(entityIter);

		//Callback function for deleting tab and select entity to nothing
		extraDeletingFunc(scriptId);

		//Pending delete
		if (pendingDelete) {
			pendingDelete = false;
		}
	}

	/*
		Make entitites to follow their parents.
		Here we are setting the origin of the entity to parent's location in the space if the entity has a parent
	*/
	for (auto& entity : entities) {
		SetEntityRealStats(entity.second.get());
	}

	//Update all entities
	for (auto& entity : entities) {
		entity.second->Update();
		if (entity.second->GetEntityParams()->GetType() == "Camera") {
			Camera* camera = dynamic_cast<Camera*>(entity.second.get());
			if (camera) {
				gameCamera = camera;
			}
		}
	}
}

/*
PURPOSE: Sets the real pos, rot, sca of the entity depends on its parent,
	for example, if the parent x=10, the child x=5, the entity will be shown at x=15 in the space
*/
ENTITYMANAGER_API void EntityManager::SetEntityRealStats(Entity* entity)
{
	//Set real stats
	auto params = entity->GetEntityParams();
	auto parent = params->parent.get();
	if (parent) {
		entity->realPos = parent->realPos + glm::vec3(params->x, params->y, params->z);
		entity->realRot = parent->realRot + glm::vec3(params->rx, params->ry, params->rz);
		entity->realSca = parent->realSca * glm::vec3(params->sx, params->sy, params->sz);
	}
	else {
		entity->realPos = glm::vec3(params->x, params->y, params->z);
		entity->realRot = glm::vec3(params->rx, params->ry, params->rz);
		entity->realSca = glm::vec3(params->sx, params->sy, params->sz);
	}
	//Do that again to all children
	for (auto& child : params->children) {
		SetEntityRealStats(child.get());
	}
}

/*
PURPOSE: Runs begin nodes of all scripts of entities
*/
ENTITYMANAGER_API void EntityManager::RunEntitiesScriptBegin()
{
	for (auto& entity : entities) {
		if(entity.second->GetEntityParams()->script)
			entity.second->GetEntityParams()->script->ExecuteBeginScript();
	}
}

/*
PURPOSE: Returns entity by id, check out getElementById of javascript
*/
ENTITYMANAGER_API Entity* EntityManager::GetEntityById(std::string id)
{
	auto entityIter = entities.find(id);
	if (entityIter == entities.end())
		return nullptr;

	return entityIter->second.get();
}

/*
PURPOSE: Releases all manager stuff
*/
void EntityManager::ReleaseEntityManager()
{
	//Release entities
	for (auto& entity : entities)
		entity.second->DeleteEntity();
	entities.clear();

	//Logger
	Logger::Log("P", "Released entity manager");
}

/*
PURPOSE: Creates a new entity and inserts it to entities
*/
std::string EntityManager::CreateEntity(
	std::string type,
	std::string& projectDir,
	std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes,
	std::string sceneId,
	nlohmann::json& currentSceneJson,
	std::shared_ptr<Entity>& parent)
{
	//std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";

	//Set the id
	int index = 0;
	std::string entityId = "";
	while (true) {
		entityId = type + std::to_string(index);
		std::string entitiesDir = projectDir + "entities/";
		std::string entityDir = entitiesDir + entityId;
		if(!std::filesystem::exists(entityDir))
			break;
		++index;
	}

	//Check if the type of the entity exists
	auto typeIter = entityTypes.find(type);
	if (typeIter == entityTypes.end()) {
		std::string str = "Could not find entity type \"";
		str += entityId;
		str += "\"";

		Logger::Log("P", str.c_str());
		return "";
	}

	/*
		Create clones of the entity and parameters.
		Here we ain't creating the entity and parameters staticly.
		Depends on the type, we are creating them dynamicly.
		If the type is "Object", it will create an Object entity and its parameters.
		This allows us to create new entity types easily
	*/
	auto entity = typeIter->second.first->clone();
	auto params = typeIter->second.second->clone();

	//Check if the entity has a parent
	if (parent.get()) {
		params->parent = parent;
		params->parentId = parent->GetEntityParams()->id;
	}

	//Set the params
	params->id = entityId;
	params->name = entityId;

	//Initialize entity with its params
	if (!entity->CreateEntity(params))
		return "";
	
	//Add the entity into its parent's children
	if (parent.get()) {
		parent->GetEntityParams()->children.push_back(entity);
	}
	
	//Insert the entity into entities
	entities.insert(std::pair<std::string, std::shared_ptr<Entity>>(entityId, entity));

	//Save the scene and entities which belong to the scene
	for (auto& entityIter : GetEntities()) {
		//Save each entity
		Entity* entity = entityIter.second.get();
		if (!entity) continue;

		auto params = entity->GetEntityParams();
		if (!params || params->id.empty()) continue;

		AssetSaver::SaveEntityToFile(entity->ToJson(), projectDir, params->id);
	}

	currentSceneJson["entities"].push_back(params->id); //The scene will have the new entity

	AssetSaver::SaveSceneToFile(currentSceneJson, projectDir, sceneId);

	//Some logger
	std::string str = "Created new entity \"";
	str += entityId;
	str += "\"";

	Logger::Log("P", str.c_str());

	//Return entity id to use it
	return entityId;
}

/*
PURPOSE: Deletes entity from project directory and removes it from the scene
*/
bool EntityManager::RemoveEntity(
	Entity* entity,
	std::string& projectDir,
	std::string& sceneId,
	nlohmann::json& currentSceneJson,
	bool saveScene)
{
	//If the entity has a parent, remove it from the parent's children
	Entity* parent = entity->GetEntityParams()->parent.get();
	if (parent) {
		auto& children = parent->GetEntityParams()->children;
		children.erase(std::remove_if(children.begin(), children.end(), [&](std::shared_ptr<Entity>& child) {
			return child->GetEntityParams()->id == entity->GetEntityParams()->id;
			}), children.end());
	}

	//Recursive removing function to delete all children of the entity
	auto& children = entity->GetEntityParams()->children;
	for (auto& child : children) {
		RemoveEntity(child.get(), projectDir, sceneId, currentSceneJson, false);
	}

	//Get script id
	std::string scriptId = "";
	if (entity->GetEntityParams()->script)
		scriptId = entity->GetEntityParams()->script->scriptId;

	if (!scriptId.empty()) {
		std::string scriptsDir = projectDir + "scripts/";
		std::string scriptDir = scriptsDir + scriptId + "/";

		std::filesystem::remove_all(scriptDir);
	}

	//Get the id of the entity
	std::string entityId = entity->GetEntityParams()->id;

	//Remove the entity from the scene
	entity->DeleteEntity();

	//Delete entity directory
	std::string entitiesDir = projectDir + "entities/";
	std::string entityDir = entitiesDir + entityId + "/";
	std::filesystem::remove_all(entityDir);

	//Save the scene
	if (saveScene) {
		//The scene will no longer have the entity
		auto& entitiesJson = currentSceneJson["entities"];

		auto iter = std::find(entitiesJson.begin(), entitiesJson.end(), entityId);
		if (iter != entitiesJson.end())
			entitiesJson.erase(iter);

		AssetSaver::SaveSceneToFile(currentSceneJson, projectDir, sceneId);
	}

	//Log
	std::string str = "Removed entity \"";
	str += entityId;
	str += "\"";
	Logger::Log("P", str.c_str());
	return true;
}