#include "Player.h"
#include "TextureManager.h"
#include "Util.h"


Player::Player() : m_currentAnimationState(PlayerAnimationState::PLAYER_IDLE_DOWN)
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/player.txt",
		"../Assets/sprites/player.png",
		"spritesheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("spritesheet"));

	// set scale, passed into the dimentions and the texture manager
	GetTransform()->scale = {2.0f, 2.0f};
	
	// set frame width
	SetWidth(20 * GetTransform()->scale.x);

	// set frame height
	SetHeight(25 * GetTransform()->scale.y);

	GetTransform()->position = glm::vec2(400.0f, 300.0f);
	GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	GetRigidBody()->isColliding = false;
	SetType(GameObjectType::PLAYER);

	BuildAnimations();
}

Player::~Player()
= default;

void Player::Draw()
{
	// alias for x and y
	const auto x = static_cast<int>(GetTransform()->position.x);
	const auto y = static_cast<int>(GetTransform()->position.y);

	// draw the player according to animation state
	switch (m_currentAnimationState)
	{
	case PlayerAnimationState::PLAYER_IDLE_DOWN:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idleDown"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_LEFT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idleLeft"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_UP:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idleUp"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_IDLE_RIGHT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idleRight"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_RUN_RIGHT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runRight"),
			x, y, 0.4f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_RUN_DOWN:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runDown"),
			x, y, 0.40f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_RUN_LEFT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runLeft"),
			x, y, 0.40f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_RUN_UP:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runUp"),
			x, y, 0.4f, GetTransform()->scale, 0, 255, true);
		break;
	case PlayerAnimationState::PLAYER_SWING_RIGHT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("attackRight"),
				x, y, 0.4f, GetTransform()->scale, 0, 255, true);
		if (++m_attackAnimTimer % 15 == 0)
		{
			m_attackAnimTimer = 0;
			m_isPlayerAttacking = false;
			GetAnimation("attackRight").current_frame = 0;
			GetParent()->RemoveChild(m_pHitBox);
			if(m_isPlayerMoving)
			{
				SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			}else
			{
				SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			
		}
		break;
	case PlayerAnimationState::PLAYER_SWING_UP:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("attackUp"),
		                                         x, y, 0.4f, GetTransform()->scale, 0, 255, true);
		if (++m_attackAnimTimer % 15 == 0)
		{
			m_attackAnimTimer = 0;
			m_isPlayerAttacking = false;
			GetAnimation("attackUp").current_frame = 0;
			GetParent()->RemoveChild(m_pHitBox);
			m_pHealthBar->SetEnabled(true);
			if (m_isPlayerMoving)
			{
				SetAnimationState(PlayerAnimationState::PLAYER_RUN_UP);
			}
			else
			{
				SetAnimationState(PlayerAnimationState::PLAYER_IDLE_UP);
			}
		}
		break;
	case PlayerAnimationState::PLAYER_SWING_LEFT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("attackLeft"),
		                                         x, y, 0.4f, GetTransform()->scale, 0, 255, true);
		if (++m_attackAnimTimer % 15 == 0)
		{
			m_attackAnimTimer = 0;
			m_isPlayerAttacking = false;
			GetAnimation("attackLeft").current_frame = 0;
			GetParent()->RemoveChild(m_pHitBox);
			if (m_isPlayerMoving)
			{
				SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			}
			else
			{
				SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
		break;
	case PlayerAnimationState::PLAYER_SWING_DOWN:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("attackDown"),
		                                         x, y, 0.4f, GetTransform()->scale, 0, 255, true);
		if (++m_attackAnimTimer % 15 == 0)
		{
			m_attackAnimTimer = 0;
			m_isPlayerAttacking = false;
			GetAnimation("attackDown").current_frame = 0;
			GetParent()->RemoveChild(m_pHitBox);
			if (m_isPlayerMoving)
			{
				SetAnimationState(PlayerAnimationState::PLAYER_RUN_DOWN);
			}
			else
			{
				SetAnimationState(PlayerAnimationState::PLAYER_IDLE_DOWN);
			}
		}
		break;
	}

	m_pHealthBar->Draw();
}

void Player::Update()
{
	if (m_isPlayerMoving && !GetRigidBody()->isColliding) {

		if (GetPlayerDirection() == PlayerDirection::RIGHT) {
			GetRigidBody()->velocity = glm::vec2(5.0f, 0.0f);
		}
		else if (GetPlayerDirection() == PlayerDirection::LEFT) {
			GetRigidBody()->velocity = glm::vec2(-5.0f, 0.0f);
		}
		else if (GetPlayerDirection() == PlayerDirection::UP) {
			GetRigidBody()->velocity = glm::vec2(0.0f, -5.0f);
		}
		else if (GetPlayerDirection() == PlayerDirection::DOWN) {
			GetRigidBody()->velocity = glm::vec2(0.0f, 5.0f);
		}
	}

	GetTransform()->position += GetRigidBody()->velocity;

	// Health Bar Movement
	m_pHealthBar->GetTransform()->position = GetTransform()->position - glm::vec2(40.0f, 45.0f);

	// Hitbox movement
	if (m_isPlayerAttacking)
	{
		m_pHitBox->GetTransform()->position += GetRigidBody()->velocity;
	}
}

void Player::Clean()
{
}

void Player::SetAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::SetPlayerDirection(PlayerDirection newDirection)
{
	m_currentDirection = newDirection;
}

void Player::SetMovement(bool isMoving)
{
	m_isPlayerMoving = isMoving;
}


PlayerDirection Player::GetPlayerDirection()
{
	return m_currentDirection;
}

