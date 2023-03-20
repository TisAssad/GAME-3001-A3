#include "TextBoxBackground.h"

#include "SoundManager.h"
#include "TextureManager.h"


TextBoxBackground::TextBoxBackground()
{
	TextureManager::Instance().Load("../Assets/textures/TextBoxBackground.png", "tbb");

	//GetTransform()->scale *= 0.5f;

	const auto size = TextureManager::Instance().GetTextureSize("tbb");
	SetWidth(static_cast<int>(size.x * GetTransform()->scale.x));
	SetHeight(static_cast<int>(size.y * GetTransform()->scale.y));
	GetTransform()->position = glm::vec2(0, 680);

	SetType(GameObjectType::TEXT_BOX);
	GetRigidBody()->isColliding = false;
	
}

TextBoxBackground::~TextBoxBackground()
= default;

void TextBoxBackground::Draw()
{
	TextureManager::Instance().Draw("tbb", GetTransform()->position, 
		this, 0, 255, false, SDL_FLIP_NONE);
}

void TextBoxBackground::Update()
{
}

void TextBoxBackground::Clean()
{
}

