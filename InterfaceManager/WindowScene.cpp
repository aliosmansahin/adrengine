#include "pch.h"
#include "WindowScene.h"
#include "Localization.h"

#include "WindowEntityProperties.h"
#include "WindowAddEntity.h"

/*
PURPOSE: Draws the window
*/
void WindowScene::DrawWindow()
{
	//Begin the window
	ImGui::Begin(Localization::GetString("interface_main_menu_item_scene"), &showWindow);
	ImGui::SetWindowFontScale(1.5f);

	//Some variables
	int windowWidth = (int)ImGui::GetWindowSize().x;
	int buttonWidth = 100;
	int padding = 10;

	//Draw the title
	ImGui::TextColored(ImVec4(0, 255, 0, 255), "Entities");
	ImGui::SameLine();

	//Draw the add button
	ImGui::SetCursorPosX(float(windowWidth - buttonWidth - padding));
	if (ImGui::Button("Add", ImVec2((float)buttonWidth, 0))) {
		//Open the add entity window, and set some stuff
		WindowAddEntity::GetInstance().showWindow = true;
		WindowAddEntity::GetInstance().shouldFocus = true;

		//Parent will be nothing
		addParent = nullptr;
	}
	ImGui::Separator();

	//Entities child window
	ImGui::BeginChild("Entities");

	//Check some key and mouse stuff like focusing
	focused = ImGui::IsWindowFocused();
	deletePressed = ImGui::IsKeyDown(ImGuiKey_Delete);

	//The root of the hierarchy
	ImGui::Selectable("Root");

	//Root will be a target of the dragging
	if (ImGui::BeginDragDropTarget()) {
		//Store the payload
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ID")) {
			const char* payloadData = static_cast<const char*>(payload->Data);
			std::string droppedId(payloadData, payload->DataSize);

			//Get the dropped entity
			auto& entities = ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetEntityManager()->GetEntities();
			auto droppedRef = entities.find(droppedId);
			if (droppedRef != entities.end()) {
				auto& dropped = droppedRef->second;

				//Get the old parent
				std::shared_ptr<IEntity> oldParent = dropped->GetEntityParams()->GetParent();

				//If dropped entity had a parent, delete the entity from the parent
				if (oldParent.get()) {
					auto& children = oldParent->GetEntityParams()->GetChildren();

					children.erase(
						std::remove_if(children.begin(), children.end(), [&](std::shared_ptr<IEntity>& child) {
							return child->GetEntityParams()->GetId() == droppedId;
							}),
						children.end()
					);
				}

				//This entity won't have a parent
				dropped->GetEntityParams()->SetParent(nullptr);
				dropped->GetEntityParams()->SetParentId("");
			}
		}
		ImGui::EndDragDropTarget();
	}

	//Draw each entity
	for (auto& entity : ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetEntityManager()->GetEntities()) {
		if(entity.second.get()->GetEntityParams()->GetParent() == nullptr) {
			DrawEntity(entity.second);
		}
	}

	//Perform entity changes after the drawing loop
	for (auto& fn : deferredEntityChanges)
		fn();

	//After performing, clear the delete functions
	deferredEntityChanges.clear();

	//End the windows
	ImGui::EndChild();
	ImGui::End();
}

