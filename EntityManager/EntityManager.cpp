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
	if (is3D) {
		//Calculate all light tranforms
		for (int i = 0; i < lights.size(); ++i) {
			glm::mat4 lightProjection;
			glm::mat4 lightView;
			glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

			if (lights[i].type == LIGHT_DIRECTIONAL) {
				//Use DEPTH shader which is for directional and spot lights
				ShaderManager::GetInstance().UseShaders(DEPTH);

				//Calculate light martix
				glm::vec3 lightDir = glm::normalize(lights[0].direction);
				float near_plane = 1.0f, far_plane = 1000.0f;
				lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
				//lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, near_plane, far_plane);
				lightView = glm::lookAt(-lightDir * 100.0f, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				lightSpaceMatrix = lightProjection * lightView;
				lights[i].lightSpaceMatrix = lightSpaceMatrix;
			}
			else if (lights[i].type == LIGHT_POINT) {
				//Calculate light parameters and matrices
				float aspect = 1.0f;
				float near_plane = 0.1f;
				float far_plane = 1000.0f;
				float fov = glm::radians(90.0f);// 90 degrees for each direction

				glm::mat4 shadowProj = glm::perspective(fov, aspect, near_plane, far_plane);
				glm::vec3 pos = lights[i].position;

				std::vector<glm::mat4> shadowTransforms;
				shadowTransforms.push_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
				shadowTransforms.push_back(shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));

				lights[i].shadowTransforms = shadowTransforms;
				lights[i].far_plane = far_plane;

				//Use DEPTH_CUBE shader which is for point lights and send the parameters and matrices to it
				ShaderManager::GetInstance().UseShaders(DEPTH_CUBE); // point light için olan

				for (unsigned int face = 0; face < 6; ++face) {
					//Each face matrices
					std::string uniformName = "shadowMatrices[" + std::to_string(face) + "]";
					ShaderManager::GetInstance().ApplyUniformMatrix(uniformName.c_str(), lights[i].shadowTransforms[face]);
				}

				//Parameters
				ShaderManager::GetInstance().ApplyUniformVec3("lightPos", lights[i].position);
				ShaderManager::GetInstance().ApplyUniformFloat("far_plane", far_plane);
			}
			else if (lights[i].type == LIGHT_SPOT) {
				//Use DEPTH shader which is for directional and spot lights
				ShaderManager::GetInstance().UseShaders(DEPTH);

				//Calculate light martix
				glm::vec3 spotPos = lights[0].position;
				glm::vec3 spotDir = glm::normalize(lights[0].direction);
				float fov = glm::degrees(2 * acos(lights[0].outerCutOff));
				lightProjection = glm::perspective(glm::radians(fov), 1.0f, 1.0f, 1000.0f);
				lightView = glm::lookAt(spotPos, spotPos + spotDir, glm::vec3(0.0f, 1.0f, 0.0f));
				lightSpaceMatrix = lightProjection * lightView;
				lights[i].lightSpaceMatrix = lightSpaceMatrix;
			}

			/*
				Create a depth buffer for each light.
				Point light has a different calculation.
				The frame buffer will create a texture of depth buffer.
				The texture will be used in 3d shaders.
			*/
			const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
			if (lights[i].type == LIGHT_POINT) {
				//Use right shaders
				ShaderManager::GetInstance().UseShaders(DEPTH_CUBE);
				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, lights[i].depthMapFBO);
				glClear(GL_DEPTH_BUFFER_BIT);
				glCullFace(GL_FRONT);

				for (auto& entity : entities) {
					entity.second->Draw(currentSceneCameraPos);  // model matris gönderdiðinden emin ol
				}
				glCullFace(GL_BACK);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else {
				//Use right shaders
				ShaderManager::GetInstance().UseShaders(DEPTH);
				glBindFramebuffer(GL_FRAMEBUFFER, lights[i].depthMapFBO);
				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glClear(GL_DEPTH_BUFFER_BIT);
				glCullFace(GL_FRONT);
				//Send the light matrix to the shader
				ShaderManager::GetInstance().ApplyUniformMatrix("lightSpaceMatrix", lightSpaceMatrix);
				for (auto& entity : entities) {
					entity.second->Draw(currentSceneCameraPos);
				}
				glCullFace(GL_BACK);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}

		//Use SHADER_3D shaders which is for 3d drawing
		ShaderManager::GetInstance().UseShaders(SHADER_3D);

		//Send each light parameters to drawing shader, some lights don't have some parameters
		ShaderManager::GetInstance().ApplyUniformInt("numLights", (int)lights.size());
		for (int i = 0; i < lights.size(); ++i) {
			std::string idx = "lights[" + std::to_string(i) + "]";
			std::string idxType = idx + ".type";
			ShaderManager::GetInstance().ApplyUniformInt(idxType.c_str(), (int)lights[i].type);

			std::string idxPos = idx + ".position";
			ShaderManager::GetInstance().ApplyUniformVec3(idxPos.c_str(), lights[i].position);
			std::string idxDirection = idx + ".direction";
			ShaderManager::GetInstance().ApplyUniformVec3(idxDirection.c_str(), glm::normalize(lights[i].direction));
			std::string idxColor = idx + ".color";
			ShaderManager::GetInstance().ApplyUniformVec3(idxColor.c_str(), lights[i].color);

			std::string idxCutOff = idx + ".cutOff";
			ShaderManager::GetInstance().ApplyUniformFloat(idxCutOff.c_str(), lights[i].cutOff);
			std::string idxOuterCutOff = idx + ".outerCutOff";
			ShaderManager::GetInstance().ApplyUniformFloat(idxOuterCutOff.c_str(), lights[i].outerCutOff);

			std::string idxConstant = idx + ".constant";
			ShaderManager::GetInstance().ApplyUniformFloat(idxConstant.c_str(), lights[i].constant);
			std::string idxLinear = idx + ".linear";
			ShaderManager::GetInstance().ApplyUniformFloat(idxLinear.c_str(), lights[i].linear);
			std::string idxQuadratic = idx + ".quadratic";
			ShaderManager::GetInstance().ApplyUniformFloat(idxQuadratic.c_str(), lights[i].quadratic);
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

	if (is3D) {
		for (int i = 0; i < lights.size(); ++i) {
			/*
				Send each light parameters, matrices and shadow maps to the shader,
				This shader will use them.
				This sending is for showing lights.
				The sending above is for creating depth buffer
			*/
			if (lights[i].type == LIGHT_POINT) {
				//Bind depthMap as a texture
				GLuint texUnit = 10 + i; //Add an offset to prevent any conflict

				glActiveTexture(GL_TEXTURE0 + texUnit);
				glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i].depthMap);

				//Send some stuff to the shader
				std::string samplerName = "shadowCubeMaps[" + std::to_string(i) + "]";
				ShaderManager::GetInstance().ApplyUniformInt(samplerName.c_str(), texUnit);

				std::string farPlaneName = "far_planes[" + std::to_string(i) + "]";
				ShaderManager::GetInstance().ApplyUniformFloat(farPlaneName.c_str(), lights[i].far_plane);
			}
			else {
				//Bind depthMaps as a texture
				glActiveTexture(GL_TEXTURE0 + i);//Each light has unique texture unit
				glBindTexture(GL_TEXTURE_2D, lights[i].depthMap);

				//Send some stuff to the shader
				std::string idxLightSpaceMatrices = "lightSpaceMatrices[" + std::to_string(i) + "]";
				ShaderManager::GetInstance().ApplyUniformMatrix(idxLightSpaceMatrices.c_str(), lights[i].lightSpaceMatrix);
				std::string idxShadowMaps = "shadowMaps[" + std::to_string(i) + "]";
				ShaderManager::GetInstance().ApplyUniformInt(idxShadowMaps.c_str(), i);
			}
		}
	}

	//Main drawing
	for (auto& entity : entities) {
		entity.second->Draw(currentSceneCameraPos);
	}

	//Disable the frame buffer to draw ImGui image
	Graphics::GetInstance().UnbindFramebuffer();

	//Clear all sets
	for (int i = 0; i < lights.size(); ++i) {
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
		auto params = entity.second->GetEntityParams();
		auto parent = params->parent.get();
		if (parent) {
			entity.second->realPos = parent->realPos;
			entity.second->realRot = parent->realRot;
			entity.second->realSca = parent->realSca;
		}
	}

	//Update all entities
	for (auto& entity : entities) {
		entity.second->Update();
	}

	//Setup lights for the first frame and any differences in entities
	size_t currentEntitySize = entities.size();

	if (currentEntitySize != lastEntitySize) {
		lastEntitySize = currentEntitySize;

		SetLights();
	}

	//Update all lights
	for (auto& entity : entities) {
		if (entity.second->GetEntityParams()->GetType() == "DirectionalLight") {
			auto casted = dynamic_cast<DirectionalLightParams*>(entity.second->GetEntityParams());
			if (casted) {
				for (auto& light : lights) {
					if (light.type == LIGHT_DIRECTIONAL && light.id == casted->id) {
						light.direction = glm::normalize(casted->direction);
						light.color = glm::normalize(casted->color);
					}
				}
			}
		}
		else if(entity.second->GetEntityParams()->GetType() == "SpotLight") {
			auto casted = dynamic_cast<SpotLightParams*>(entity.second->GetEntityParams());
			if (casted) {
				for (auto& light : lights) {
					if (light.type == LIGHT_SPOT && light.id == casted->id) {
						light.direction = glm::normalize(casted->direction);
						light.color = glm::normalize(casted->color);
						light.position = entity.second->realPos;
						light.cutOff = casted->cutOff;
						light.outerCutOff = casted->outerCutOff;
						light.constant = casted->constant;
						light.linear = casted->linear;
						light.quadratic = casted->quadratic;
					}
				}
			}
		}
		else if (entity.second->GetEntityParams()->GetType() == "PointLight") {
			auto casted = dynamic_cast<PointLightParams*>(entity.second->GetEntityParams());
			if (casted) {
				for (auto& light : lights) {
					if (light.type == LIGHT_POINT && light.id == casted->id) {
						light.position = entity.second->realPos;
						light.color = casted->color;
						light.constant = casted->constant;
						light.linear = casted->linear;
						light.quadratic = casted->quadratic;
					}
				}
			}
		}
	}
}

