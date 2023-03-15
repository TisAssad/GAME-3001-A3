#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	if(m_pPlayer->GetTransform()->position.x > 800)
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}

void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_pPlayer->GetTransform()->position -= glm::vec2(5.0f, 0.0f);
			m_playerFacingRight = false;
			m_isPlayerMoving = true;
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_pPlayer->GetTransform()->position += glm::vec2(5.0f, 0.0f);
			m_playerFacingRight = true;
			m_isPlayerMoving = true;
		}

		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_W))
		{
			m_pPlayer->GetTransform()->position -= glm::vec2(0.0f, 5.0f);
			if(m_playerFacingRight == true)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			}
			m_isPlayerMoving = true;
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_S))
		{
			m_pPlayer->GetTransform()->position += glm::vec2(0.0f, 5.0f);
			if (m_playerFacingRight == true)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			}
			m_isPlayerMoving = true;
		}

		if (!m_isPlayerMoving) 
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}

	

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}

}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";


	// Player Sprite
	m_pPlayer = new Player();
	AddChild(m_pPlayer);
	m_playerFacingRight = true;


	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() const 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001_Lab1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	float float2[2] = { m_pPlayer->GetTransform()->position.x, m_pPlayer->GetTransform()->position.y };

	if (m_pPlayer->GetTransform()->position.x == float2[0] && m_pPlayer->GetTransform()->position.y == float2[1]) // Checking if the GUI sliders are moving
	{
		m_isPlayerMoving = false;
	}

	if(ImGui::SliderFloat2("Player Position", float2, 0.0f, 800.0f))
	{
		if(m_pPlayer->GetTransform()->position.x > float2[0]) // X movement 
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
			m_isPlayerMoving = true;
		}
		else if(m_pPlayer->GetTransform()->position.x < float2[0])
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
			m_isPlayerMoving = true;
		}

		if (m_pPlayer->GetTransform()->position.y > float2[1]) // Y movement, this could be one if statement, but two would be needed if there were animations for up and down
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			}
			m_isPlayerMoving = true;
		}
		else if (m_pPlayer->GetTransform()->position.y < float2[1])
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			}
			m_isPlayerMoving = true;
		}

		m_pPlayer->GetTransform()->position = glm::vec2(float2[0], float2[1]);
		
	}
	//float2[0] = m_pPlayer->GetTransform()->position.x;
	ImGui::End();
}
