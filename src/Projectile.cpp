#include "Projectile.h"

#include "Game.h"
#include "TextureManager.h"
#include "Util.h"

Projectile::Projectile(bool fromPlayer, glm::vec2 position, int angle)
{
	TextureManager::Instance().Load("../Assets/textures/swordBeam.png", "swordBeam");
	m_fromPlayer = fromPlayer;
	if (m_fromPlayer) {
		GetTransform()->scale *= 2.0f;
		const auto size = TextureManager::Instance().GetTextureSize("swordBeam");
		SetWidth(static_cast<int>(size.x * GetTransform()->scale.x));
		SetHeight(static_cast<int>(size.y * GetTransform()->scale.y));
	}else
	{
		SetWidth(static_cast<int>(20));
		SetHeight(static_cast<int>(20));
	}
	
	SetCurrentHeading(angle);
	GetTransform()->position = position;
	//setIsCentered(false);
	GetRigidBody()->bounds = glm::vec2(GetWidth(), GetHeight());
	GetRigidBody()->isColliding = false;

	CollisionManager::RotateAABB(this, GetCurrentHeading());
}

Projectile::~Projectile()
= default;


void Projectile::Draw()
{
	
	if(m_fromPlayer)
	{
		TextureManager::Instance().Draw("swordBeam", GetTransform()->position,
			this);
	}
}

void Projectile::Clean()
{
}

void Projectile::Update()
{
	this->GetTransform()->position = this->GetTransform()->position + GetCurrentDirection() * 10.0f;
}
