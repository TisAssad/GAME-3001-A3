#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"
#include "InventoryManager.h"
#include "TextureManager.h"

StartScene::StartScene()
{
	StartScene::Start();
}

StartScene::~StartScene()
= default;

void StartScene::Draw()
{
	TextureManager::Instance().Draw("bg_start", 0, 0);
	DrawDisplayList();
}

void StartScene::Update()
{
	UpdateDisplayList();
}

void StartScene::Clean()
{
	RemoveAllChildren();
}

void StartScene::HandleEvents()
{
	EventManager::Instance().Update();

	// Keyboard Events
	if(EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if(EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		//Game::Instance().ChangeSceneState(SceneState::TEMPLATE);
	}
}

void StartScene::Start()
{

	TextureManager::Instance().Load("../Assets/textures/backgrounds/backGroundstart.png", "bg_start");

	// Music
	SoundManager::Instance().Load("../Assets/audio/theme.mp3", "titleTheme", SoundType::SOUND_MUSIC);
	//SoundManager::Instance().PlayMusic("titleTheme");
	SoundManager::Instance().SetMusicVolume(40);

	// Start Button
	m_pStartButton = new Button();
	m_pStartButton->GetTransform()->position = glm::vec2(400.0f, 400.0f); 

	m_pStartButton->AddEventListener(Event::CLICK, [&]()-> void
	{
		m_pStartButton->SetActive(false);
		Game::Instance().ChangeSceneState(SceneState::TUTORIAL);
	});
	
	m_pStartButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
		m_pStartButton->SetAlpha(128);
	});

	m_pStartButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
		m_pStartButton->SetAlpha(255);
	});
	AddChild(m_pStartButton);

	const SDL_Color white = { 255, 255, 255, 255 };

	m_pInstructionsLabel = new Label("AI Assignment 3",
		"Daydream", 20, white, glm::vec2(400.0f, 100.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel, 1);


	m_pInstructionsLabel = new Label("By Raven Stinson 101405611 and",
		"Daydream", 20, white, glm::vec2(400.0f, 150.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel, 1);

	m_pInstructionsLabel = new Label("Assad Ali 101377166 and Jakob Wax 101406885",
		"Daydream", 20, white, glm::vec2(400.0f, 200.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel, 1);

	// Initialize all Inventory Items
	InventoryManager::Instance().SetInventory("bedroomkey", false);
	InventoryManager::Instance().SetInventory("bedroommaster", false);

	ImGuiWindowFrame::Instance().SetDefaultGuiFunction();
}

