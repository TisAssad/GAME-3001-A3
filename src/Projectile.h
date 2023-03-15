#pragma once
#ifndef __PROJECTILE__
#define __PROJECTILE__
#include "Agent.h"
class Projectile : public Agent
{
public:
	Projectile(bool fromPlayer, glm::vec2 position, int angle);
	~Projectile();

	void Draw() override;
	void Clean() override;
	void Update() override;

private:
	bool m_fromPlayer;
};
#endif
