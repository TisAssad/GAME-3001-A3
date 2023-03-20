#include "Obstacle.h"

#include "SoundManager.h"
#include "TextureManager.h"


Obstacle::Obstacle()
{
	TextureManager::Instance().Load("../Assets/textures/tree.png", "obstacle");

	const auto size = TextureManager::Instance().GetTextureSize("obstacle");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));

	SetType(GameObjectType::OBSTACLE);
	GetRigidBody()->isColliding = false;
}

Obstacle::~Obstacle()
= default;

void Obstacle::Draw()
{
	TextureManager::Instance().Draw("obstacle", 
		GetTransform()->position, 0, 255);
}

void Obstacle::Update()
{
}

void Obstacle::Clean()
{
}

