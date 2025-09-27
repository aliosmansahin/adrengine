#include "pch.h"
#include "WindowProjectDialog.h"

/*
PURPOSE: Draws the window
*/
void WindowProjectDialog::DrawWindow(std::vector<std::string>& latestProjects) {
    //Begin the window
    ImGui::Begin("Create or Open a Project", (bool*)false, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowFontScale(1.5f);

    /* Create a project */

    //Inputbox of path of asset
    ImGui::SeparatorText("Create a Project");
    ImGui::SetNextItemWidth(500.0f);
    ImGui::InputTextWithHint("Create Project Path", "Path", createPathBuf, sizeof(createPathBuf));
    if (ImGui::IsItemEdited()) {
        createPathBuf[sizeof(createPathBuf) - 1] = '\0';
        createPath = createPathBuf;
        createPath += "\\";
    }

    ImGui::SameLine();
    if (ImGui::Button("Browse for Project Path")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        config.countSelectionMax = 1;
        ImGuiFileDialog::Instance()->OpenDialog("CreateProjectPathDialog", "Select a folder", nullptr, config);
    }

    //Inputbox of project name
    ImGui::SetNextItemWidth(200.0f);
    ImGui::InputTextWithHint("Create Project Name", "Name", nameBuf, sizeof(nameBuf));
    if (ImGui::IsItemEdited()) {
        nameBuf[sizeof(nameBuf) - 1] = '\0';
        createProjectName = nameBuf;
    }

    //Create project button
    if (!createPath.empty() && !createProjectName.empty()) {
        if (ImGui::Button("Create Project")) {
            isCreatingProject = true;
        }
    }

    /* Open a project */

    ImGui::SeparatorText("Open a Project");
    ImGui::SetNextItemWidth(500.0f);
    ImGui::InputTextWithHint("Open Project Path", "Path", openPathBuf, sizeof(openPathBuf));
    if (ImGui::IsItemEdited()) {
        openPathBuf[sizeof(openPathBuf) - 1] = '\0';

        std::filesystem::path parentPath = std::filesystem::path(openPathBuf).parent_path();

        openPath = parentPath.parent_path().string() + "\\";
        openProjectName = parentPath.filename().string();
    }

    ImGui::SameLine();
    if (ImGui::Button("Browse for Project File")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        config.countSelectionMax = 1;
        ImGuiFileDialog::Instance()->OpenDialog("OpenProjectPathDialog", "Select a project file", ".adrengineproject", config);
    }

    //Open project button
    if (!openPath.empty()) {
        if (ImGui::Button("Open Project")) {
            isOpeningProject = true;
        }
    }

    /* Latest projects */

    ImGui::SeparatorText("Latest Projects");
    if (latestProjects.empty()) {
        ImGui::TextColored(ImVec4(0, 255, 0, 255), "There is no project");
    }
    else {
        for (auto& project : latestProjects) {
            //Split the path
            std::filesystem::path parentPath = std::filesystem::path(project).parent_path();

            std::string projectPath = parentPath.parent_path().string() + "\\";
            std::string projectName = parentPath.filename().string();

            //Project button
            bool selected = false;

            //Selectable for the project
            ImGui::Selectable(projectName.c_str(), &selected, ImGuiSelectableFlags_None, ImVec2(100, 100));

            //When user double-clicks, open the project
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                openLatestPath = projectPath;
                openLatestProjectName = projectName;

                isOpeningFromLatestProjects = true;
            }
            ImGui::SameLine();
        }
    }

    ImGui::End();

    /* Dialogs */

    //Select folder dialog for creating a project
    if (ImGuiFileDialog::Instance()->Display("CreateProjectPathDialog")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            //Get the folder path
            std::string folderPath = ImGuiFileDialog::Instance()->GetCurrentPath();

            strcpy_s(createPathBuf, sizeof(createPathBuf), folderPath.c_str());
            createPath = folderPath + "\\";
        }

        ImGuiFileDialog::Instance()->Close();
    }

    //Select a project file to open a project
    if (ImGuiFileDialog::Instance()->Display("OpenProjectPathDialog")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            //Get the folder path
            std::string filePathWithFileName = ImGuiFileDialog::Instance()->GetFilePathName();

            strcpy_s(openPathBuf, sizeof(openPathBuf), filePathWithFileName.c_str());

            std::filesystem::path parentPath = std::filesystem::path(filePathWithFileName).parent_path();

            openPath = parentPath.parent_path().string() + "\\";
            openProjectName = parentPath.filename().string();
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

/*
PURPOSE: Resets all buffers and variables
*/
INTERFACEMANAGER_API void WindowProjectDialog::ResetInputs()
{
    createPath = "";
    createProjectName = "";

    openPath = "";
    openProjectName = "";

    openLatestPath = "";
    openLatestProjectName = "";

    memset(createPathBuf, 0, sizeof(createPathBuf));
    memset(nameBuf, 0, sizeof(nameBuf));

    memset(openPathBuf, 0, sizeof(openPathBuf));
}

/*
PURPOSE: Gets the instance of the class
*/
WindowProjectDialog& WindowProjectDialog::GetInstance()
{
	static WindowProjectDialog menubar;
	return menubar;
}
