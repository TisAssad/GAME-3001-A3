#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Player.h"


class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;
	mutable bool m_playerFacingRight;        // bools here are mutable, so that they can be used within the GUI_Function
	mutable bool m_isPlayerMoving = false;   // The other option is to make the GUI_Function not const, though I don't know if that would cause any unforseen problems

	// UI Items

};

#endif /* defined (__PLAY_SCENE__) */