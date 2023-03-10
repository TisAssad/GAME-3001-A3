#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"
#include "PlayerDirection.h"
#include <vector>
#include <map>
#include "InventoryManager.h"

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




private:
	void BuildAnimations();

	PlayerAnimationState m_currentAnimationState;
	PlayerDirection m_currentDirection;
	bool m_isPlayerMoving = false;


};

#endif
