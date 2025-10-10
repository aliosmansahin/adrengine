#include "pch.h"
#include "EntityManager.h"
#include "Graphics.h"

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
			auto& entity = entityIter.second;
			auto params = entity->GetEntityParams();


			if (params->GetType() == "SpotLight") {
				auto casted = std::dynamic_pointer_cast<ISpotLight>(entity);
				auto castedParams = std::dynamic_pointer_cast<ISpotLightParams>(params);
				if (casted && castedParams) {
					casted->SetIndex(numLights);//Set the index of the light. For example, 0. index of all lights

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
				auto casted = std::dynamic_pointer_cast<IDirectionalLight>(entity);
				auto castedParams = std::dynamic_pointer_cast<IDirectionalLightParams>(params);
				if (casted && castedParams) {
					casted->SetIndex(numLights);//Set the index of the light. For example, 0. index of all lights

					//Use right shaders
					ShaderManager::GetInstance().UseShaders(Utils::DEPTH);
					adr_glBindFramebuffer(GL_FRAMEBUFFER, casted->GetDepthMapFBO());

					adr_glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
					adr_glClear(GL_DEPTH_BUFFER_BIT);
					adr_glCullFace(GL_FRONT);
					//Send the light matrix to the shader
					ShaderManager::GetInstance().ApplyUniformMatrix("lightSpaceMatrix", casted->GetLightSpaceMatrix());
					for (auto& entity : entities) {
						entity.second->Draw(currentSceneCameraPos);
					}
					adr_glCullFace(GL_BACK);
					adr_glBindFramebuffer(GL_FRAMEBUFFER, 0);

					numLights++;
				}
			}
			else if (params->GetType() == "PointLight") {
				auto casted = std::dynamic_pointer_cast<IPointLight>(entity);
				auto castedParams = std::dynamic_pointer_cast<IPointLightParams>(params);
				if (casted && castedParams) {
					casted->SetIndex(numLights); //Set the index of the light. For example, 0. index of all lights
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
			auto& entity = entityIter.second;
			auto params = entity->GetEntityParams();

			if (params->GetType() == "SpotLight") {
				auto casted = std::dynamic_pointer_cast<ISpotLight>(entity);
				auto castedParams = std::dynamic_pointer_cast<ISpotLightParams>(params);
				if (casted && castedParams) {
					int i = casted->GetIndex();

					//Set light parameters
					std::string base = "lights[" + std::to_string(i) + "]";
					ShaderManager::GetInstance().ApplyUniformInt((base + ".type").c_str(), 2);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".position").c_str(), castedParams->GetPosition());
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".direction").c_str(), glm::normalize(castedParams->GetDirection()));
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".color").c_str(), castedParams->GetColor());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".cutOff").c_str(), castedParams->GetCutOff());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".outerCutOff").c_str(), castedParams->GetOuterCutOff());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".constant").c_str(), castedParams->GetConstant());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".linear").c_str(), castedParams->GetLinear());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".quadratic").c_str(), castedParams->GetQuadratic());

					//Bind depthMap as a texture
					GLuint textureUnit = i + (unsigned int)entities.size(); // 10'dan baþlat: çatýþmayý önler
					adr_glActiveTexture(GL_TEXTURE0 + textureUnit);
					adr_glBindTexture(GL_TEXTURE_2D, casted->GetDepthMap());

					//ShaderManager::GetInstance().ApplyUniformInt(("shadowMaps[" + std::to_string(i) + "]").c_str(), textureUnit);
					ShaderManager::GetInstance().ApplyUniformMatrix(("lightSpaceMatrices[" + std::to_string(i) + "]").c_str(), casted->GetLightSpaceMatrix());
				}
			}

			else if (params->GetType() == "DirectionalLight") {
				auto casted = std::dynamic_pointer_cast<IDirectionalLight>(entity);
				auto castedParams = std::dynamic_pointer_cast<IDirectionalLightParams>(params);
				if (casted && castedParams) {
					int i = casted->GetIndex();

					//Set light parameters
					std::string base = "lights[" + std::to_string(i) + "]";
					ShaderManager::GetInstance().ApplyUniformInt((base + ".type").c_str(), 0);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".direction").c_str(), castedParams->GetDirection());
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".color").c_str(), castedParams->GetColor());

					//Bind depthMap as a texture
					GLuint textureUnit = i + (unsigned int)entities.size();
					adr_glActiveTexture(GL_TEXTURE0 + textureUnit);//Each light has unique texture unit
					adr_glBindTexture(GL_TEXTURE_2D, casted->GetDepthMap());

					//Send some stuff to the shader
					ShaderManager::GetInstance().ApplyUniformMatrix(("lightSpaceMatrices[" + std::to_string(i) + "]").c_str(), casted->GetLightSpaceMatrix());
					ShaderManager::GetInstance().ApplyUniformInt(("shadowMaps[" + std::to_string(i) + "]").c_str(), textureUnit);
				}
			}

			else if (params->GetType() == "PointLight") {
				auto casted = std::dynamic_pointer_cast<IPointLight>(entity);
				auto castedParams = std::dynamic_pointer_cast<IPointLightParams>(params);
				if (casted && castedParams) {
					int i = casted->GetIndex();

					//Set light parameters
					std::string base = "lights[" + std::to_string(i) + "]";
					ShaderManager::GetInstance().ApplyUniformInt((base + ".type").c_str(), 1);
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".position").c_str(), castedParams->GetPosition());
					ShaderManager::GetInstance().ApplyUniformVec3((base + ".color").c_str(), castedParams->GetColor());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".constant").c_str(), castedParams->GetConstant());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".linear").c_str(), castedParams->GetLinear());
					ShaderManager::GetInstance().ApplyUniformFloat((base + ".quadratic").c_str(), castedParams->GetQuadratic());

					//Bind depthMap as a texture
					GLuint textureUnit =  i + (unsigned int)entities.size(); //Add an offset to prevent any conflict

					adr_glActiveTexture(GL_TEXTURE0 + textureUnit);
					adr_glBindTexture(GL_TEXTURE_CUBE_MAP, casted->GetDepthMap());

					//Send some stuff to the shader
					//ShaderManager::GetInstance().ApplyUniformInt(("shadowCubeMaps[" + std::to_string(i) + "]").c_str(), texUnit);
					ShaderManager::GetInstance().ApplyUniformFloat(("far_planes[" + std::to_string(i) + "]").c_str(), casted->GetFarPlane());
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
		std::vector<std::shared_ptr<IEntity>> sortedEntities;

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
		std::sort(sortedEntities.begin(), sortedEntities.end(), [&currentSceneCameraPos](const std::shared_ptr<IEntity>& a, const std::shared_ptr<IEntity>& b) {
			//calculate distance between camera and the object
			float disA = glm::distance(currentSceneCameraPos, a->GetEntityParams()->GetPosition());
			float disB = glm::distance(currentSceneCameraPos, b->GetEntityParams()->GetPosition());

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
		std::vector<std::pair<std::string, std::shared_ptr<IEntity>>> sortedEntities(entities.begin(), entities.end());

		std::sort(sortedEntities.begin(), sortedEntities.end(), [](const std::pair<std::string, std::shared_ptr<IEntity>>& a, const std::pair<std::string, std::shared_ptr<IEntity>>& b) {
			return a.second->GetEntityParams()->GetPosition().z < b.second->GetEntityParams()->GetPosition().z;
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
	bool isPlaying,
	std::string selectedId,
	std::function<void(std::string)> extraDeletingFunc,
	std::string& sceneId,
	nlohmann::json& currentSceneJson,
	std::shared_ptr<ICamera>& gameCamera)
{
	//Perform entity deletion actions
	PerformEntityDeletions(
		windowSceneFocused,
		windowSceneDeletePressed,
		pendingDelete,
		selectedId,
		sceneId,
		currentSceneJson,
		extraDeletingFunc
	);

	/*
		Make entitites to follow their parents.
		Here we are setting the origin of the entity to parent's location in the space if the entity has a parent
	*/
	for (auto& entity : entities) {
		SetEntityRealStats(entity.second);
	}

	//Update all entities
	if (isPlaying) {
		for (auto& entity : entities) {
			entity.second->Update();
			if (entity.second->GetEntityParams()->GetType() == "Camera") {
				std::shared_ptr<ICamera> camera = std::dynamic_pointer_cast<ICamera>(entity.second);
				if (camera) {
					gameCamera = camera;
				}
			}
		}
	}
}

/*
PURPOSE: Sets the real pos, rot, sca of the entity depends on its parent,
	for example, if the parent x=10, the child x=5, the entity will be shown at x=15 in the space
*/
void EntityManager::SetEntityRealStats(std::shared_ptr<IEntity> entity)
{
	//Set real stats
	auto params = entity->GetEntityParams();
	auto parent = params->GetParent();
	if (parent) {
		auto parentParams = parent->GetEntityParams();
		entity->SetTransform(
			parentParams->GetPosition() + params->GetPosition(),
			parentParams->GetRotation() + params->GetRotation(),
			parentParams->GetScale() + params->GetScale()
		);
	}
	else {
		entity->SetTransform(
			params->GetPosition(),
			params->GetRotation(),
			params->GetScale()
		);
	}
	//Do that again to all children
	for (auto& child : params->GetChildren()) {
		SetEntityRealStats(child);
	}
}

/*
PURPOSE: Runs begin nodes of all scripts of entities
*/
ENTITYMANAGER_API void EntityManager::RunEntitiesScriptBegin()
{
	for (auto& entity : entities) {
		if(entity.second->GetEntityParams()->GetVisualScript())
			entity.second->GetEntityParams()->GetVisualScript()->ExecuteBeginScript();
	}
}

/*
PURPOSE: Resets runtime values (position, rotation, physics...)
*/
ENTITYMANAGER_API void EntityManager::ResetEntitiesRuntimeValues()
{
	for (auto& entity : entities) {
		entity.second->GetEntityParams()->ResetRuntimeValues();
		if (entity.second->GetEntityParams()->GetType() == "Object") {
			std::shared_ptr<IObject> object = std::dynamic_pointer_cast<IObject>(entity.second);
			if (object) {
				object->ResetPhysics();
			}
		}
	}
}

/*
PURPOSE: Updates rigidbody transfroms from objects
*/
ENTITYMANAGER_API void EntityManager::SetRigitbodiesFromEntities()
{
	for (auto& entity : entities) {
		std::shared_ptr<IObject> object = std::dynamic_pointer_cast<IObject>(entity.second);
		if (object == nullptr)
			continue;

		glm::vec3 pos = entity.second->GetEntityParams()->GetPosition();
		glm::vec3 rot = entity.second->GetEntityParams()->GetRotation();

		ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetPhysics()->UpdateRigidbodyTransforms(object->GetRigidBody(), pos, rot);
	}
}

/*
PURPOSE: Updates objects transfroms from rigidbodies
*/
ENTITYMANAGER_API void EntityManager::SetEntitiesFromRigidbodies()
{
	for (auto& entity : entities) {
		std::shared_ptr<IObject> object = std::dynamic_pointer_cast<IObject>(entity.second);
		if (object == nullptr)
			continue;

		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 rot = glm::vec3(0.0f);

		ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetPhysics()->UpdateEntityTransforms(object->GetRigidBody(), pos, rot);

		entity.second->GetEntityParams()->SetRuntimePosition(pos);
		entity.second->GetEntityParams()->SetRuntimeRotation(rot);
	}
}

/*
PURPOSE: Returns entity by id, check out getElementById of javascript
*/
ENTITYMANAGER_API std::shared_ptr<IEntity> EntityManager::GetEntityById(std::string id)
{
	auto entityIter = entities.find(id);
	if (entityIter == entities.end())
		return nullptr;

	return entityIter->second;
}

/*
PURPOSE: Performs entity deletions if needed
*/
void EntityManager::PerformEntityDeletions(
	bool windowSceneFocused,
	bool windowSceneDeletePressed,
	bool& pendingDelete,
	std::string selectedId,
	std::string& sceneId,
	nlohmann::json& currentSceneJson,
	std::function<void(std::string)> extraDeletingFunc)
{
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
		if (entityIter->second->GetEntityParams()->GetVisualScript())
			scriptId = entityIter->second->GetEntityParams()->GetVisualScript()->GetScriptId();

		//Remove the entity
		RemoveEntity(entityIter->second, sceneId, currentSceneJson);

		//Erase it
		entities.erase(entityIter);

		//Callback function for deleting tab and select entity to nothing
		extraDeletingFunc(scriptId);

		//Pending delete
		if (pendingDelete) {
			pendingDelete = false;
		}
	}
}

/*
PURPOSE: Releases all manager stuff
*/
void EntityManager::ReleaseEntityManager()
{
	//Release entities
	for (auto& entity : entities) {
		std::shared_ptr<IObject> object = std::dynamic_pointer_cast<IObject>(entity.second);
		if (object != nullptr) {

			std::shared_ptr<IRigidBody> rigidBody = object->GetRigidBody();

			ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetPhysics()->RemoveRigidBody(rigidBody);

			object->SetRigidBody(rigidBody);
		}
		entity.second->DeleteEntity();
	}
	entities.clear();

	//Logger
	Logger::Log("P", "Released entity manager");
}

/*
PURPOSE: Creates a new entity and inserts it to entities
*/
std::string EntityManager::CreateEntity(
	std::string type,
	std::string sceneId,
	nlohmann::json& currentSceneJson,
	std::shared_ptr<IEntity>& parent)
{
	//Set the id
	int index = 0;
	std::string entityId = "";
	while (true) {
		entityId = type + std::to_string(index);
		std::string entitiesDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "entities/";
		std::string entityDir = entitiesDir + entityId;
		if(!std::filesystem::exists(entityDir))
			break;
		++index;
	}

	//Check if the type of the entity exists
	auto typeIter = ServiceLocator::Get<IEngine>()->GetEntityTypes().find(type);
	if (typeIter == ServiceLocator::Get<IEngine>()->GetEntityTypes().end()) {
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
		params->SetParent(parent);
		params->SetParentId(parent->GetEntityParams()->GetId());
	}

	//Set the params
	params->SetId(entityId);
	params->SetName(entityId);

	//Initialize entity with its params
	if (!entity->CreateEntity(params))
		return "";
	
	//Initialize a rigidbody for object
	std::shared_ptr<IObject> object = std::dynamic_pointer_cast<IObject>(entity);
	if (object != nullptr) { //Ensure this is an object
		std::shared_ptr<IRigidBody> rigidBody = object->GetRigidBody();

		ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetPhysics()->AddRigidBody(rigidBody);

		object->SetRigidBody(rigidBody);

		ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetPhysics()->ApplyPropsForRigidBody(rigidBody);
	}

	//Add the entity into its parent's children
	if (parent.get()) {
		parent->GetEntityParams()->GetChildren().push_back(entity);
	}
	
	//Insert the entity into entities
	entities.insert(std::pair<std::string, std::shared_ptr<IEntity>>(entityId, entity));

	//Save the scene and entities which belong to the scene
	for (auto& entityIter : GetEntities()) {
		//Save each entity
		std::shared_ptr<IEntity> entity = entityIter.second;
		if (!entity) continue;

		auto params = entity->GetEntityParams();
		if (!params || params->GetId().empty()) continue;

		AssetSaver::SaveEntityToFile(entity->ToJson(), ServiceLocator::Get<IProject>()->GetProjectDir(), params->GetId());
	}

	currentSceneJson["entities"].push_back(params->GetId()); //The scene will have the new entity

	AssetSaver::SaveSceneToFile(currentSceneJson, ServiceLocator::Get<IProject>()->GetProjectDir(), sceneId);

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
	std::shared_ptr<IEntity> entity,
	std::string& sceneId,
	nlohmann::json& currentSceneJson,
	bool saveScene)
{
	//If the entity has a parent, remove it from the parent's children
	std::shared_ptr<IEntity> parent = entity->GetEntityParams()->GetParent();
	if (parent) {
		auto& children = parent->GetEntityParams()->GetChildren();
		children.erase(std::remove_if(children.begin(), children.end(), [&](std::shared_ptr<IEntity>& child) {
			return child->GetEntityParams()->GetId() == entity->GetEntityParams()->GetId();
			}), children.end());
	}

	//Remove rigidbody for object
	std::shared_ptr<IObject> object = std::dynamic_pointer_cast<IObject>(entity);
	if (object != nullptr) { //Ensure this is an object
		std::shared_ptr<IRigidBody> rigidBody = object->GetRigidBody();

		ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetPhysics()->RemoveRigidBody(rigidBody);

		object->SetRigidBody(rigidBody);
	}

	//Recursive removing function to delete all children of the entity
	auto& children = entity->GetEntityParams()->GetChildren();
	for (auto& child : children) {
		RemoveEntity(child, sceneId, currentSceneJson, false);
	}

	//Get script id
	std::string scriptId = "";
	if (entity->GetEntityParams()->GetVisualScript())
		scriptId = entity->GetEntityParams()->GetVisualScript()->GetScriptId();

	if (!scriptId.empty()) {
		std::string scriptsDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "scripts/";
		std::string scriptDir = scriptsDir + scriptId + "/";

		std::filesystem::remove_all(scriptDir);
	}

	//Get the id of the entity
	std::string entityId = entity->GetEntityParams()->GetId();

	//Remove the entity from the scene
	entity->DeleteEntity();

	//Delete entity directory
	std::string entitiesDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "entities/";
	std::string entityDir = entitiesDir + entityId + "/";
	std::filesystem::remove_all(entityDir);

	//Save the scene
	if (saveScene) {
		//The scene will no longer have the entity
		auto& entitiesJson = currentSceneJson["entities"];

		auto iter = std::find(entitiesJson.begin(), entitiesJson.end(), entityId);
		if (iter != entitiesJson.end())
			entitiesJson.erase(iter);

		AssetSaver::SaveSceneToFile(currentSceneJson, ServiceLocator::Get<IProject>()->GetProjectDir(), sceneId);
	}

	//Log
	std::string str = "Removed entity \"";
	str += entityId;
	str += "\"";
	Logger::Log("P", str.c_str());
	return true;
}

ENTITYMANAGER_API void EntityManager::LoadEntitiesFromJson(
	const nlohmann::json& json,
	std::shared_ptr<IScene> scene)
{
	if (json.contains("entities")) {
		for (auto& entityName : json["entities"]) {
			/* Load Entity */

			//Entity pointers
			std::shared_ptr<IEntity> entity;
			std::shared_ptr<IEntityParams> params;

			//Load entity json
			nlohmann::json entityJson = AssetSaver::LoadEntityFromFile(ServiceLocator::Get<IProject>()->GetProjectDir(), std::string(entityName));
			if (entityJson.is_null())
				continue;

			//Checks for the type
			std::string type = entityJson.value("type", "");
			if (type.empty())
				continue;

			auto& types = ServiceLocator::Get<IEngine>()->GetEntityTypes();
			auto typeIter = types.find(type);
			if (typeIter == types.end())
				continue;

			//Create an entity clone object from entity type
			entity = typeIter->second.first->clone();

			//Create parameter object for the entity
			params = typeIter->second.second->clone();
			entity->CreateEntity(params);

			//TileMap has own fromjson function
			auto tileMap = std::dynamic_pointer_cast<ITileMap>(entity);
			if (tileMap.get()) {
				tileMap->FromJson(entityJson);
			}
			//FlipBook has own fromjson function
			auto flipBook = std::dynamic_pointer_cast<IFlipBook>(entity);
			if (flipBook.get()) {
				flipBook->FromJson(entityJson);
			}

			//Add the entity to entity manager
			if (entity.get())
				entities.insert(std::pair<std::string, std::shared_ptr<IEntity>>(entityJson.value("id", ""), entity));

			if (params.get())
				params->FromJson(entityJson, ServiceLocator::Get<IProject>()->GetProjectDir(), scene);

			//Initialize a rigidbody for object
			std::shared_ptr<IObject> object = std::dynamic_pointer_cast<IObject>(entity);
			if (object != nullptr) { //Ensure this is an object

				std::shared_ptr<IRigidBody> rigidBody = object->GetRigidBody();

				if (scene) {

					scene->GetPhysics()->AddRigidBody(rigidBody);

					object->SetRigidBody(rigidBody);

					//Load RigidBody from json
					if (entityJson.contains("rigid-body")) {
						nlohmann::json rbJson = entityJson["rigid-body"];
						rigidBody->FromJson(rbJson);
					}
					//Apply props
					scene->GetPhysics()->ApplyPropsForRigidBody(rigidBody);
				}
			}
		}
	}
}

/*
PURPOSE: Builds an hierarchy for all entities in the scene
	Uses parent-child relationships
*/
ENTITYMANAGER_API void EntityManager::BuildEntityHierarchy()
{
	for (auto& entity : entities) {
		for (auto& child : entities) {
			if (entity.second->GetEntityParams()->GetId() == child.second->GetEntityParams()->GetParentId()) {
				child.second->GetEntityParams()->GetParent() = entity.second;

				entity.second->GetEntityParams()->GetChildren().push_back(child.second);
			}
		}
	}
}
