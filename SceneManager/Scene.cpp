#include "pch.h"
#include "Scene.h"

#include "ServiceLocator.h"

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
	entityManager = ServiceLocator::Get<IEngine>()->CreateEntityManager();
	if (!entityManager->InitEntityManager())
		return false;

	//Some mouse position setups
	firstMouseX = InputManager::GetInstance().GetMouseX();
	firstMouseY = InputManager::GetInstance().GetMouseY();

	//Create camera entity of the editor
	std::shared_ptr<IEntity> i_entity = ServiceLocator::Get<IEngine>()->CreateEntity("Camera");

	editorCamera = std::dynamic_pointer_cast<ICamera>(i_entity);

	if (!editorCamera)
		return false;

	std::shared_ptr<ICameraParams> cameraParams = std::dynamic_pointer_cast<ICameraParams>(editorCamera->GetEntityParams());

	//Set type of projection
	if (sceneType == Utils::SCENE_2D)
		cameraParams->SetProjectionType(CameraProjection::ORTHOGRAPHIC);
	if (sceneType == Utils::SCENE_3D)
		cameraParams->SetProjectionType(CameraProjection::PERPECTIVE);

	currentCamera = editorCamera;

	//Initialize physics
	physics = ServiceLocator::Get<IEngine>()->CreatePhysics();
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
	int window_width,
	int window_height,
	bool windowSceneFocused,
	bool windowSceneDeletePressed,
	bool& pendingDelete,
	std::string selectedId,
	std::function<void(std::string)> extraDeletingFunc)
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
		//Use editor camera and update it
		UpdateEditorCamera(
			currentMouseX,
			currentMouseY,
			window_width,
			window_height,
			windowGameViewportIsFocused,
			isPlaying
		);
	}

	//Update the transform matrix depends on the scene type
	UpdateTransformMatrixForTheCamera(window_width, window_height);

	/*
		Sync rigidbodies and entities here
		 * Entities -> Rigidbodies
		 * Update Physics
		 * Update Entities
		 * Rigidbodies -> Entities
	*/

	//Update rigidbodies from entities
	if (entityManager) {
		entityManager->SetRigitbodiesFromEntities();
	}

	//Update physics
	physics->Update(Timer::GetDeltaTime());

	//Update each entity via entity manager
	if (entityManager) {
		nlohmann::json sceneJson = ToJson();
		entityManager->UpdateEntities(windowSceneFocused, windowSceneDeletePressed, pendingDelete, isPlaying, selectedId, extraDeletingFunc, sceneId, sceneJson, gameCamera);
	}

	//Update entities from rigidbodies
	if (entityManager) {
		entityManager->SetEntitiesFromRigidbodies();
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
	}

	//Release physics
	if (physics) {
		physics->Shutdown();
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
				j["entities"].push_back(entity.second->GetEntityParams()->GetId());
		}
	}
	return j;
}

/*
PURPOSE: Creates a scene from its json content
*/
void Scene::FromJson(const nlohmann::json& json)
{
	//Scene properties
	sceneId = json.value("id", "");
	sceneName = json.value("name", "");
	sceneType = (Utils::SceneType)json.value("type", Utils::SCENE_2D);

	//Create camera entity of the editor
	std::shared_ptr<IEntity> i_entity = ServiceLocator::Get<IEngine>()->CreateEntity("Camera");

	editorCamera = std::dynamic_pointer_cast<ICamera>(i_entity);

	if (!editorCamera)
		return;

	std::shared_ptr<ICameraParams> cameraParams = std::dynamic_pointer_cast<ICameraParams>(editorCamera->GetEntityParams());

	//Set type of projection
	if (sceneType == Utils::SCENE_2D)
		cameraParams->SetProjectionType(CameraProjection::ORTHOGRAPHIC);
	if (sceneType == Utils::SCENE_3D)
		cameraParams->SetProjectionType(CameraProjection::PERPECTIVE);

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
	entityManager = ServiceLocator::Get<IEngine>()->CreateEntityManager();
	entityManager->InitEntityManager();

	//Initialize physics
	physics = ServiceLocator::Get<IEngine>()->CreatePhysics();
	physics->Init();

	//Load each entity
	entityManager->LoadEntitiesFromJson(json, shared_from_this());

	//Setup parent child relationships
	entityManager->BuildEntityHierarchy();
}

/*
PURPOSE: Returns scene id
*/
SCENEMANAGER_API std::string Scene::GetSceneId()
{
	return sceneId;
}

/*
PURPOSE: Returns scene id
*/
SCENEMANAGER_API std::string Scene::GetSceneName()
{
	return sceneName;
}

/*
PURPOSE: Returns scene type
*/
SCENEMANAGER_API Utils::SceneType Scene::GetSceneType()
{
	return sceneType;
}

/*
PURPOSE: Returns current camera of the scene as a smart pointer
*/
SCENEMANAGER_API std::shared_ptr<ICamera> Scene::GetCurrentCamera()
{
	return currentCamera;
}

/*
PURPOSE: Returns physics object as a smart pointers
*/
SCENEMANAGER_API std::shared_ptr<IPhysics> Scene::GetPhysics()
{
	return physics;
}

/*
PURPOSE: Returns delta x and y of the cursor as a std::pair
	first -> deltaX
	second -> deltaY
*/
SCENEMANAGER_API std::pair<float, float> Scene::GetDeltaXY()
{
	return { deltaX, deltaY };
}

/*
PURPOSE: Returns true if mouse left is pressed
	otherwise, returns false
*/
SCENEMANAGER_API bool Scene::GetLeftPressed()
{
	return leftPressed;
}

/*
PURPOSE: Returns true if key delete is pressed
	otherwise, returns false
*/
SCENEMANAGER_API bool Scene::GetDeletePressed()
{
	return deletePressed;
}

/*
PURPOSE: Updates editor camera, handles inputs
*/
void Scene::UpdateEditorCamera(
	int currentMouseX,
	int currentMouseY,
	int window_width,
	int window_height,
	bool windowGameViewportIsFocused,
	bool isPlaying)
{
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

		//Store screen size
		float screenWidth = (float)ServiceLocator::Get<IEngine>()->GetScreenSize().first;
		float screenHeight = (float)ServiceLocator::Get<IEngine>()->GetScreenSize().second;

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


/*
PURPOSE: Updates transformation matrices of the camera.
	Updates 2d when the camera type is orthographic.
	Updates 3d when the camera type is perpective.
*/
void Scene::UpdateTransformMatrixForTheCamera(int window_width, int window_height)
{
	if (currentCamera->GetProjectionType() == CameraProjection::ORTHOGRAPHIC)
		ShaderManager::GetInstance().UpdateTransformMatrix2D(
			window_width,
			window_height,
			(int)currentCamera->GetEntityParams()->GetPosition().x,
			(int)currentCamera->GetEntityParams()->GetPosition().y
		);
	if (currentCamera->GetProjectionType() == CameraProjection::PERPECTIVE)
		ShaderManager::GetInstance().UpdateTransformMatrix3D(
			currentCamera->GetEntityParams()->GetRotation(),
			window_width,
			window_height,
			currentCamera->GetEntityParams()->GetPosition().x,
			currentCamera->GetEntityParams()->GetPosition().y,
			currentCamera->GetEntityParams()->GetPosition().z,
			currentCamera->GetFOV()
		);
}
