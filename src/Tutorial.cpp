#include "Tutorial.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "TextureManager.h"
#include "Util.h"


Tutorial::Tutorial()
{
	Start();
}

Tutorial::~Tutorial() = default;

void Tutorial::Draw()
{
	Util::DrawFilledRect(glm::vec2{ 0.0f, 0.0f }, 800.0f, 800.0f, glm::vec4{ 0, 0,0 ,1 });
	DrawDisplayList();
}

void Tutorial::Update()
{
	UpdateDisplayList();
}

void Tutorial::Clean()
{
	RemoveAllChildren();
}

void Tutorial::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_SPACE))
	{
		Game::Instance().ChangeSceneState(SceneState::TEMPLATE);
	}
}

void Tutorial::Start()
{
	const SDL_Color black = { 0, 0, 0, 255 };

	m_pInstructionsLabel = new Label("To play use W, A, S, D ",
		"Daydream", 20, black, glm::vec2(400.0f, 350.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel, 1);


	m_pInstructionsLabel = new Label("And mouse buttons to attack",
		"Daydream", 20, black, glm::vec2(400.0f, 400.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel, 1);

	m_pInstructionsLabel = new Label("Press space to continue",
		"Daydream", 20, black, glm::vec2(400.0f, 450.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel, 1);

	ImGuiWindowFrame::Instance().SetDefaultGuiFunction();
}