bool Player::GetMovement()
{
	return m_isPlayerMoving;
}

bool Player::GetAttacking()
{
	return m_isPlayerAttacking;
}

void Player::InitHPBar()
{
	// Initialize health bar
	m_pHealthBar = new HealthBar(GetTransform()->position);
	GetParent()->AddChild(m_pHealthBar);
}

HealthBar* Player::GetHPBar()
{
	return m_pHealthBar;
}

void Player::Attack()
{
	m_isPlayerAttacking = true;
	switch(m_currentDirection)
	{
	case PlayerDirection::LEFT:
		SetAnimationState(PlayerAnimationState::PLAYER_SWING_LEFT);
		m_pHitBox = new InteractionObject(GetWidth(), GetHeight());
		m_pHitBox->GetTransform()->position = 
			glm::vec2(GetTransform()->position.x - GetWidth() - GetWidth() / 2, 
				GetTransform()->position.y - GetHeight() / 2);
		GetParent()->AddChild(m_pHitBox);
		break;
	case PlayerDirection::RIGHT:
		SetAnimationState(PlayerAnimationState::PLAYER_SWING_RIGHT);
		m_pHitBox = new InteractionObject(GetWidth(), GetHeight());
		m_pHitBox->GetTransform()->position =
			glm::vec2(GetTransform()->position.x + GetWidth() - GetWidth() / 2, 
				GetTransform()->position.y - GetHeight() / 2);
		GetParent()->AddChild(m_pHitBox);
		break;
	case PlayerDirection::UP:
		SetAnimationState(PlayerAnimationState::PLAYER_SWING_UP);
		m_pHitBox = new InteractionObject(GetHeight(), GetWidth());
		m_pHitBox->GetTransform()->position =
			glm::vec2(GetTransform()->position.x - GetWidth() / 2, 
				GetTransform()->position.y - GetHeight());
		GetParent()->AddChild(m_pHitBox);
		m_pHealthBar->SetEnabled(false);
		break;
	case PlayerDirection::DOWN:
		SetAnimationState(PlayerAnimationState::PLAYER_SWING_DOWN);
		m_pHitBox = new InteractionObject(GetHeight(), GetWidth());
		m_pHitBox->GetTransform()->position =
			glm::vec2(GetTransform()->position.x - GetWidth() / 2, 
				GetTransform()->position.y + GetHeight() - GetHeight() / 2);
		GetParent()->AddChild(m_pHitBox);
		break;
	}
}

void Player::BuildAnimations()
{
	//Idle
	auto idleRight_animation = Animation();
	idleRight_animation.name = "idleRight";
	idleRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-idle-0"));
	SetAnimation(idleRight_animation);

	auto idleUp_animation = Animation();
	idleUp_animation.name = "idleUp";
	idleUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-idle-1"));
	SetAnimation(idleUp_animation);

	auto idleLeft_animation = Animation();
	idleLeft_animation.name = "idleLeft";
	idleLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-idle-2"));
	SetAnimation(idleLeft_animation);

	auto idleDown_animation = Animation();
	idleDown_animation.name = "idleDown";
	idleDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-idle-3"));
	SetAnimation(idleDown_animation);

	// Runing
	auto runRight_animation = Animation();
	runRight_animation.name = "runRight";
	runRight_animation.frames.push_back(idleRight_animation.frames[0]);
	runRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-right-0"));
	runRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-right-1"));
	runRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-right-2"));
	runRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-right-3"));
	runRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-right-4"));
	SetAnimation(runRight_animation);

	auto runUp_animation = Animation();
	runUp_animation.name = "runUp";
	runUp_animation.frames.push_back(idleUp_animation.frames[0]);
	runUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-up-0"));
	runUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-up-1"));
	runUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-up-2"));
	runUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-up-3"));
	runUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-up-4"));
	SetAnimation(runUp_animation);

	auto runLeft_animation = Animation();
	runLeft_animation.name = "runLeft";
	runLeft_animation.frames.push_back(idleLeft_animation.frames[0]);
	runLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-left-0"));
	runLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-left-1"));
	runLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-left-2"));
	runLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-left-3"));
	runLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-left-4"));
	SetAnimation(runLeft_animation);

	auto runDown_animation = Animation();
	runDown_animation.name = "runDown";
	runDown_animation.frames.push_back(idleDown_animation.frames[0]);
	runDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-down-0"));
	runDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-down-1"));
	runDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-down-2"));
	runDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-down-3"));
	runDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("player-down-4"));
	SetAnimation(runDown_animation);

	auto attackRight_animation = Animation();
	attackRight_animation.name = "attackRight";
	attackRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-right-0"));
	attackRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-right-1"));
	attackRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-right-2"));
	attackRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-right-3"));
	SetAnimation(attackRight_animation);

	auto attackUp_animation = Animation();
	attackUp_animation.name = "attackUp";
	attackUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-up-0"));
	attackUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-up-1"));
	attackUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-up-2"));
	attackUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-up-3"));
	SetAnimation(attackUp_animation);

	auto attackLeft_animation = Animation();
	attackLeft_animation.name = "attackLeft";
	attackLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-left-0"));
	attackLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-left-1"));
	attackLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-left-2"));
	attackLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-left-3"));
	SetAnimation(attackLeft_animation);

	auto attackDown_animation = Animation();
	attackDown_animation.name = "attackDown";
	attackDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-down-0"));
	attackDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-down-1"));
	attackDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-down-2"));
	attackDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("attack-down-3"));
	SetAnimation(attackDown_animation);
	
}
