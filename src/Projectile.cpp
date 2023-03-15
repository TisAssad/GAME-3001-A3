#include "Projectile.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

Projectile::Projectile(bool fromPlayer, glm::vec2 position, int angle)
{
	TextureManager::Instance().Load("../Assets/textures/swordBeam.png", "swordBeam");
	m_fromPlayer = fromPlayer;
	SetCurrentHeading(angle);
	GetTransform()->position = position;

	

	if (m_fromPlayer) {
		GetTransform()->scale *= 2.0f;
		const auto size = TextureManager::Instance().GetTextureSize("swordBeam");
		SetWidth(static_cast<int>(size.x * GetTransform()->scale.x));
		SetHeight(static_cast<int>(size.y * GetTransform()->scale.y));
	}
	
}

Projectile::~Projectile()
= default;


void Projectile::Draw()
{
	
	if(m_fromPlayer)
	{
		TextureManager::Instance().Draw("swordBeam", GetTransform()->position,
			this, GetCurrentHeading());
	}
}

void Projectile::Clean()
{
}

void Projectile::Update()
{
	GetTransform()->position += GetCurrentDirection() * 5.0f;
}
