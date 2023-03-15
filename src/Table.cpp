#include "Table.h"

#include "SoundManager.h"
#include "TextureManager.h"


Table::Table()
{
	TextureManager::Instance().Load("../Assets/textures/furniture/table.png", "table");

	const auto size = TextureManager::Instance().GetTextureSize("table");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));

	SetType(GameObjectType::OBSTACLE);
	GetRigidBody()->isColliding = false;
}

Table::~Table()
= default;

void Table::Draw()
{
	TextureManager::Instance().Draw("table", 
		GetTransform()->position, 0, 255);
}

void Table::Update()
{
}

void Table::Clean()
{
}

