#include "Bed.h"

#include "SoundManager.h"
#include "TextureManager.h"


Bed::Bed()
{
	TextureManager::Instance().Load("../Assets/textures/furniture/bed.png", "bed");

	//GetTransform()->scale *= 0.5f;

	const auto size = TextureManager::Instance().GetTextureSize("bed");
	SetWidth(static_cast<int>(size.x * GetTransform()->scale.x));
	SetHeight(static_cast<int>(size.y * GetTransform()->scale.y));

	SetType(GameObjectType::OBSTACLE);
	GetRigidBody()->isColliding = false;
	
}

Bed::~Bed()
= default;

void Bed::Draw()
{
	TextureManager::Instance().Draw("bed", GetTransform()->position, 
		this, 0, 255, false, SDL_FLIP_NONE);
}

void Bed::Update()
{
}

void Bed::Clean()
{
}

