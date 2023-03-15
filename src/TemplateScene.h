#pragma once
#ifndef __TEMPLATE_SCENE__
#define __TEMPLATE_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Label.h"
#include "Renderer.h"
#include "InteractionObject.h"

class TemplateScene : public Scene
{
public:
	TemplateScene();
	~TemplateScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;

	/*void TextBox(const std::string& text, const std::string& font_name, const int font_size, 
		const SDL_Color colour, const glm::vec2 position, const int font_style, const bool is_centered);

	void GetTextBox(std::string& info);*/

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Player* m_pPlayer;
	bool m_melee = true;
	bool m_left, m_right, m_up, m_down;

	// UI Items
	bool m_debugView = false;

	// Text Box vars
	bool m_textBoxOnScreen = false;
	bool m_changeState = false;
	int m_childrenWithoutTB;
	std::string m_textBoxText;
};

#endif /* defined (__PLAY_SCENE__) */