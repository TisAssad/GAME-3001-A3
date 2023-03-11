#include "Player.h"
#include "TextureManager.h"
#include "Util.h"


Player::Player() : m_currentAnimationState(PlayerAnimationState::PLAYER_IDLE_DOWN)
{
	TextureManager::Instance().LoadSpriteSheet(
		"../Assets/sprites/yume_spritesheet.txt",
		"../Assets/sprites/yume_spritesheet.png",
		"spritesheet");

	SetSpriteSheet(TextureManager::Instance().GetSpriteSheet("spritesheet"));

	// set scale, passed into the dimentions and the texture manager
	GetTransform()->scale = {2.0f, 2.0f};
	
	// set frame width
	SetWidth(32 * GetTransform()->scale.x);

	// set frame height
	SetHeight(32 * GetTransform()->scale.y);

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
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;
	case PlayerAnimationState::PLAYER_IDLE_LEFT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idleLeft"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;
	case PlayerAnimationState::PLAYER_IDLE_UP:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idleUp"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;
	case PlayerAnimationState::PLAYER_IDLE_RIGHT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("idleRight"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;

	case PlayerAnimationState::PLAYER_RUN_DOWN:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runDown"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;
	case PlayerAnimationState::PLAYER_RUN_LEFT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runLeft"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;
	case PlayerAnimationState::PLAYER_RUN_UP:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runUp"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;
	case PlayerAnimationState::PLAYER_RUN_RIGHT:
		TextureManager::Instance().PlayAnimation("spritesheet", GetAnimation("runRight"),
			x, y, 0.20f, GetTransform()->scale, 0, 255, false);
		break;
	default:
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

void Player::InitHPBar()
{
	// Initialize health bar
	m_pHealthBar = new HealthBar(GetTransform()->position - glm::vec2(50.0f, 0.0f));
	GetParent()->AddChild(m_pHealthBar);
}

HealthBar* Player::GetHPBar()
{
	return m_pHealthBar;
}

void Player::BuildAnimations()
{
	//Idle
	auto idleDown_animation = Animation();
	idleDown_animation.name = "idleDown";
	idleDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-idle-0"));
	SetAnimation(idleDown_animation);

	auto idleUp_animation = Animation();
	idleUp_animation.name = "idleUp";
	idleUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-idle-1"));
	SetAnimation(idleUp_animation);

	auto idleLeft_animation = Animation();
	idleLeft_animation.name = "idleLeft";
	idleLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-idle-2"));
	SetAnimation(idleLeft_animation);

	auto idleRight_animation = Animation();
	idleRight_animation.name = "idleRight";
	idleRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-idle-3"));
	SetAnimation(idleRight_animation);

	// Runing
	auto runDown_animation = Animation();
	runDown_animation.name = "runDown";
	runDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-0"));
	runDown_animation.frames.push_back(idleDown_animation.frames[0]);
	runDown_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-1"));
	runDown_animation.frames.push_back(idleDown_animation.frames[0]);
	SetAnimation(runDown_animation);

	auto runUp_animation = Animation();
	runUp_animation.name = "runUp";
	runUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-2"));
	runUp_animation.frames.push_back(idleUp_animation.frames[0]);
	runUp_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-3"));
	runUp_animation.frames.push_back(idleUp_animation.frames[0]);
	SetAnimation(runUp_animation);

	auto runLeft_animation = Animation();
	runLeft_animation.name = "runLeft";
	runLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-4"));
	runLeft_animation.frames.push_back(idleLeft_animation.frames[0]);
	runLeft_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-5"));
	runLeft_animation.frames.push_back(idleLeft_animation.frames[0]);
	SetAnimation(runLeft_animation);

	auto runRight_animation = Animation();
	runRight_animation.name = "runRight";
	runRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-6"));
	runRight_animation.frames.push_back(idleRight_animation.frames[0]);
	runRight_animation.frames.push_back(GetSpriteSheet()->GetFrame("yume-run-7"));
	runRight_animation.frames.push_back(idleRight_animation.frames[0]);
	SetAnimation(runRight_animation);
}
