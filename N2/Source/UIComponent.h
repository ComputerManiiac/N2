#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "Component.h"
#include "Info.h"

enum UI_TYPE {
	UI_TEXTURE,
	UI_TEXT,
	UI_BUTTON,
};

class UIComponent : public Component
{
public:

	UIComponent(Entity* parent, UI_TYPE type);
	UIComponent(Entity* parent);
	UIComponent();
	~UIComponent();

private:
	UI_TYPE type;
	Vector2 position;
	Vector2 texCoord;
};

#endif