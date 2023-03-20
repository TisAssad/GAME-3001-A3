#pragma once
#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__
#include "DisplayObject.h"

class HealthBar final : public DisplayObject
{
public:
	// constructors
	HealthBar(glm::vec2 position);
	
	// destructor
	~HealthBar() override;
	
	// life cycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;

	// Util functions
	void TakeDamage(double damage);
	double GetHealth() const;
private:
	double m_health;
	
};

#endif