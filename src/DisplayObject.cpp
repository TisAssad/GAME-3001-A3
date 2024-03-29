#include "DisplayObject.h"

#include <iostream>

#include "CollisionManager.h"

DisplayObject::DisplayObject()
= default;

DisplayObject::~DisplayObject()
= default;

Scene * DisplayObject::GetParent() const
{
	return m_pParentScene;
}

void DisplayObject::SetParent(Scene * parent)
{
	m_pParentScene = parent;
}

uint32_t DisplayObject::GetLayerIndex() const
{
	return m_layerIndex;
}
uint32_t DisplayObject::GetLayerOrderIndex() const
{
	return m_layerOrderIndex;
}

void DisplayObject::SetLayerIndex(const uint32_t new_index, const uint32_t new_order)
{
	m_layerIndex = new_index;
	m_layerOrderIndex = new_order;
}

bool DisplayObject::ObjectInteration(DisplayObject* object)
{
	if(CollisionManager::AABBRadiusCheck(this, object, 5))
	{
		return true;
	}
	return false;
}

