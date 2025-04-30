#include "WindowScene.h"

void WindowScene::DrawWindow()
{
	//setting window
	ImGui::Begin(Localization::Get().GetString("interface_main_menu_item_scene"), &showWindow);
	ImGui::SetWindowFontScale(1.5f);
	//caption
	int windowWidth = ImGui::GetWindowSize().x;
	int buttonWidth = 100;
	int padding = 10;
	ImGui::TextColored(ImVec4(0, 255, 0, 255), "Entities");
	ImGui::SameLine();
	ImGui::SetCursorPosX(windowWidth - buttonWidth - padding);
	if (ImGui::Button("Add", ImVec2(buttonWidth, 0))) {
		WindowAddEntity::GetInstance().showWindow = true;
		WindowAddEntity::GetInstance().shouldFocus = true;
	}
	ImGui::Separator();

	//entities window
	ImGui::BeginChild("Entities");
	focused = ImGui::IsWindowFocused();
	deletePressed = ImGui::IsKeyDown(ImGuiKey_Delete);
	ImGui::Selectable("Root");

	//to drop to root
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ID")) {
			const char* payloadData = static_cast<const char*>(payload->Data);
			std::string droppedId(payloadData, payload->DataSize);

			auto& entities = SceneManager::GetInstance().currentScene->GetEntityManager()->GetEntities();
			auto droppedRef = entities.find(droppedId);
			if (droppedRef != entities.end()) {
				auto& dropped = droppedRef->second;

				std::shared_ptr<Entity> oldParent = dropped->GetEntityParams()->parent;
				if (oldParent.get()) {
					auto& children = oldParent->GetEntityParams()->children;

					children.erase(
						std::remove_if(children.begin(), children.end(), [&](std::shared_ptr<Entity>& child) {
							return child->GetEntityParams()->id == droppedId;
							}),
						children.end()
					);
				}
				dropped->GetEntityParams()->parent = nullptr;
				dropped->GetEntityParams()->parentId = "";
			}
		}
		ImGui::EndDragDropTarget();
	}

	//entities
	for (auto& entity : SceneManager::GetInstance().currentScene->GetEntityManager()->GetEntities()) {
		if(entity.second.get()->GetEntityParams()->parent == nullptr) {
			DrawEntity(*entity.second.get());
		}
	}

	for (auto& fn : deferredEntityChanges)
		fn();

	deferredEntityChanges.clear();
	ImGui::EndChild();
	ImGui::End();
}

void WindowScene::DrawEntity(Entity& entity, int depth)
{
	//preparation for treenode
	auto params = entity.GetEntityParams();

	ImGui::PushID(params->id.c_str());
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (params->children.empty())
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (entity.GetEntityParams()->id == selectedId)
		flags |= ImGuiTreeNodeFlags_Selected;

	bool nodeOpen = ImGui::TreeNodeEx(params->name.c_str(), flags);

	if (ImGui::IsItemClicked()) {
		if (ImGui::IsMouseDown(0)) {
			WindowEntityProperties::GetInstance().SelectEntity(&entity);
			selectedId = entity.GetEntityParams()->id;
		}
	}
	if (ImGui::IsItemHovered()) {
		if (ImGui::IsMouseReleased(1)) {
			ImGui::OpenPopup("EntityPopup");
			selectedId = entity.GetEntityParams()->id;
		}
	}

	if (ImGui::BeginPopup("EntityPopup")) {
		if (ImGui::MenuItem("Add Entity")) {
			WindowAddEntity::GetInstance().showWindow = true;
			WindowAddEntity::GetInstance().shouldFocus = true;
			addParent = &entity;
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Delete")) {
			pendingDelete = true;
		}
		ImGui::EndPopup();
	}

	//drag and drop source
	if (ImGui::BeginDragDropSource()) {
		ImGui::SetDragDropPayload("ENTITY_ID", entity.GetEntityParams()->id.c_str(), entity.GetEntityParams()->id.size());
		ImGui::Text("Drop", entity.GetEntityParams()->id);
		ImGui::EndDragDropSource();
	}

	//drag and drop target
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_ID")) {
			const char* payloadData = static_cast<const char*>(payload->Data);
			std::string droppedId = std::string(payloadData, payload->DataSize);

			if (droppedId != entity.GetEntityParams()->id) {
				std::string targetId = entity.GetEntityParams()->id;

				//do the process after
				deferredEntityChanges.push_back([=]() {
					auto& entities = SceneManager::GetInstance().currentScene->GetEntityManager()->GetEntities();
					//get dropped and target entities
					auto droppedRef = entities.find(droppedId);
					auto targetRef = entities.find(targetId);

					if (droppedRef != entities.end() && targetRef != entities.end()) {
						auto& dropped = droppedRef->second;
						auto& target = targetRef->second;

						bool toChildren = false;

						CheckForChildren(toChildren, dropped.get(), targetId);

						if (!toChildren) {
							if (dropped->GetEntityParams()->parent.get() != nullptr) {
								//delete from children of old parent
								std::shared_ptr<Entity> oldParent = dropped->GetEntityParams()->parent;
								if (oldParent != nullptr) {
									auto& oldParentChildren = oldParent->GetEntityParams()->children;

									oldParentChildren.erase(std::remove_if(oldParentChildren.begin(), oldParentChildren.end(), [&](std::shared_ptr<Entity>& child) {
										return child->GetEntityParams()->id == droppedId;
										}), oldParentChildren.end());

								}
							}

							//new position
							target->GetEntityParams()->children.push_back(dropped);
							dropped->GetEntityParams()->parent = target;
							dropped->GetEntityParams()->parentId = target->GetEntityParams()->id;
						}
					}
					});
			}
		}
		
		ImGui::EndDragDropTarget();
	}

	if (nodeOpen) {
		//recursive for children
		if (entity.GetEntityParams()) {
			for (auto& child : entity.GetEntityParams()->children) {
				if (!child) return;

				auto params = child->GetEntityParams();
				if (!params) return;

				std::string id = params->id;

				auto scene = SceneManager::GetInstance().currentScene;
				if (!scene) return;

				auto manager = scene->GetEntityManager();
				if (!manager) return;

				auto& entities = manager->GetEntities();
				auto it = entities.find(id);
				if (it != entities.end() && it->second) {
					DrawEntity(*it->second, depth + 1);
				}
			}
		}
		ImGui::TreePop();
	}


	//close
	ImGui::PopID();
}

void WindowScene::CheckForChildren(bool& toChildren, Entity* entity, std::string droppedId)
{
	auto& children = entity->GetEntityParams()->children;
	for (auto& child : children) {
		if (child->GetEntityParams()->id == droppedId) {
			toChildren = true;
			return;
		}
		CheckForChildren(toChildren, child.get(), droppedId);
	}
}

WindowScene& WindowScene::GetInstance()
{
	static WindowScene windowScene;
	return windowScene;
}
