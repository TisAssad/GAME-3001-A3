#include "Dresser.h"

#include "SoundManager.h"
#include "TextureManager.h"


Dresser::Dresser()
{
	TextureManager::Instance().Load("../Assets/textures/furniture/dresser.png", "dresser");
	GetTransform()->scale = { 0.5f, 0.5f };

	const auto size = TextureManager::Instance().GetTextureSize("dresser");
	SetWidth(static_cast<int>(size.x * GetTransform()->scale.x));
	SetHeight(static_cast<int>(size.y * GetTransform()->scale.y));

	SetType(GameObjectType::OBSTACLE);
	GetRigidBody()->isColliding = false;
}

Dresser::~Dresser()
= default;

void Dresser::Draw()
{
	TextureManager::Instance().Draw("dresser", GetTransform()->position,
		this, 0, 255, false, SDL_FLIP_NONE);
}

void Dresser::Update()
{
}

void Dresser::Clean()
{
}