/*
PURPOSE: Draws an entity which has given as a smart pointer
TODO: the depth parameter is unnecessary, remove it
*/
void WindowScene::DrawEntity(std::shared_ptr<IEntity>& entity, int depth)
{
	//Preparation for treenode
	auto params = entity->GetEntityParams();

	ImGui::PushID(params->GetId().c_str());
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (params->GetChildren().empty())
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (entity->GetEntityParams()->GetId() == selectedId)
		flags |= ImGuiTreeNodeFlags_Selected;

	//Draw a treenode and store the status if the node is opened
	bool nodeOpen = ImGui::TreeNodeEx(params->GetName().c_str(), flags);

	//Handle left click
	if (ImGui::IsItemClicked()) {
		if (ImGui::IsMouseDown(0)) {
			//Select the entity
			WindowEntityProperties::GetInstance().SelectEntity(entity);
			selectedId = entity->GetEntityParams()->GetId();
		}
	}
	//Handle if right click released
	if (ImGui::IsItemHovered()) {
		if (ImGui::IsMouseReleased(1)) {
			//Open a popup
			ImGui::OpenPopup("EntityPopup");
			selectedId = entity->GetEntityParams()->GetId();
		}
	}

	//Draw the popup
	if (ImGui::BeginPopup("EntityPopup")) {
		if (ImGui::MenuItem("Add Entity")) {
			//Open the add entity window
			WindowAddEntity::GetInstance().showWindow = true;
			WindowAddEntity::GetInstance().shouldFocus = true;
			//If the user selects it, the entity that will be added will have a parent
			addParent = entity;
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Delete")) {
			//The entity manager will delete the entity
			pendingDelete = true;
		}
		ImGui::EndPopup();
	}

	//Drag and drop source
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("ENTITY_ID", entity->GetEntityParams()->GetId().c_str(), entity->GetEntityParams()->GetId().size());
		ImGui::Text("Drop", entity->GetEntityParams()->GetId());
		ImGui::EndDragDropSource();
	}

	//Drag and drop target
	if (ImGui::BeginDragDropTarget()) {
		//Store the payload
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ID")) {
			const char* payloadData = static_cast<const char*>(payload->Data);

			//Get the dropped entity id
			std::string droppedId = std::string(payloadData, payload->DataSize);
			if (droppedId != entity->GetEntityParams()->GetId()) {

				//Get the target entity id
				std::string targetId = entity->GetEntityParams()->GetId();

				//Do the change process after
				deferredEntityChanges.push_back([=]() {
					auto& entities = ServiceLocator::Get<ISceneManager>()->GetOpenedScene()->GetEntityManager()->GetEntities();

					//Get dropped and target entities from their ids
					auto droppedRef = entities.find(droppedId);
					auto targetRef = entities.find(targetId);

					if (droppedRef != entities.end() && targetRef != entities.end()) {
						auto& dropped = droppedRef->second;
						auto& target = targetRef->second;

						//Check if the entity will be moved to its children, if it will, do nothing
						bool toChildren = false;

						CheckForChildren(toChildren, dropped, targetId);

						if (!toChildren) {
							if (dropped->GetEntityParams()->GetParent().get() != nullptr) {
								//Delete it from the children of the old parent
								std::shared_ptr<IEntity> oldParent = dropped->GetEntityParams()->GetParent();
								if (oldParent != nullptr) {
									auto& oldParentChildren = oldParent->GetEntityParams()->GetChildren();

									oldParentChildren.erase(std::remove_if(oldParentChildren.begin(), oldParentChildren.end(), [&](std::shared_ptr<IEntity>& child) {
										return child->GetEntityParams()->GetId() == droppedId;
										}), oldParentChildren.end());

								}
							}

							//Set new position
							target->GetEntityParams()->GetChildren().push_back(dropped);
							dropped->GetEntityParams()->GetParent() = target;
							dropped->GetEntityParams()->GetParentId() = target->GetEntityParams()->GetId();
						}
					}
					});
			}
		}
		
		ImGui::EndDragDropTarget();
	}

	//If the children of the entity is opened
	if (nodeOpen) {
		//Recursive for children
		if (entity->GetEntityParams()) {
			for (auto& child : entity->GetEntityParams()->GetChildren()) {
				if (!child) return;

				auto params = child->GetEntityParams();
				if (!params) return;

				std::string id = params->GetId();

				auto scene = ServiceLocator::Get<ISceneManager>()->GetOpenedScene();
				if (!scene) return;

				auto manager = scene->GetEntityManager();
				if (!manager) return;

				auto& entities = manager->GetEntities();
				auto it = entities.find(id);
				if (it != entities.end() && it->second) {
					DrawEntity(it->second, depth + 1);
				}
			}
		}
		ImGui::TreePop();
	}


	//close
	ImGui::PopID();
}

/*
PURPOSE: Checks if an entity will be moved to its children, if it will, set "toChildren" variable to "true"
*/
void WindowScene::CheckForChildren(bool& toChildren, std::shared_ptr<IEntity> entity, std::string droppedId)
{
	auto& children = entity->GetEntityParams()->GetChildren();
	for (auto& child : children) {
		if (child->GetEntityParams()->GetId() == droppedId) {
			toChildren = true;
			return;
		}
		CheckForChildren(toChildren, child, droppedId);
	}
}

/*
PURPOSE: Gets the instance of the class
*/
WindowScene& WindowScene::GetInstance()
{
	static WindowScene windowScene;
	return windowScene;
}
