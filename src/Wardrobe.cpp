#include "Wardrobe.h"

#include "SoundManager.h"
#include "TextureManager.h"


Wardrobe::Wardrobe()
{
	TextureManager::Instance().Load("../Assets/textures/furniture/wardrobe.png", "wardrobe");

	const auto size = TextureManager::Instance().GetTextureSize("wardrobe");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));

	SetType(GameObjectType::OBSTACLE);
	GetRigidBody()->isColliding = false;
}

Wardrobe::~Wardrobe()
= default;

void Wardrobe::Draw()
{
	TextureManager::Instance().Draw("wardrobe", 
		GetTransform()->position, 0, 255);
}

void Wardrobe::Update()
{
}

void Wardrobe::Clean()
{
}

