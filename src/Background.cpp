#include "Background.h"

#include "SoundManager.h"
#include "TextureManager.h"


Background::Background()
{
	//GetTransform()->scale *= 0.5f;
	TextureManager::Instance().Load("../Assets/textures/backgrounds/hallway.png", "hallway");
	const auto size = TextureManager::Instance().GetTextureSize("hallway");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	SetType(GameObjectType::BACKGROUND);
	GetRigidBody()->isColliding = false;
	
}

Background::~Background()
= default;

void Background::Draw()
{
	TextureManager::Instance().Draw("hallway",
		GetTransform()->position, 0, 255);
}

void Background::Update()
{
}

void Background::Clean()
{
}

