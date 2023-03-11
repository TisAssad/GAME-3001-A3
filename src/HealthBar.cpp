#include "HealthBar.h"

#include "SoundManager.h"
#include "TextureManager.h"
#include "Util.h"


HealthBar::HealthBar(const glm::vec2 position)
{
	//GetTransform()->scale *= 0.5f;
	GetTransform()->position = position;
	SetWidth(76);
	SetHeight(20);
	SetType(GameObjectType::BACKGROUND);
	GetRigidBody()->isColliding = false;
	m_health = 66.0;
	
}

HealthBar::~HealthBar()
= default;

void HealthBar::Draw()
{
	//SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 128, 128, 128, 255);
	Util::DrawFilledRect(GetTransform()->position, GetWidth(), GetHeight(), { 128, 128, 128, 255 });
	//SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 255, 0, 255);
	Util::DrawFilledRect(GetTransform()->position + glm::vec2(6,6) , m_health, GetHeight() - 12, { 0, 255, 0, 255 });
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

