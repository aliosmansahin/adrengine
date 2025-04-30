#include "Scene.h"

bool Scene::CreateScene(std::string sceneId)
{
	this->sceneId = sceneId;
	this->sceneName = sceneId;

	entityManager = new EntityManager();
	entityManager->InitEntityManager();

	firstMouseX = InputManager::GetInstance().GetMouseX();
	firstMouseY = InputManager::GetInstance().GetMouseY();

	return true;
}

void Scene::DrawScene()
{
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
				//InputManager::GetInstance().SetMousePos(screenWidth / 2, screenHeight / 2);
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

			// Kamerayý hareket ettir
			cameraX += resX;
			cameraY += resY;

			firstMouseX = currentMouseX;
			firstMouseY = currentMouseY;

			//InputManager::GetInstance().SetMousePos(screenWidth / 2, screenHeight / 2);
		}
	}
	if(entityManager)
		entityManager->DrawEntities();
}

void Scene::UpdateScene()
{
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
