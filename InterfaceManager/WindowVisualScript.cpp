#include "pch.h"
#include "WindowVisualScript.h"
#include "SceneManager.h"

/*
PURPOSE: Draws the window
*/
void WindowVisualScript::DrawWindow(
    int tabHeight,
    std::string& projectDir,
    Utils::Tab*& openedTab,
    std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& tabs)
{
    //TODO: Active it when it became necessary
    // 
    // 
    // 
    ////Some variables
    //int windowHeight = (int)ImGui::GetWindowHeight();
    //int toolbarWidth = 100;
    //int toolbarHeight = (int)ImGui::GetContentRegionAvail().y - tabHeight;

    ////Settings for the toolbar and begin it
    //ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    //const ImGuiViewport* viewport = ImGui::GetMainViewport();

    //ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;
    //float titleHeight = ImGui::GetFontSize() + windowPadding.y;
    //ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + tabHeight + titleHeight));
    //ImGui::SetNextWindowSize(ImVec2((float)toolbarWidth, viewport->WorkSize.y - tabHeight - titleHeight));
    ////ImGui::SetNextWindowViewport(viewport->ID);
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    //window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    //window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    //ImGui::Begin("Toolbar", nullptr, window_flags);
    //ImGui::SetWindowFontScale(1.5f);
    //ImGui::PopStyleVar(3);

    ////End the toolbar
    //ImGui::End();

    //TODO: Error handling

    //Begin the script editor
    ImGui::Begin("Visual Script", &showWindow);
    ImNodes::BeginNodeEditor();

    //Draw each nodes
    for (auto& node : VisualScriptManager::GetInstance().currentScript->nodes) {
        NodeVisual* vis = &node.second;
        Node* node = vis->logicNode.get();

        node->BeginDraw(vis);
        node->Draw(vis);
        node->EndDraw();
    }

    //Draw each link
    for (const auto link : VisualScriptManager::GetInstance().currentScript->links) {
        ImNodes::Link(link.first, link.second.first, link.second.second);
    }

    //End node editor
    ImNodes::EndNodeEditor();

    //Store start and end of the link
    int start_attr, end_attr;

    //When user creates a link
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) {
        Pin* from = VisualScriptManager::GetInstance().currentScript->FindPinById(start_attr);
        Pin* to = VisualScriptManager::GetInstance().currentScript->FindPinById(end_attr);
        
        if (from && to && (from->type == to->type || from->type == PinType::Any || to->type == PinType::Any)) {
            to->connectedTo = from;
            from->connectedTo = to;
            VisualScriptManager::GetInstance().currentScript->links.insert({ VisualScriptManager::GetInstance().currentScript->nextId++, {from->id, to->id} });
        }
    }

    //When user deletes a link
    int linkId;
    if (ImNodes::IsLinkHovered(&linkId) && ImGui::IsKeyReleased(ImGuiKey_Delete)) {
        auto link = VisualScriptManager::GetInstance().currentScript->links.find(linkId);

        if (link != VisualScriptManager::GetInstance().currentScript->links.end()) {
            Pin* from = VisualScriptManager::GetInstance().currentScript->FindPinById(link->second.first);
            Pin* to = VisualScriptManager::GetInstance().currentScript->FindPinById(link->second.second);

            if (to && to->connectedTo == from) {
                to->connectedTo = nullptr;
                from->connectedTo = nullptr;
            }

            VisualScriptManager::GetInstance().currentScript->links.erase(link);
        }
    }

    //End the script editor window
    ImGui::End();

    //Add node window
    static bool showAddNode = false;

    //Right click opens add node window
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        showAddNode = true;
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
        ImGui::SetNextWindowSize(ImVec2(300, 500));
    }
    if (showAddNode) {
        //Begin add node window
        ImGui::Begin("Add Node", &showAddNode, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowFontScale(1.5f);

        //Some variables
        static bool shouldFocus = true;
        static char typeBuf[32];
        static std::vector<std::string> types;
        ImVec2 pos = ImGui::GetWindowPos();

        //In first frame, focus the input
        if (shouldFocus) {
            ImGui::SetKeyboardFocusHere();
            shouldFocus = false;
        }

        //New node input
        ImGui::InputTextWithHint("Node Name", "Search for node", typeBuf, sizeof(typeBuf));
        if (ImGui::IsItemEdited()) {
            //Clear the previous search result
            types.clear();

            //Zero termination
            typeBuf[sizeof(typeBuf) - 1] = '\0';

            //Store the input in a string
            std::string search = typeBuf;
            if (!search.empty()) {
                std::transform(search.begin(), search.end(), search.begin(), [](unsigned char c) {
                    return std::toupper(c);
                    });

                //Compare each node type with input
                for (auto& typeIter : VisualScriptManager::GetInstance().types) {
                    std::string type = typeIter.first;
                    std::string typeToUp = type;
                    std::transform(typeToUp.begin(), typeToUp.end(), typeToUp.begin(), [](unsigned char c) {
                        return std::toupper(c);
                        });
                    if (typeToUp.compare(0, search.length(), search) == 0) {
                        types.push_back(type);
                    }
                }
            }
        }

        ImGui::Separator();

        //Child window for results
        ImGui::BeginChild("Results");
        int padding = 10;

        //Draw each node type
        for (auto& type : types) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
            if (ImGui::Selectable(type.c_str())) {
                auto& manager = VisualScriptManager::GetInstance();
                auto it = manager.types.find(type);

                if (it == manager.types.end()) return;

                //If the Begin node exists, won't create another one
                if (type == "Begin" && VisualScriptManager::GetInstance().currentScript->nodes.find(0) != VisualScriptManager::GetInstance().currentScript->nodes.end())
                    break;

                std::shared_ptr<Node> node = it->second->clone();

                //Set ids for each type
                node->SetPos((int)pos.x, (int)pos.y);
                node->SetPins();

                /*if (type == "GetThisEntity") {
                    GetThisEntity* entityNode = dynamic_cast<GetThisEntity*>(node.get());

                    if (entityNode) {
                        entityNode->entity = SceneManager::GetInstance().GetSceneById(VisualScriptManager::GetInstance().currentScript->belongsScene)->GetEntityManager()->GetEntityById(VisualScriptManager::GetInstance().currentScript->belongsTo).get();
                    }
                }*/

                //Add a new node
                NodeVisual nodeVisual;
                nodeVisual.logicNode = node;

                //Store node id into another variable
                int idForThisNode = VisualScriptManager::GetInstance().currentScript->nextId;

                //Set id for special nodes
                if (nodeVisual.logicNode->GetType() == "Begin") {
                    idForThisNode = 0; //Begin node always has id 0
                }
                nodeVisual.id = idForThisNode++;

                //Set pin ids
                for (auto& pin : node->inputPins) {
                    pin->id = idForThisNode++;

                    nodeVisual.inputIds.push_back(pin->id);
                }

                for (auto& pin : node->outputPins) {
                    pin->id = idForThisNode++;

                    nodeVisual.outputIds.push_back(pin->id);
                }

                VisualScriptManager::GetInstance().currentScript->nodes[nodeVisual.id] = nodeVisual;
                
                //Set the next id from the stored id
                if (nodeVisual.logicNode->GetType() == "Begin") {
                    VisualScriptManager::GetInstance().currentScript->nextId += idForThisNode;
                }
                else
                    VisualScriptManager::GetInstance().currentScript->nextId = idForThisNode;

                //Close the window and clear input buffer
                showAddNode = false;
                memset(typeBuf, 0, sizeof(typeBuf));
                types.clear();
                break;
            }
        }

        //End windows
        ImGui::EndChild();
        ImGui::End();
    }
}

/*
PURPOSE: Gets the instance of the class
*/
WindowVisualScript& WindowVisualScript::GetInstance()
{
    static WindowVisualScript window;
    return window;
}
