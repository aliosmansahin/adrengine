#include "Scene.h"

bool Scene::CreateScene(std::string sceneId, SceneType sceneType)
{
	this->sceneId = sceneId;
	this->sceneName = sceneId;
	this->sceneType = sceneType;

	entityManager = new EntityManager();
	entityManager->InitEntityManager();

	firstMouseX = InputManager::GetInstance().GetMouseX();
	firstMouseY = InputManager::GetInstance().GetMouseY();

	return true;
}

void Scene::DrawScene()
{
	if(entityManager)
		entityManager->DrawEntities();
}

void Scene::UpdateScene()
{
	//if (WindowGameViewport::GetInstance().isFocused) {
		int currentMouseX = InputManager::GetInstance().GetMouseX();
		int currentMouseY = InputManager::GetInstance().GetMouseY();
		if (WindowGameViewport::GetInstance().isPlaying) {
		}
		else {
			if (InputManager::GetInstance().IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
				if (WindowGameViewport::GetInstance().isHovered) {
					isDragging = true;
					skipThisFrame = true;
					InputManager::GetInstance().SetMouseVisibility(false);
				}
			}
			if (InputManager::GetInstance().IsMouseButtonJustReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
				isDragging = false;
				InputManager::GetInstance().SetMouseVisibility(true);
			}
			if (isDragging) {
				if (skipThisFrame) {
					// Sadece ilk frame'de resetle
					firstMouseX = currentMouseX;
					firstMouseY = currentMouseY;
					skipThisFrame = false;
				}
				int deltaMouseX = currentMouseX - firstMouseX;
				int deltaMouseY = currentMouseY - firstMouseY;

				// Ekran çözünürlüðü (ana monitör çözünürlüðü)
				int screenWidth = Engine::GetInstance().screenWidth;
				int screenHeight = Engine::GetInstance().screenHeight;

				// Pencere boyutu
				float windowWidth = WindowGameViewport::GetInstance().window_width;
				float windowHeight = WindowGameViewport::GetInstance().window_height;

				// Normalize et: Mouse hareketinin pencere ve ekran çözünürlüðü arasýnda orantýsýný hesapla
				float resX = (float)deltaMouseX * windowWidth / (float)screenWidth;
				float resY = (float)deltaMouseY * windowHeight / (float)screenHeight;

				if (sceneType == SCENE_2D) {
					cameraX += resX;
					cameraY += resY;
				}
				else if (sceneType == SCENE_3D) {
					yaw += resX * 0.5f;
					pitch += -resY * 0.5f;

					if (pitch > 89.0f)
						pitch = 89.0f;
					if (pitch < -89.0f)
						pitch = -89.0f;

					glm::vec3 direction{};
					direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
					direction.y = sin(glm::radians(pitch));
					direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
					eye = glm::normalize(direction);

					float speed = 0.5f; //deltaTime

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

				firstMouseX = currentMouseX;
				firstMouseY = currentMouseY;

			}
		}
	//}
	if (entityManager)
		entityManager->UpdateEntities();
}

void Scene::ReleaseScene()
{
	if (entityManager) {
		entityManager->ReleaseEntityManager();
		delete entityManager;
	}
}

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
	if (entityManager) {
		for (auto& entity : entityManager->GetEntities()) {
			if(entity.second && entity.second->GetEntityParams())
				j["entities"].push_back(entity.second->GetEntityParams()->id);
		}
	}
	return j;
}

void Scene::FromJson(const nlohmann::json& json)
{
	sceneId = json.value("id", "");
	sceneName = json.value("name", "");
	cameraX = json.value("cameraX", 0.0f);
	cameraY = json.value("cameraY", 0.0f);
	cameraZ = json.value("cameraZ", 0.0f);
	yaw = json.value("yaw", 0.0f);
	pitch = json.value("pitch", 90.0f);
	sceneType = (SceneType)json.value("type", SCENE_2D);

	entityManager = new EntityManager();
	entityManager->InitEntityManager();

	if (json.contains("entities")) {
		for (auto& entity : json["entities"]) {
			std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
			std::string entitiesDir = projectDir + "entities/";
			std::string entityDir = entitiesDir + std::string(entity) + "/";
			std::string entityFile = entityDir + std::string(entity) + ".adrengineentity";
			std::shared_ptr<Entity> entity = AssetSaver::LoadEntityFromFile(entityFile);
			if(entity)
				entityManager->GetEntities().insert(std::pair<std::string, std::shared_ptr<Entity>>(entity->GetEntityParams()->id, entity));
		}
	}

	for (auto& entity : GetEntityManager()->GetEntities()) {
		for (auto& child : GetEntityManager()->GetEntities()) {
			if (entity.second->GetEntityParams()->id == child.second->GetEntityParams()->parentId) {
				child.second->GetEntityParams()->parent = entity.second;

				entity.second->GetEntityParams()->children.push_back(child.second);
			}
		}
	}
}
