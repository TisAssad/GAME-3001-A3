#include "InteractionObject.h"

#include "SoundManager.h"
#include "TextureManager.h"


InteractionObject::InteractionObject(int width, int height)
{
	//GetTransform()->scale *= 0.5f;
	SetWidth(width);
	SetHeight(height);
	SetType(GameObjectType::INTERACTION_OBJECT);
	GetRigidBody()->isColliding = false;
	
}

InteractionObject::~InteractionObject()
= default;

void InteractionObject::Draw()
{
}

void InteractionObject::Update()
{
}

void InteractionObject::Clean()
{
}

