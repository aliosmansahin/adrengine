#include "pch.h"
#include "WindowModalDialog.h"

/*
PURPOSE: Draws the modal window
	A modal window can be a dialog or an alert
*/
void WindowModalDialog::DrawWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 200));
	if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_NoResize)) {
		ImGui::SetWindowFontScale(1.5f);

		ImGui::TextWrapped(caption.c_str());

		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 buttonSize = ImVec2(100, 30);
		ImVec2 buttonPaddingSize = ImVec2(10, 10);

		switch (type)
		{
		case ModalType::Question: {
			ImVec2 buttonsStartPos = ImVec2(windowSize.x - (buttonSize.x + buttonPaddingSize.x) * 2.0f, windowSize.y - buttonSize.y - buttonPaddingSize.y);

			ImGui::SetCursorPos(buttonsStartPos);
			if (ImGui::Button("Yes", buttonSize)) {
				yesCallback();
				CloseModalWindow();
			}

			buttonsStartPos = ImVec2(buttonsStartPos.x + (buttonSize.x + buttonPaddingSize.x), buttonsStartPos.y);

			ImGui::SetCursorPos(buttonsStartPos);
			if (ImGui::Button("No", buttonSize)) {
				CloseModalWindow();
			}
			break;
		}
		case ModalType::Alert: {
			ImVec2 buttonsStartPos = ImVec2(windowSize.x - (buttonSize.x + buttonPaddingSize.x), windowSize.y - buttonSize.y - buttonPaddingSize.y);

			ImGui::SetCursorPos(buttonsStartPos);
			if (ImGui::Button("OK", buttonSize)) {
				CloseModalWindow();
			}
			break;
		}
		default:
			CloseModalWindow();
			break;
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar(1);
}

/*
PURPOSE: Setups and shows a yes-no question window
	the parameter "yesCallback" will be executed when "yes" button is clicked
*/
INTERFACEMANAGER_API void WindowModalDialog::ShowModalQuestion(std::string title, std::string caption, std::function<void()> yesCallback)
{
	this->yesCallback = yesCallback;
	type = ModalType::Question;

	SetupModal(title, caption);
}

/*
PURPOSE: Setups and shows an alert window
*/
INTERFACEMANAGER_API void WindowModalDialog::ShowModalAlert(std::string title, std::string caption)
{
	type = ModalType::Alert;

	SetupModal(title, caption);
}

/*
PURPOSE: Opens a modal window if needed
	OpenPopup function have to be called in a frame,
	but in update function, there is no frame.
	So in order to show modal window there,
	We will open it after (in DrawInterface function)
*/
INTERFACEMANAGER_API void WindowModalDialog::OpenModalIfNeeded()
{
	if (willShowModal) {
		willShowModal = false;

		ImGui::OpenPopup(title.c_str());
	}
}

/*
PURPOSE: Resets variables for modal window and closes it
*/
INTERFACEMANAGER_API void WindowModalDialog::CloseModalWindow()
{
	this->yesCallback = std::function<void()>();
	this->title = "";
	this->caption = "";
	ImGui::CloseCurrentPopup();
}

/*
PURPOSE: Sets main variables for modal
*/
void WindowModalDialog::SetupModal(std::string title, std::string caption)
{
	this->title = title;
	this->caption = caption;

	willShowModal = true;
}

WindowModalDialog& WindowModalDialog::GetInstance()
{
	static WindowModalDialog window;
	return window;
}