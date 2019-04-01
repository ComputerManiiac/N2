#include "UIComponent.h"


UIComponent::UIComponent(Entity* parent, UI_TYPE type) : Component("UI", parent), type(type)
{

}


UIComponent::UIComponent(Entity* parent) : Component("UI", parent)
{
}

UIComponent::UIComponent()
{

}

UIComponent::~UIComponent()
{
}
