#include "pch.h"
#include "Scene.h"

#include "EntityManager.h"
#include "Timer.h"
#include "InputManager.h"

/*
PURPOSE: Sets a scene parameters and creates an entity manager
*/
bool Scene::CreateScene(std::string sceneId, SceneType sceneType)
{
	//Set the scene parameters
	this->sceneId = sceneId;
	this->sceneName = sceneId;
	this->sceneType = sceneType;

	//Create an entity manager
	entityManager = new EntityManager();
	entityManager->InitEntityManager();

	//Some mouse position setups
	firstMouseX = InputManager::GetInstance().GetMouseX();
	firstMouseY = InputManager::GetInstance().GetMouseY();

	return true;
}

/*
PURPOSE: Draws the scene
*/
void Scene::DrawScene(int window_width, int window_height, glm::vec3 currentSceneCameraPos)
{
	//If there is an entity manager, draw each entity via entity manager
	if(entityManager)
		entityManager->DrawEntities(window_width, window_height, currentSceneCameraPos, (sceneType == SCENE_3D));
}

/*
PURPOSE: Update scene objects and handles camera updates
*/
void Scene::UpdateScene(
	bool isPlaying,
	bool windowGameViewportIsHovered,
	int screenWidth,
	int screenHeight,
	int window_width,
	int window_height,
	bool windowSceneFocused,
	bool windowSceneDeletePressed,
	bool& pendingDelete,
	std::string selectedId,
	std::function<void()> selectFunction,
	std::string& projectDir)
{
	//Setups for mouse positions
	int currentMouseX = InputManager::GetInstance().GetMouseX();
	int currentMouseY = InputManager::GetInstance().GetMouseY();
	//if (WindowGameViewport::GetInstance().isFocused) {
		if (isPlaying) {
			/*
				TODO: If user plays the scene,
					give the controls to the visual scripts or the entities or some kind of stuffs...
			*/
		}
		else {
			/*
				If user doesn't play the scene,
					give all controls to the scene,
					these controls come build-in
			*/

			//Reloads all lights
			if (InputManager::GetInstance().IsKeyJustPressed(GLFW_KEY_R)) {
				entityManager->SetLights();
			}

			//If user right-clicks the scene, enable dragging the scene
			if (InputManager::GetInstance().IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
				if (windowGameViewportIsHovered) {
					isDragging = true;
					skipThisFrame = true;
					InputManager::GetInstance().RequestSetMouseVisibility(false);
				}
			}

			//If user releases right-click, disable dragging the scene
			if (InputManager::GetInstance().IsMouseButtonJustReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
				isDragging = false;
				InputManager::GetInstance().RequestSetMouseVisibility(true);
			}

			if (isDragging) {
				if (skipThisFrame) {
					//Reset the mouse position for the first frame
					firstMouseX = currentMouseX;
					firstMouseY = currentMouseY;
					skipThisFrame = false;
				}

				//Get delta mouse position
				int deltaMouseX = currentMouseX - firstMouseX;
				int deltaMouseY = currentMouseY - firstMouseY;

				//Store window size
				int windowWidth = window_width;
				int windowHeight = window_height;

				//Mouse movement effects the scene depends on the window size
				float resX = (float)deltaMouseX * (float)windowWidth / (float)screenWidth;
				float resY = (float)deltaMouseY * (float)windowHeight / (float)screenHeight;

				//If the type of the scene is 2d
				if (sceneType == SCENE_2D) {
					//Move the camera
					cameraX += resX;
					cameraY += resY;
				}
				//If the type of the scene is 3d
				else if (sceneType == SCENE_3D) {
					//Change the camera position
					yaw += resX * 0.5f;
					pitch += -resY * 0.5f;

					//Limit the pitch
					if (pitch > 89.0f)
						pitch = 89.0f;
					if (pitch < -89.0f)
						pitch = -89.0f;

					//Convert yaw and pitch to a vector
					glm::vec3 direction{};
					direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
					direction.y = sin(glm::radians(pitch));
					direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
					eye = glm::normalize(direction);

<<<<<<< HEAD:SceneManager/Scene.cpp
<<<<<<< HEAD:SceneManager/Scene.cpp
					//Set the speed of the camera
					float speed = 20.0f * Timer::GetDeltaTime();
					
					//Movement controls
=======
=======
>>>>>>> parent of db445c5 (added timer to calculate deltatime end use it,):Adrengine/Scene.cpp
					float speed = 0.5f; //deltaTime

>>>>>>> parent of db445c5 (added timer to calculate deltatime end use it,):Adrengine/Scene.cpp
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_W)) {
						cameraX += eye.x * speed;
						cameraY += eye.y * speed;
						cameraZ += eye.z * speed;
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_S)) {
						cameraX -= eye.x * speed;
						cameraY -= eye.y * speed;
						cameraZ -= eye.z * speed;
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_D)) {
						glm::vec3 right = glm::normalize(glm::cross(eye, glm::vec3(0.0f, 1.0f, 0.0f)));

						cameraX += right.x * speed;
						cameraY += right.y * speed;
						cameraZ += right.z * speed;
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_A)) {
						glm::vec3 right = glm::normalize(glm::cross(eye, glm::vec3(0.0f, 1.0f, 0.0f)));

						cameraX -= right.x * speed;
						cameraY -= right.y * speed;
						cameraZ -= right.z * speed;
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_SPACE)) {
						glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

						cameraX += up.x * speed;
						cameraY += up.y * speed;
						cameraZ += up.z * speed;
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
						glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

						cameraX -= up.x * speed;
						cameraY -= up.y * speed;
						cameraZ -= up.z * speed;
					}

				}

				//Set last mouse to current mouse
				firstMouseX = currentMouseX;
				firstMouseY = currentMouseY;
			}
		}
	//}
		//Update each entity via entity manager
		if (entityManager) {
			nlohmann::json sceneJson = ToJson();
			entityManager->UpdateEntities(windowSceneFocused, windowSceneDeletePressed, pendingDelete, selectedId, selectFunction, projectDir, sceneId, sceneJson);
		}
}