/*
PURPOSE: Releases previous lights and creates new ones for any changings on entities
	If user creates a new directional light, set the lights
*/
ENTITYMANAGER_API void EntityManager::SetLights()
{
	//Clear previous data
	for (int i = 0; i < lights.size(); ++i) {
		glDeleteFramebuffers(1, &lights[i].depthMapFBO);
		glDeleteTextures(1, &lights[i].depthMap);
	}
	lights.clear();

	//Create new lights and push it to all lights
	for (auto& entity : entities) {
		if (entity.second->GetEntityParams()->GetType() == "DirectionalLight") {
			auto casted = dynamic_cast<DirectionalLightParams*>(entity.second->GetEntityParams());
			if (casted) {
				Light light;
				light.id = entity.second->GetEntityParams()->id;
				light.type = LIGHT_DIRECTIONAL;
				light.direction = glm::normalize(casted->direction);
				light.color = glm::normalize(casted->color);

				glGenFramebuffers(1, &light.depthMapFBO);
				const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;

				glGenTextures(1, &light.depthMap);
				glBindTexture(GL_TEXTURE_2D, light.depthMap);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
					SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

				glBindFramebuffer(GL_FRAMEBUFFER, light.depthMapFBO);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light.depthMap, 0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				lights.push_back(light);
			}
		}
		else if (entity.second->GetEntityParams()->GetType() == "SpotLight") {
			auto casted = dynamic_cast<SpotLightParams*>(entity.second->GetEntityParams());
			if (casted) {
				auto castedEntity = std::dynamic_pointer_cast<SpotLight>(entity.second);
				Light light;
				light.type = LIGHT_SPOT;
				light.id = entity.second->GetEntityParams()->id;
				light.direction = glm::normalize(casted->direction);
				light.color = glm::normalize(casted->color);
				if (castedEntity.get())
					light.position = castedEntity->realPos;
				light.cutOff = casted->cutOff;
				light.outerCutOff = casted->outerCutOff;
				light.constant = casted->constant;
				light.linear = casted->linear;
				light.quadratic = casted->quadratic;

				const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;

				glGenTextures(1, &light.depthMap);
				glBindTexture(GL_TEXTURE_2D, light.depthMap);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
					SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

				glGenFramebuffers(1, &light.depthMapFBO);
				glBindFramebuffer(GL_FRAMEBUFFER, light.depthMapFBO);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light.depthMap, 0);
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				lights.push_back(light);
			}
		}
		else if (entity.second->GetEntityParams()->GetType() == "PointLight") {
			auto casted = dynamic_cast<PointLightParams*>(entity.second->GetEntityParams());
			if (casted) {
				Light light;
				light.type = LIGHT_POINT;
				light.id = entity.second->GetEntityParams()->id;
				light.position = entity.second->realPos;
				light.color = casted->color;
				light.constant = casted->constant;
				light.linear = casted->linear;
				light.quadratic = casted->quadratic;

				glGenFramebuffers(1, &light.depthMapFBO);
				const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;

				glGenTextures(1, &light.depthMap);
				glBindTexture(GL_TEXTURE_CUBE_MAP, light.depthMap);
				for (GLuint i = 0; i < 6; ++i) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
						SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
				}

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				/*GLenum err;
				while ((err = glGetError()) != GL_NO_ERROR) {
					std::cout << "OpenGL error: " << err << std::endl;
				}*/

				glBindFramebuffer(GL_FRAMEBUFFER, light.depthMapFBO);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, light.depthMap, 0); // <- Tüm yüzler için
				glDrawBuffer(GL_NONE);
				glReadBuffer(GL_NONE);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				lights.push_back(light);
			}
		}
	}
}

/*
PURPOSE: Releases all manager stuff
*/
void EntityManager::ReleaseEntityManager()
{
	//Release lights
	lastEntitySize = (size_t)0;
	lights.clear();

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
