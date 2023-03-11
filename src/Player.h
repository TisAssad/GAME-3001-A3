#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"
#include "PlayerDirection.h"
#include "InventoryManager.h"
#include "HealthBar.h"

class Player final : public Sprite
{
public:
	Player();
	~Player() override;

	// Life Cycle Methods
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	// setters
	void SetAnimationState(PlayerAnimationState new_state);
	void SetPlayerDirection(PlayerDirection newDirection);
	void SetMovement(bool isMoving);

	// getters
	PlayerDirection GetPlayerDirection();
	bool GetMovement();

	void InitHPBar();
	HealthBar* GetHPBar();

private:
	void BuildAnimations();

	PlayerAnimationState m_currentAnimationState;
	PlayerDirection m_currentDirection;
	HealthBar* m_pHealthBar;
	bool m_isPlayerMoving = false;

};

#endif
