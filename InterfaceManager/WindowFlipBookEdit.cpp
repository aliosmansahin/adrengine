#include "pch.h"
#include "WindowFlipBookEdit.h"

INTERFACEMANAGER_API void WindowFlipBookEdit::DrawWindow()
{
}

INTERFACEMANAGER_API WindowFlipBookEdit& WindowFlipBookEdit::GetInstance()
{
	static WindowFlipBookEdit window;
	return window;
}
