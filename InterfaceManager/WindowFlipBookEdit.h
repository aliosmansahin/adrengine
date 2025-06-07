#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "FlipBook.h"

class WindowFlipBookEdit
{
public:
	//drawer
	INTERFACEMANAGER_API void DrawWindow();

	//getter for the intance
	INTERFACEMANAGER_API static WindowFlipBookEdit& GetInstance();
private:
	//singleton
	WindowFlipBookEdit() = default;
	~WindowFlipBookEdit() = default;
	WindowFlipBookEdit(const WindowFlipBookEdit&) = delete;
	WindowFlipBookEdit& operator=(const WindowFlipBookEdit&) = delete;
public:
	bool showWindow = false;
	FlipBook* edittingFlipBook = nullptr;
};