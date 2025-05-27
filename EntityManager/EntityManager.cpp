#include "pch.h"
#include "EntityManager.h"
#include "Graphics.h"

/*
PURPOSE: Initialize the entity manager
*/
bool EntityManager::InitEntityManager()
{
	Logger::Log("P", "Initalizing entity manager");

	//Load glad
	int version = gladLoadGL();
	if (version == 0) {
		Logger::Log("E", "Starting engine failed in gladLoadGLLoader");
		return false;
	}
	return true;
}

/*
PURPOSE: Calculates all transforms of each lights,
	creates depth maps for lights,
	draws all entities and lights
*/
void EntityManager::DrawEntities(int window_width, int window_height, glm::vec3 currentSceneCameraPos, bool is3D)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //TODO: Will have changed to each object type
	int numLights = 0;
	if (is3D) {
		//Calculate all light tranforms
		for (auto& entityIter : entities) {
			auto entity = entityIter.second.get();
			auto params = entity->GetEntityParams();

			glm::mat4 lightProjection;
			glm::mat4 lightView;

			if (params->GetType() == "DirectionalLight") {
				auto casted = dynamic_cast<DirectionalLight*>(entity);
				if (casted) {
					//Use DEPTH shader which is for directional and spot lights
					ShaderManager::GetInstance().UseShaders(DEPTH);

					//Calculate light martix
					glm::vec3 lightDir = glm::normalize(casted->realRot);
					float width = 100.0f;
					float near_plane = 1.0f, far_plane = 1000.0f;
					lightProjection = glm::ortho(-width, width, -width, width, near_plane, far_plane);
					//lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);
					lightView = glm::lookAt(-lightDir * 100.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

					casted->lightSpaceMatrix = lightProjection * lightView;

					numLights++;
				}
			}
			else if (params->GetType() == "PointLight") {
				auto casted = dynamic_cast<PointLight*>(entity);
				if (casted) {
					//Calculate light parameters and matrices
					float aspect = 1.0f;
					float near_plane = 0.1f;
					float far_plane = 1000.0f;

					glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);
					glm::vec3 pos = casted->realPos;

					std::vector<glm::mat4> shadowTransforms;
					shadowTransforms.reserve(6);
					shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
					shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
					shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
					shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
					shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
					shadowTransforms.emplace_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));

					//Use DEPTH_CUBE shader which is for point lights and send the parameters and matrices to it
					ShaderManager::GetInstance().UseShaders(DEPTH_CUBE); //For point lights

					for (unsigned int face = 0; face < 6; ++face) {
						//Each face matrices
						std::string uniformName = "shadowMatrices[" + std::to_string(face) + "]";
						ShaderManager::GetInstance().ApplyUniformMatrix(uniformName.c_str(), shadowTransforms[face]);
					}

					//Parameters
					ShaderManager::GetInstance().ApplyUniformVec3("lightPos", pos);
					ShaderManager::GetInstance().ApplyUniformFloat("far_plane", far_plane);

					numLights++;

					casted->far_plane = far_plane;
				}
			}
			else if (params->GetType() == "SpotLight") {
				auto casted = dynamic_cast<SpotLight*>(entity);
				auto castedParams = dynamic_cast<SpotLightParams*>(params);
				if (casted && castedParams) {
					float aspect = 1.0f;
					float near_plane = 0.1f;
					float far_plane = 1000.0f;
					//Use DEPTH shader which is for directional and spot lights
					ShaderManager::GetInstance().UseShaders(DEPTH);

					//Calculate light martix
					glm::vec3 spotPos = casted->realPos;
					glm::vec3 spotDir = glm::normalize(casted->realRot);

					std::cout << spotDir.x << std::endl;

					float fov = glm::degrees(2 * acos(castedParams->outerCutOff));
					lightProjection = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);

					lightView = glm::lookAt(spotPos, spotPos + spotDir, glm::vec3(0.0f, 1.0f, 0.0f));

					casted->lightSpaceMatrix = lightProjection * lightView;

					numLights++;
				}
			}
		}
		/*
			Create a depth buffer for each light.
			Point light has a different calculation.
			The frame buffer will create a texture of depth buffer.
			The texture will be used in 3d shaders.
		*/


		int index = 0;
		for (auto& entityIter : entities) {
			auto entity = entityIter.second.get();
			auto params = entity->GetEntityParams();



			std::string idx = "lights[" + std::to_string(index) + "]";

			const unsigned int SHADOW_WIDTH = 8196, SHADOW_HEIGHT = 8196;
			if (params->GetType() == "PointLight") {
				auto casted = dynamic_cast<PointLight*>(entity);
				auto castedParams = dynamic_cast<PointLightParams*>(params);
				if (casted && castedParams) {
					//Use right shaders
					ShaderManager::GetInstance().UseShaders(DEPTH_CUBE);
					glBindFramebuffer(GL_FRAMEBUFFER, casted->depthMapFBO);
					glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					glClear(GL_DEPTH_BUFFER_BIT);
					glCullFace(GL_FRONT);

					for (auto& entity : entities) {
						entity.second->Draw(currentSceneCameraPos);
					}
					glCullFace(GL_BACK);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);


					//Use SHADER_3D shaders which is for 3d drawing
					ShaderManager::GetInstance().UseShaders(SHADER_3D);

					//Send light parameters to drawing shader, some lights don't have some parameters
					ShaderManager::GetInstance().ApplyUniformInt("numLights", numLights);


					std::string idxType = idx + ".type";
					ShaderManager::GetInstance().ApplyUniformInt(idxType.c_str(), (int)1);


					std::string idxPos = idx + ".position";
					ShaderManager::GetInstance().ApplyUniformVec3(idxPos.c_str(), casted->realPos);
					std::string idxColor = idx + ".color";
					ShaderManager::GetInstance().ApplyUniformVec3(idxColor.c_str(), castedParams->color);

					std::string idxConstant = idx + ".constant";
					ShaderManager::GetInstance().ApplyUniformFloat(idxConstant.c_str(), castedParams->constant);
					std::string idxLinear = idx + ".linear";
					ShaderManager::GetInstance().ApplyUniformFloat(idxLinear.c_str(), castedParams->linear);
					std::string idxQuadratic = idx + ".quadratic";
					ShaderManager::GetInstance().ApplyUniformFloat(idxQuadratic.c_str(), castedParams->quadratic);

					index++;
				}
			}
			else if (params->GetType() == "SpotLight") {
				auto casted = dynamic_cast<SpotLight*>(entity);
				auto castedParams = dynamic_cast<SpotLightParams*>(params);
				if (casted && castedParams) {
					//Use right shaders
					ShaderManager::GetInstance().UseShaders(DEPTH);
					glBindFramebuffer(GL_FRAMEBUFFER, casted->depthMapFBO);
					glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					glClear(GL_DEPTH_BUFFER_BIT);
					glCullFace(GL_FRONT);
					//Send the light matrix to the shader
					ShaderManager::GetInstance().ApplyUniformMatrix("lightSpaceMatrix", casted->lightSpaceMatrix);
					for (auto& entity : entities) {
						entity.second->Draw(currentSceneCameraPos);
					}
					glCullFace(GL_BACK);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);

					//Use SHADER_3D shaders which is for 3d drawing
					ShaderManager::GetInstance().UseShaders(SHADER_3D);

					//Send light parameters to drawing shader, some lights don't have some parameters
					ShaderManager::GetInstance().ApplyUniformInt("numLights", numLights);


					std::string idxType = idx + ".type";
					ShaderManager::GetInstance().ApplyUniformInt(idxType.c_str(), (int)2);


					std::string idxPos = idx + ".position";
					ShaderManager::GetInstance().ApplyUniformVec3(idxPos.c_str(), casted->realPos);
					std::string idxDirection = idx + ".direction";
					ShaderManager::GetInstance().ApplyUniformVec3(idxDirection.c_str(), casted->realRot);
					std::string idxColor = idx + ".color";
					ShaderManager::GetInstance().ApplyUniformVec3(idxColor.c_str(), castedParams->color);

					std::string idxCutOff = idx + ".cutOff";
					ShaderManager::GetInstance().ApplyUniformFloat(idxCutOff.c_str(), castedParams->cutOff);
					std::string idxOuterCutOff = idx + ".outerCutOff";
					ShaderManager::GetInstance().ApplyUniformFloat(idxOuterCutOff.c_str(), castedParams->outerCutOff);

					std::string idxConstant = idx + ".constant";
					ShaderManager::GetInstance().ApplyUniformFloat(idxConstant.c_str(), castedParams->constant);
					std::string idxLinear = idx + ".linear";
					ShaderManager::GetInstance().ApplyUniformFloat(idxLinear.c_str(), castedParams->linear);
					std::string idxQuadratic = idx + ".quadratic";
					ShaderManager::GetInstance().ApplyUniformFloat(idxQuadratic.c_str(), castedParams->quadratic);

					index++;

				}
			}
			else if (params->GetType() == "DirectionalLight") {
				auto casted = dynamic_cast<DirectionalLight*>(entity);
				auto castedParams = dynamic_cast<DirectionalLightParams*>(params);
				if (casted && castedParams) {
					//Use right shaders
					ShaderManager::GetInstance().UseShaders(DEPTH);
					glBindFramebuffer(GL_FRAMEBUFFER, casted->depthMapFBO);
					glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					glClear(GL_DEPTH_BUFFER_BIT);
					glCullFace(GL_FRONT);
					//Send the light matrix to the shader
					ShaderManager::GetInstance().ApplyUniformMatrix("lightSpaceMatrix", casted->lightSpaceMatrix);
					for (auto& entity : entities) {
						entity.second->Draw(currentSceneCameraPos);
					}
					glCullFace(GL_BACK);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);

					//Use SHADER_3D shaders which is for 3d drawing
					ShaderManager::GetInstance().UseShaders(SHADER_3D);

					//Send light parameters to drawing shader, some lights don't have some parameters
					ShaderManager::GetInstance().ApplyUniformInt("numLights", numLights);


					std::string idxType = idx + ".type";
					ShaderManager::GetInstance().ApplyUniformInt(idxType.c_str(), (int)0);

					std::string idxDirection = idx + ".direction";
					ShaderManager::GetInstance().ApplyUniformVec3(idxDirection.c_str(), casted->realRot);
					std::string idxColor = idx + ".color";
					ShaderManager::GetInstance().ApplyUniformVec3(idxColor.c_str(), castedParams->color);

					index++;

				}
			}
			
		}

	}

	if (is3D) {
		//Use SHADER_3D shaders which is for 3d drawing
		ShaderManager::GetInstance().UseShaders(SHADER_3D);


		int index = 0;
		for (auto& entityIter : entities) {
			auto entity = entityIter.second.get();
			auto params = entity->GetEntityParams();
			/*
				Send each light parameters, matrices and shadow maps to the shader,
				This shader will use them.
				This sending is for showing lights.
				The sending above is for creating depth buffer
			*/
			if (params->GetType() == "PointLight") {
				auto casted = dynamic_cast<PointLight*>(entity);
				if (casted) {
					//Bind depthMap as a texture
					GLuint texUnit = 10 + index; //Add an offset to prevent any conflict

					glActiveTexture(GL_TEXTURE0 + texUnit);
					glBindTexture(GL_TEXTURE_CUBE_MAP, casted->depthMap);

					//Send some stuff to the shader
					std::string samplerName = "shadowCubeMaps[" + std::to_string(index) + "]";
					ShaderManager::GetInstance().ApplyUniformInt(samplerName.c_str(), texUnit);

					std::string farPlaneName = "far_planes[" + std::to_string(index) + "]";
					ShaderManager::GetInstance().ApplyUniformFloat(farPlaneName.c_str(), casted->far_plane);

					index++;
				}
			}
			else if (params->GetType() == "DirectionalLight") {
				auto casted = dynamic_cast<DirectionalLight*>(entity);
				if (casted) {
					//Bind depthMaps as a texture
					glActiveTexture(GL_TEXTURE0 + index);//Each light has unique texture unit
					glBindTexture(GL_TEXTURE_2D, casted->depthMap);

					//Send some stuff to the shader
					std::string idxLightSpaceMatrices = "lightSpaceMatrices[" + std::to_string(index) + "]";
					ShaderManager::GetInstance().ApplyUniformMatrix(idxLightSpaceMatrices.c_str(), casted->lightSpaceMatrix);
					std::string idxShadowMaps = "shadowMaps[" + std::to_string(index) + "]";
					ShaderManager::GetInstance().ApplyUniformInt(idxShadowMaps.c_str(), index);

					index++;
				}
			}
			else if (params->GetType() == "SpotLight") {
				auto casted = dynamic_cast<SpotLight*>(entity);
				if (casted) {
					//Bind depthMaps as a texture
					glActiveTexture(GL_TEXTURE0 + index);//Each light has unique texture unit
					glBindTexture(GL_TEXTURE_2D, casted->depthMap);

					//Send some stuff to the shader
					std::string idxLightSpaceMatrices = "lightSpaceMatrices[" + std::to_string(index) + "]";
					ShaderManager::GetInstance().ApplyUniformMatrix(idxLightSpaceMatrices.c_str(), casted->lightSpaceMatrix);
					std::string idxShadowMaps = "shadowMaps[" + std::to_string(index) + "]";
					ShaderManager::GetInstance().ApplyUniformInt(idxShadowMaps.c_str(), index);

					index++;
				}
			}
		}
	}
	else {
		//Use SHADER_2D shaders which is for 2d drawing
		ShaderManager::GetInstance().UseShaders(SHADER_2D);
	}

	/*
		Start to draw.
		Draw to a frame buffer.
		This frame buffer will create a texture.
		This texture will be used by ImGui as a image to draw the scene
	*/
	Graphics::GetInstance().BindFramebuffer();
	Graphics::GetInstance().Clear();
	glViewport(0, 0, window_width, window_height);

	//Main drawing
	for (auto& entity : entities) {
		entity.second->Draw(currentSceneCameraPos);
	}

	//Disable the frame buffer to draw ImGui image
	Graphics::GetInstance().UnbindFramebuffer();

	//Clear all sets
	for (int i = 0; i < numLights; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

/*
PURPOSE: Performs all of deleting entity actions and updates all entities
*/
void EntityManager::UpdateEntities(
	bool windowSceneFocused,
	bool windowSceneDeletePressed,
	bool& pendingDelete,
	std::string selectedId,
	std::function<void()> selectFunction,
	std::string& projectDir,
	std::string& sceneId,
	nlohmann::json& currentSceneJson)
{
	//Perform deleting entity actions
	if (windowSceneFocused && windowSceneDeletePressed) {
		RemoveEntity(selectedId, projectDir, sceneId, currentSceneJson);
		selectFunction();
	}
	if (pendingDelete) {
		RemoveEntity(selectedId, projectDir, sceneId, currentSceneJson);
		selectFunction();
		pendingDelete = false;
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
	}
}

/*
PURPOSE: Sets the real pos, rot, sca of the entity depends on its parent,
	for example, if the parent x=10, the child x=5, the entity will be shown at x=15 in the space
*/
ENTITYMANAGER_API void EntityManager::SetEntityRealStats(Entity* entity)
{
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
	for (auto& child : params->children) {
		SetEntityRealStats(child.get());
	}
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
	std::shared_ptr<Entity>& parent,
	bool newEntity)
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
	std::string scenesDir = projectDir + "scenes/";
	std::filesystem::create_directory(scenesDir);
	std::string sceneDir = scenesDir + sceneId + "/";
	std::filesystem::create_directory(sceneDir);
	std::string sceneFile = sceneDir + sceneId + ".adrenginescene";

	for (auto& entityIter : GetEntities()) {
		//Save each entity
		Entity* entity = entityIter.second.get();
		if (!entity) continue;

		auto params = entity->GetEntityParams();
		if (!params || params->id.empty()) continue;

		//std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
		std::string entitiesDir = projectDir + "entities/";
		std::filesystem::create_directory(entitiesDir);

		std::string entityDir = entitiesDir + params->id + "/";
		std::filesystem::create_directory(entityDir);

		std::string entityFile = entityDir + params->id + ".adrengineentity";
		AssetSaver::SaveEntityToFile(entity->ToJson(), entityFile);
	}

	currentSceneJson["entities"].push_back(params->id); //The scene will have the new entity

	AssetSaver::SaveSceneToFile(currentSceneJson, sceneFile, projectDir);

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
	std::string which,
	std::string& projectDir,
	std::string& sceneId,
	nlohmann::json& currentSceneJson,
	bool saveScene)
{
	//Check if the entity exists
	auto entityIter = entities.find(which);

	if (entityIter == entities.end()) {
		std::string str = "There is not any entity which has given id \"";
		str += which;
		str += "\"";
		Logger::Log("E", str.c_str());
		return false;
	}

	auto entity = entityIter->second.get();

	//If the entity has a parent, remove it from the parent's children
	auto parent = entity->GetEntityParams()->parent.get();
	if (parent) {
		auto& children = parent->GetEntityParams()->children;
		children.erase(std::remove_if(children.begin(), children.end(), [&](std::shared_ptr<Entity>& child) {
			return child->GetEntityParams()->id == entity->GetEntityParams()->id;
			}), children.end());
	}

	//Recursive removing function to delete all children of the entity
	auto& children = entity->GetEntityParams()->children;
	for (auto& child : children) {
		RemoveEntity(child->GetEntityParams()->id, projectDir, sceneId, currentSceneJson, false);
	}

	//Remove the entity from the scene
	entity->DeleteEntity();
	entities.erase(entityIter);

	//Delete entity directory
	std::string entitiesDir = projectDir + "entities/";
	std::string entityDir = entitiesDir + which + "/";
	std::filesystem::remove_all(entityDir);

	//Save the scene
	if (saveScene) {
		std::string scenesDir = projectDir + "scenes/";
		std::filesystem::create_directory(scenesDir);
		std::string sceneDir = scenesDir + sceneId + "/";
		std::filesystem::create_directory(sceneDir);
		std::string sceneFile = sceneDir + sceneId + ".adrenginescene";

		//The scene will no longer have the entity
		auto& entitiesJson = currentSceneJson["entities"];

		auto entityIter = entitiesJson.find(which);
		if (entityIter != entitiesJson.end()) {
			entitiesJson.erase(entitiesJson);
		}

		AssetSaver::SaveSceneToFile(currentSceneJson, sceneFile, projectDir);
	}

	//Log
	std::string str = "Removed entity \"";
	str += which;
	str += "\"";
	Logger::Log("P", str.c_str());
	return true;
}