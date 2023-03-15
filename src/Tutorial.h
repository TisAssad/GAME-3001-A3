#pragma once
#ifndef __TUTORIAL__
#define __TUTORIAL__

#include "Scene.h"
#include "Label.h"
#include "Button.h"

class Tutorial : public Scene
{
public:
	Tutorial();
	~Tutorial() override;

	// Inherited via Scene
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

private:

	Label* m_pStartLabel{};
	Label* m_pInstructionsLabel{};

};

#endif /* defined (__START_SCENE__) */