/*
PURPOSE: Releases scene objects
*/
void Scene::ReleaseScene()
{
	//Release entity manager
	if (entityManager) {
		entityManager->ReleaseEntityManager();
		delete entityManager;
	}
}

/*
PURPOSE: Creates a json content from this scene
*/
nlohmann::json Scene::ToJson()
{
	nlohmann::json j;
	j["id"] = sceneId;
	j["name"] = sceneName;
	j["cameraX"] = cameraX;
	j["cameraY"] = cameraY;
	j["cameraZ"] = cameraZ;
	j["yaw"] = yaw;
	j["pitch"] = pitch;
	j["type"] = sceneType;

	//Add each entity to json
	if (entityManager) {
		for (auto& entity : entityManager->GetEntities()) {
			if(entity.second && entity.second->GetEntityParams())
				j["entities"].push_back(entity.second->GetEntityParams()->id);
		}
	}
	return j;
}

/*
PURPOSE: Creates a scene from its json content
*/
void Scene::FromJson(const nlohmann::json& json, std::string projectDir, std::unordered_map<std::string, std::pair<std::shared_ptr<Entity>, std::shared_ptr<EntityParams>>>& entityTypes)
{
	//Some loads
	sceneId = json.value("id", "");
	sceneName = json.value("name", "");
	sceneType = (SceneType)json.value("type", SCENE_2D);
	cameraX = json.value("cameraX", 0.0f);
	cameraY = json.value("cameraY", 0.0f);
	cameraZ = json.value("cameraZ", 0.0f);
	yaw = json.value("yaw", 0.0f);
	pitch = json.value("pitch", -90.0f);

	//Setup the eye vector from yaw and pitch
	glm::vec3 direction{};
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	eye = glm::normalize(direction);

	//Create an entity manager
	entityManager = new EntityManager();
	entityManager->InitEntityManager();

	//Load each entity
	if (json.contains("entities")) {
		for (auto& entity : json["entities"]) {
			//Load entity
			std::string entitiesDir = projectDir + "entities/";
			std::string entityDir = entitiesDir + std::string(entity) + "/";
			std::string entityFile = entityDir + std::string(entity) + ".adrengineentity";
			nlohmann::json entityJson = AssetSaver::LoadEntityFromFile(entityFile);

			std::shared_ptr<Entity> entity;
			if (!entityJson.is_null()) {
				std::string type = entityJson.value("type", "");
				if (type.empty())
					continue;
				auto& types = entityTypes;
				auto typeIter = types.find(type);
				if (typeIter == types.end())
					continue;

				//Create an entity clone object from entity type
				entity = typeIter->second.first->clone();

				//Create parameter object for the entity
				std::shared_ptr<EntityParams> params = typeIter->second.second->clone();
				params->FromJson(entityJson);
				entity->CreateEntity(params);
			}
			//Add the entity to entity manager
			if(entity.get())
				entityManager->GetEntities().insert(std::pair<std::string, std::shared_ptr<Entity>>(entity->GetEntityParams()->id, entity));
		}
	}

	//Setup parent child relationships
	for (auto& entity : GetEntityManager()->GetEntities()) {
		for (auto& child : GetEntityManager()->GetEntities()) {
			if (entity.second->GetEntityParams()->id == child.second->GetEntityParams()->parentId) {
				child.second->GetEntityParams()->parent = entity.second;

				entity.second->GetEntityParams()->children.push_back(child.second);
			}
		}
	}
}
