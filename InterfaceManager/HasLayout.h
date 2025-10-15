#pragma once

/*
HasLayout
  - Mixin class for windows that have a layout
  - Provides functionality to set default layout and apply a layout
*/

#include "LayoutManager.h"

class HasLayout
{
public:
	void UpdateFirstFrame();
	virtual void SetDefaultLayout() {}; // TODO: make pure virtual
	void ApplyLayout(std::shared_ptr<WindowLayout> newWindowLayout);
	void SetWindowLayout(std::shared_ptr<WindowLayout> layout) { windowLayout = layout; }

protected:
	bool firstFrame = true;
	std::shared_ptr<WindowLayout> windowLayout = nullptr;
};