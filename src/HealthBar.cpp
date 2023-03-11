#include "HealthBar.h"

#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"


HealthBar::HealthBar(const glm::vec2 position)
{
	//GetTransform()->scale *= 0.5f;
	GetTransform()->position = position;
	SetWidth(150);
	SetHeight(50);
	SetType(GameObjectType::BACKGROUND);
	GetRigidBody()->isColliding = false;
	m_health = 140.0;
}

HealthBar::~HealthBar()
= default;

void HealthBar::Draw()
{
	Util::DrawRect(GetTransform()->position, GetWidth(), GetHeight(), { 128,128,128,255 });
	Util::DrawRect(GetTransform()->position + glm::vec2(5,5) , m_health, GetHeight() - 5, {0,255,0,255});
}

void HealthBar::Update()
{
}

void HealthBar::Clean()
{
}

void HealthBar::TakeDamage(const double damage)
{
	m_health -= damage;
}

double HealthBar::GetHealth() const
{
	return m_health;
}

