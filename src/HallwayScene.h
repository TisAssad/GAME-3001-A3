#pragma once
#ifndef __HALLWAY_SCENE__
#define __HALLWAY_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Bed.h"
#include "Label.h"
#include "InteractionObject.h"
#include "Background.h"

class HallwayScene : public Scene
{
public:
	HallwayScene(int spawnDoor);
	~HallwayScene() override;

	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

	/*void TextBox(const std::string& text, const std::string& font_name, const int font_size, 
		const SDL_Color colour, const glm::vec2 position, const int font_style, const bool is_centered);

	void GetTextBox(std::string& info);*/

private:
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;

	InteractionObject* m_pDoor;
	InteractionObject* m_pDoor2;
	InteractionObject* m_pDoor3;
	InteractionObject* m_pDoor4;
	InteractionObject* m_pDoor5;
	Background* m_pBackground;

	// UI Items
	bool m_debugView = false;

	bool m_textBoxOnScreen = false;
	int m_childrenWithoutTB;
	std::string m_textBoxText;
	int m_changeTo;
};

#endif /* defined (__PLAY_SCENE__) */