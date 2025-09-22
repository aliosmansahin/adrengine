#include "pch.h"
#include "Scene.h"

/*
PURPOSE: Sets a scene parameters and creates an entity manager
*/
bool Scene::CreateScene(std::string sceneId, Utils::SceneType sceneType)
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

	editorCamera = new Camera();
	CameraParams* cameraParams = new CameraParams();

	//Set type of projection
	if (sceneType == Utils::SCENE_2D)
		cameraParams->projectionType = CameraProjection::ORTHOGRAPHIC;
	if (sceneType == Utils::SCENE_3D)
		cameraParams->projectionType = CameraProjection::PERPECTIVE;

	editorCamera->CreateEntity(std::shared_ptr<CameraParams>(cameraParams));
	currentCamera = editorCamera;

	physics = new BulletPhysics();
	physics->Init();

	return true;
}

/*
PURPOSE: Draws the scene
*/
void Scene::DrawScene(int window_width, int window_height)
{
	//If there is an entity manager, draw each entity via entity manager
	if(entityManager)
		entityManager->DrawEntities(window_width, window_height, currentCamera->GetEntityParams()->GetPosition(), (sceneType == Utils::SCENE_3D));
}

/*
PURPOSE: Update scene objects and handles camera updates
*/
void Scene::UpdateScene(
	bool isPlaying,
	bool windowGameViewportIsHovered,
	bool windowGameViewportIsFocused,
	int screenWidth,
	int screenHeight,
	int window_width,
	int window_height,
	bool windowSceneFocused,
	bool windowSceneDeletePressed,
	bool& pendingDelete,
	std::string selectedId,
	std::function<void(std::string)> extraDeletingFunc,
	std::string& projectDir)
{
	//Setups for mouse positions
	int currentMouseX = InputManager::GetInstance().GetMouseX();
	int currentMouseY = InputManager::GetInstance().GetMouseY();

	if (isPlaying) {
		//Scene will use the camera that user creates
		if(gameCamera)
			currentCamera = gameCamera;
		/*
			TODO: If user plays the scene,
				give the controls to the visual scripts or the entities or some kind of stuffs...
		*/
	}
	else {
		//Use editor camera
		currentCamera = editorCamera;

		/*
			If user doesn't play the scene,
				give all controls to the scene,
				these controls come build-in
		*/

		//If user right-clicks the scene, enable dragging the scene
		if (InputManager::GetInstance().IsMouseButtonJustPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			isDragging = true;
			skipThisFrame = true;
			InputManager::GetInstance().SetMouseVisibility(false);
		}

		//If user releases right-click, disable dragging the scene
		if (InputManager::GetInstance().IsMouseButtonJustReleased(GLFW_MOUSE_BUTTON_RIGHT)) {
			isDragging = false;
			InputManager::GetInstance().SetMouseVisibility(true);
		}

		//Get if mouse left is clicked
		leftPressed = InputManager::GetInstance().IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

		//Get if delete key is pressed
		deletePressed = InputManager::GetInstance().IsKeyPressed(GLFW_KEY_DELETE);

		//Reset delta mouse position
		deltaX = 0.0f;
		deltaY = 0.0f;

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

			//Save delta mouse position to use it from another window
			deltaX = resX;
			deltaY = resY;

			if (windowGameViewportIsFocused) {
				//If the type of the scene is 2d
				if (sceneType == Utils::SCENE_2D) {
					//Move the camera
					currentCamera->AddPosition(glm::vec3(-resX, -resY, 0.0f), isPlaying);
				}
				//If the type of the scene is 3d
				else if (sceneType == Utils::SCENE_3D) {
					//Change the camera position
					currentCamera->AddRotation(resX * 0.5f, -resY * 0.5f, isPlaying);

					//Set the speed of the camera
					float speed = 20.0f * Timer::GetDeltaTime();

					glm::vec3 forwardVector = currentCamera->GetEntityParams()->GetRotation();

					//Movement controls
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_W)) {
						currentCamera->AddPosition(forwardVector * speed, isPlaying);
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_S)) {
						currentCamera->AddPosition(forwardVector * -speed, isPlaying);
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_D)) {
						glm::vec3 right = glm::normalize(glm::cross(forwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

						currentCamera->AddPosition(right * speed, isPlaying);
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_A)) {
						glm::vec3 right = glm::normalize(glm::cross(forwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

						currentCamera->AddPosition(right * -speed, isPlaying);
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_SPACE)) {
						glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

						currentCamera->AddPosition(up * speed, isPlaying);
					}
					if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
						glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

						currentCamera->AddPosition(up * -speed, isPlaying);
					}
				}
			}

			//Set last mouse to current mouse
			firstMouseX = currentMouseX;
			firstMouseY = currentMouseY;
		}
	}

	//Update the transform matrix depends on the scene type
	if (currentCamera->GetProjectionType() == CameraProjection::ORTHOGRAPHIC)
		ShaderManager::GetInstance().UpdateTransformMatrix2D((int)window_width, (int)window_height, (int)currentCamera->GetEntityParams()->GetPosition().x, (int)currentCamera->GetEntityParams()->GetPosition().y);
	if (currentCamera->GetProjectionType() == CameraProjection::PERPECTIVE)
		ShaderManager::GetInstance().UpdateTransformMatrix3D(currentCamera->GetEntityParams()->GetRotation(),
			(int)window_width, (int)window_height,
			currentCamera->GetEntityParams()->GetPosition().x, currentCamera->GetEntityParams()->GetPosition().y, currentCamera->GetEntityParams()->GetPosition().z,
			currentCamera->GetFOV());

	if (entityManager) {
		entityManager->SetRigitbodiesFromEntities(physics);
	}

	//Update physics
	physics->Update(Timer::GetDeltaTime());

	//Update each entity via entity manager
	if (entityManager) {
		nlohmann::json sceneJson = ToJson();
		entityManager->UpdateEntities(windowSceneFocused, windowSceneDeletePressed, pendingDelete, isPlaying, selectedId, extraDeletingFunc, projectDir, sceneId, sceneJson, gameCamera, physics);
	}

	if (entityManager) {
		entityManager->SetEntitiesFromRigidbodies(physics);
	}
}

