#include "WindowVisualScript.h"

void WindowVisualScript::DrawWindow()
{
    // Compile toolbar
    int windowHeight = (int)ImGui::GetWindowHeight();
    int toolbarWidth = 100;
    int toolbarHeight = (int)ImGui::GetContentRegionAvail().y - InterfaceManager::GetInstance().tabHeight;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;
    float titleHeight = ImGui::GetFontSize() + windowPadding.y;
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + InterfaceManager::GetInstance().tabHeight + titleHeight));
    ImGui::SetNextWindowSize(ImVec2((float)toolbarWidth, viewport->WorkSize.y - InterfaceManager::GetInstance().tabHeight - titleHeight));
    //ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::Begin("Toolbar", nullptr, window_flags);
    ImGui::SetWindowFontScale(1.5f);
    ImGui::PopStyleVar(3);

    static int selected = false;
    if (ImGui::Selectable("Compile", selected, ImGuiSelectableFlags_None, ImVec2((float)toolbarWidth, 50))) {
        VisualScriptManager::GetInstance().SaveScript(VisualScriptManager::GetInstance().currentScript);
        std::string result = VisualScriptManager::GetInstance().CompileScript(VisualScriptManager::GetInstance().currentScript.get());
        if (result.empty())
            Logger::Log("P", "Current script was compiled successfully");
        else
            Logger::Log("E", result.c_str());
    }


    ImGui::End();

    //error handling


    ImGui::Begin("Visual Script", &showWindow);

    ImNodes::BeginNodeEditor();

    for (auto& node : VisualScriptManager::GetInstance().currentScript->nodes) {
        node.second->Draw();
    }

    for (auto& link : VisualScriptManager::GetInstance().currentScript->links) {
        ImNodes::Link(link.first, link.second.first, link.second.second);
    }

    ImNodes::EndNodeEditor();

    static int nextId = 5; // change after thich node

    static int maxLinkId = 0;
    int start_attr, end_attr;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) {
        maxLinkId = std::max(maxLinkId + 1, (int)VisualScriptManager::GetInstance().currentScript->links.size());
        VisualScriptManager::GetInstance().currentScript->links.insert(std::pair<int, std::pair<int, int>>(maxLinkId, std::make_pair(start_attr, end_attr)));
    }
    int linkId;
    if (ImNodes::IsLinkHovered(&linkId) && ImGui::IsKeyReleased(ImGuiKey_Delete)) {
        auto link = VisualScriptManager::GetInstance().currentScript->links.find(linkId);
        if (link != VisualScriptManager::GetInstance().currentScript->links.end())
            VisualScriptManager::GetInstance().currentScript->links.erase(link);
    }

    ImGui::End();

    static bool showAddNode = false;

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        showAddNode = true;
        ImGui::SetNextWindowPos(ImGui::GetMousePos());
        ImGui::SetNextWindowSize(ImVec2(300, 500));
    }
    if (showAddNode) {
        ImGui::Begin("Add Node", &showAddNode, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowFontScale(1.5f);

        static bool shouldFocus = true;
        static char typeBuf[32];
        static std::vector<std::string> types;
        ImVec2 pos = ImGui::GetWindowPos();

        if (shouldFocus) {
            ImGui::SetKeyboardFocusHere();
            shouldFocus = false;
        }
        ImGui::InputTextWithHint("Node Name", "Search for node", typeBuf, sizeof(typeBuf));
        if (ImGui::IsItemEdited()) {
            types.clear();
            typeBuf[sizeof(typeBuf) - 1] = '\0';
            std::string search = typeBuf;
            if (!search.empty()) {
                std::transform(search.begin(), search.end(), search.begin(), [](unsigned char c) {
                    return std::toupper(c);
                    });
                std::cout << "search: " << search << std::endl;

                for (auto& typeIter : VisualScriptManager::GetInstance().types) {
                    std::string type = typeIter.first;
                    std::string typeToUp = type;
                    std::transform(typeToUp.begin(), typeToUp.end(), typeToUp.begin(), [](unsigned char c) {
                        return std::toupper(c);
                        });
                    std::cout << "typetoup: " << typeToUp << std::endl;
                    if (typeToUp.compare(0, search.length(), search) == 0) {
                        std::cout << type << std::endl;
                        types.push_back(type);
                    }
                }
            }
        }

        ImGui::Separator();
        ImGui::BeginChild("Results");
        int padding = 10;
        for (auto& type : types) {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
            if (ImGui::Selectable(type.c_str())) {
                auto& manager = VisualScriptManager::GetInstance();
                auto it = manager.types.find(type);
                if (it == manager.types.end()) return; // Not found, güvenlik önlemi

                std::shared_ptr<Node> node = it->second->clone();

                if (type == "Begin") {
                    node->SetId(0);
                }
                else if (type == "Return") {
                    node->SetId(2);
                }
                else {
                    node->SetId(nextId);
                }

                std::cout << "id: " << node->GetId() << std::endl;

                node->SetPos((int)pos.x, (int)pos.y);
                VisualScriptManager::GetInstance().currentScript->nodes[node->GetId()] = node;
                nextId += node->GetIdPass();

                showAddNode = false;
                memset(typeBuf, 0, sizeof(typeBuf));
                types.clear();
            }
        }
        ImGui::EndChild();

        ImGui::End();

    }
}

WindowVisualScript& WindowVisualScript::GetInstance()
{
    static WindowVisualScript window;
    return window;
}