/*
PURPOSE: Releases scene objects
*/
void Scene::ReleaseScene()
{
	//Release entity manager
	if (entityManager) {
		entityManager->ReleaseEntityManager(physics);
		delete entityManager;
	}

	//Release physics
	if (physics) {
		physics->Shutdown();
		delete physics;
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
	j["cameraX"] = editorCamera->GetEntityParams()->GetPosition().x;
	j["cameraY"] = editorCamera->GetEntityParams()->GetPosition().y;
	j["cameraZ"] = editorCamera->GetEntityParams()->GetPosition().z;
	j["yaw"] = editorCamera->GetYawPitch().first;
	j["pitch"] = editorCamera->GetYawPitch().second;
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
	sceneType = (Utils::SceneType)json.value("type", Utils::SCENE_2D);

	//Create editor camera and set it
	editorCamera = new Camera();
	CameraParams* cameraParams = new CameraParams();

	//Set type of projection
	if (sceneType == Utils::SCENE_2D)
		cameraParams->projectionType = CameraProjection::ORTHOGRAPHIC;
	if (sceneType == Utils::SCENE_3D)
		cameraParams->projectionType = CameraProjection::PERPECTIVE;

	editorCamera->CreateEntity(std::shared_ptr<CameraParams>(cameraParams));

	glm::vec3 cameraPos = glm::vec3(0.0f);
	cameraPos.x = json.value("cameraX", 0.0f);
	cameraPos.y = json.value("cameraY", 0.0f);
	cameraPos.z = json.value("cameraZ", 0.0f);
	
	float yaw = json.value("yaw", -90.0f);
	float pitch = json.value("pitch", 0.0f);

	editorCamera->SetPosition(cameraPos, false);
	editorCamera->SetRotation(yaw, pitch, false);

	currentCamera = editorCamera;

	//Create an entity manager
	entityManager = new EntityManager();
	entityManager->InitEntityManager();

	physics = new BulletPhysics();
	physics->Init();

	//Load each entity
	if (json.contains("entities")) {
		for (auto& entity : json["entities"]) {
			//Load entity
			nlohmann::json entityJson = AssetSaver::LoadEntityFromFile(projectDir, std::string(entity));

			std::shared_ptr<Entity> entity;
			std::shared_ptr<EntityParams> params;
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
				params = typeIter->second.second->clone();
				entity->CreateEntity(params);
				
				//TileMap has own fromjson function
				if (type == "TileMap") {
					auto tileMap = std::dynamic_pointer_cast<TileMap>(entity);
					if (tileMap.get()) {
						tileMap->FromJson(entityJson);
					}
				}
				//FlipBook has own fromjson function
				else if (type == "FlipBook") {
					auto flipBook = std::dynamic_pointer_cast<FlipBook>(entity);
					if (flipBook.get()) {
						flipBook->FromJson(entityJson);
					}
				}
			}
			
			//Add the entity to entity manager
			if(entity.get())
				entityManager->GetEntities().insert(std::pair<std::string, std::shared_ptr<Entity>>(entityJson.value("id", ""), entity));

			if(params.get())
				params->FromJson(entityJson, projectDir, this);

			//Initialize a rigidbody for object
			Object* object = dynamic_cast<Object*>(entity.get());
			if (object != nullptr) { //Ensure this is an object
				physics->AddRigidBody(object->rigidBody);
			}
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
