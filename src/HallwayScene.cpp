#include "HallwayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

HallwayScene::HallwayScene(int spawnDoor)
{
	HallwayScene::Start();
}

HallwayScene::~HallwayScene()
= default;

void HallwayScene::Draw()
{
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 255, 0, 255);
	DrawDisplayList();
	if (m_debugView) { // basic debug, shows bounding boxes
		SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 255, 0, 255);
		Util::DrawRect(glm::vec2(170, 535),
			570 - 170 + m_pPlayer->GetWidth(), 675 - 535 + m_pPlayer->GetHeight());

		for(const auto display_object : GetDisplayList())
		{
			if(display_object->GetType() == GameObjectType::TEXT_BOX)
			{
				continue;
			}
			Util::DrawRect(glm::vec2(display_object->GetTransform()->position.x,
			display_object->GetTransform()->position.y),
			display_object->GetWidth(), display_object->GetHeight());
		}
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 0, 0, 255);
}

void HallwayScene::Update()
{
	// Wall collision
	bool wallCollsion = false;
	if(m_pPlayer->GetTransform()->position.x < 170 || m_pPlayer->GetTransform()->position.x > 570
		|| m_pPlayer->GetTransform()->position.y < 535 
		|| m_pPlayer->GetTransform()->position.y > 675)
	{
		wallCollsion = true;
	}

	// object collision
	// basic aabb checks between the player and all gameobjects
	// can be optimized

	m_pPlayer->GetRigidBody()->isColliding = wallCollsion;

	if(m_pPlayer->GetRigidBody()->isColliding)
	{
		switch(m_pPlayer->GetPlayerDirection())
		{
		case PlayerDirection::RIGHT:
			m_pPlayer->GetRigidBody()->velocity.x = -5.0f;
			break;
		case PlayerDirection::LEFT:
			//m_pPlayer->SetPlayerDirection(PlayerDirection::RIGHT);
			m_pPlayer->GetRigidBody()->velocity.x = 5.0f;
			break;
		case PlayerDirection::UP:
			m_pPlayer->GetRigidBody()->velocity.y = 5.0f;
			break;
		case PlayerDirection::DOWN:
			m_pPlayer->GetRigidBody()->velocity.y = -5.0f;
			break;
		default:
			break;
		}
	}
	
	UpdateDisplayList();
}

void HallwayScene::Clean()
{
	RemoveAllChildren();
}

void HallwayScene::HandleEvents()
{
	EventManager::Instance().Update();

	// handle player movement if no Game Controllers found & there is no text box on screen
	if (SDL_NumJoysticks() < 1 && !m_textBoxOnScreen) 
	{
		bool screenMovement = false;
		// movement, can be simply copied over between all scenes
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			if (!m_pPlayer->GetRigidBody()->isColliding) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::LEFT);
				
				if (m_pPlayer->GetTransform()->position.x <= Config::SCREEN_WIDTH / 2
					&& m_pBackground->GetTransform()->position.x <= 0)
				{
					m_pPlayer->GetRigidBody()->velocity = { 0,0 };
					m_pBackground->GetTransform()->position.x += 5;
					m_pDoor->GetTransform()->position.x += 5;
					m_pDoor2->GetTransform()->position.x += 5;
					m_pDoor3->GetTransform()->position.x += 5;
					m_pDoor4->GetTransform()->position.x += 5;
					m_pDoor5->GetTransform()->position.x += 5;
					screenMovement = true;
					m_pPlayer->SetMovement(false);
				}else
				{
					m_pPlayer->SetMovement(true);
				}
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			if (!m_pPlayer->GetRigidBody()->isColliding) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::RIGHT);
				if (m_pPlayer->GetTransform()->position.x >= Config::SCREEN_WIDTH / 2
					&& m_pBackground->GetTransform()->position.x >= -800)
				{
					m_pPlayer->GetRigidBody()->velocity = { 0,0 };
					m_pBackground->GetTransform()->position.x -= 5;
					m_pDoor->GetTransform()->position.x -= 5;
					m_pDoor2->GetTransform()->position.x -= 5;
					m_pDoor3->GetTransform()->position.x -= 5;
					m_pDoor4->GetTransform()->position.x -= 5;
					m_pDoor5->GetTransform()->position.x -= 5;
					screenMovement = true;
					m_pPlayer->SetMovement(false);
				}else
				{
					m_pPlayer->SetMovement(true);
				}
			}
			
		}

		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_W))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_UP);
			if (!m_pPlayer->GetRigidBody()->isColliding) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::UP);
				m_pPlayer->SetMovement(true);
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_S))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_DOWN);
			if (!m_pPlayer->GetRigidBody()->isColliding) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::DOWN);
				m_pPlayer->SetMovement(true);
			}

		}

		if (!m_pPlayer->GetMovement() && !screenMovement) // idle state 
		{
			m_pPlayer->GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
			if (m_pPlayer->GetPlayerDirection() == PlayerDirection::RIGHT)
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else if (m_pPlayer->GetPlayerDirection() == PlayerDirection::UP){
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_UP);
			}
			else if (m_pPlayer->GetPlayerDirection() == PlayerDirection::DOWN){
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_DOWN);
			}
			else
			{
				m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}

	//std::cout << "Player Moving: " << (m_pPlayer->GetMovement() ? "yes" : "no") << std::endl;

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		// should probably add pause screen here
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::TEMPLATE);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}

	// Object interaction
	// Place text box related interaction within the first if statement
	// IMPORTANT: ensure that you set m_textBoxOnScreen = true
	// without this the text boxes won't work
	if(EventManager::Instance().KeyPressed(SDL_SCANCODE_SPACE))
	{
		if (!m_textBoxOnScreen) { // If there isn't a text box
			if(m_pDoor->ObjectInteration(m_pPlayer)) // interaction object, the door
			{
				GetTextBox("The door is locked", glm::vec2{ 0, 680 }, 5);
				m_textBoxOnScreen = true; 
			}

			if (m_pDoor2->ObjectInteration(m_pPlayer)) // interaction object, the door
			{
				GetTextBox("The door to your room", glm::vec2{ 0, 680 }, 5);
				m_changeTo = 2;
				
				m_textBoxOnScreen = true;
			}

			if (m_pDoor3->ObjectInteration(m_pPlayer)) // interaction object, the door
			{
				GetTextBox("The door is locked", glm::vec2{ 0, 680 }, 5);
				m_textBoxOnScreen = true;
			}

			if (m_pDoor4->ObjectInteration(m_pPlayer)) // interaction object, the door
			{
				GetTextBox("The door is locked", glm::vec2{ 0, 680 }, 5);
				m_textBoxOnScreen = true;
			}

			if (m_pDoor5->ObjectInteration(m_pPlayer)) // interaction object, the door
			{
				GetTextBox("You open the door and see a light", glm::vec2{ 0, 680 }, 7);
				GetTextBox("Stepping in your mind starts to drift away...", glm::vec2{ 0, 680 }, 5);
				m_changeTo = 5;
				m_textBoxOnScreen = true;
			}
		}
		else if(GetDisplayList().size() != m_childrenWithoutTB) // if there is a text box on screen
		{
			// Removing 2 children, the text and the box
			RemoveChild(GetDisplayList().back());
			RemoveChild(GetDisplayList().back());
			if(GetDisplayList().size() == m_childrenWithoutTB) // if there isn't a text box
			{
				m_textBoxOnScreen = false; // reset the if statement
				switch (m_changeTo)
				{
				case 1:
					//Game::Instance().ChangeSceneState(BATHROOM)
					break;
				case 2:Game::Instance().ChangeSceneState(SceneState::TEMPLATE);
					break;
				case 3:
					//Game::Instance().ChangeSceneState(BASEMENT)
					break;
				case 4:
					//Game::Instance().ChangeSceneState(KITCHEN)
					break;
				case 5:
					//Game::Instance().ChangeSceneState(LIVING_ROOM)
					Game::Instance().ChangeSceneState(SceneState::END);
					break;
				}
			}
		}
		else // default
		{
			std::cout << "Not within an interaction radius" << std::endl;
		}
	}
	
}

void HallwayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Always draw background first
	m_pBackground = new Background;
	AddChild(m_pBackground, 0);
	m_pBackground->GetTransform()->position = { 0,0 };

	// interaction objects for the doors
	m_pDoor = new InteractionObject(100, 50);
	AddChild(m_pDoor, 0);
	m_pDoor->GetTransform()->position = glm::vec2(100, 580);

	m_pDoor2 = new InteractionObject(100, 100);
	AddChild(m_pDoor2, 0);
	m_pDoor2->GetTransform()->position = glm::vec2(320, 480);

	m_pDoor3 = new InteractionObject(100, 100);
	AddChild(m_pDoor3, 0);
	m_pDoor3->GetTransform()->position = glm::vec2(720, 480);

	m_pDoor4 = new InteractionObject(100, 100);
	AddChild(m_pDoor4, 0);
	m_pDoor4->GetTransform()->position = glm::vec2(1155, 480);

	m_pDoor5 = new InteractionObject(100, 50);
	AddChild(m_pDoor5, 0);
	m_pDoor5->GetTransform()->position = glm::vec2(1430, 580);

	// Player Sprite
	m_pPlayer = new Player();
	AddChild(m_pPlayer, 1);
	m_pPlayer->SetPlayerDirection(PlayerDirection::DOWN);
	m_pPlayer->GetRigidBody()->isColliding = false;
	m_pPlayer->GetTransform()->position = glm::vec2(400,600);

	m_childrenWithoutTB = GetDisplayList().size(); // need to incremented any time you add a new display object that isnt a textbox

	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&HallwayScene::GUI_Function, this));
}

void HallwayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001_Lab1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	if(ImGui::Button("Reset Player")) // simple position reset
	{
		m_pPlayer->GetTransform()->position = glm::vec2(400.0f, 600.0f);
	}

	ImGui::Separator();

	static bool debug = m_debugView; // debug view
	if (ImGui::Checkbox("Toggle Debug View", &debug))
	{
		m_debugView = debug;
	}

	float float2[2] = { m_pPlayer->GetTransform()->position.x, m_pPlayer->GetTransform()->position.y };

	if (m_pPlayer->GetTransform()->position.x == float2[0] && m_pPlayer->GetTransform()->position.y == float2[1]) // Checking if the GUI sliders are moving
	{
		m_pPlayer->SetMovement(false);
	}

	// slider player movement
	if(ImGui::SliderFloat2("Player Position", float2, 0.0f, 800.0f))
	{
		if(m_pPlayer->GetTransform()->position.x > float2[0]) // X movement 
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_pPlayer->SetMovement(true);
		}
		else if(m_pPlayer->GetTransform()->position.x < float2[0])
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_pPlayer->SetMovement(true);
		}

		if (m_pPlayer->GetTransform()->position.y > float2[1]) // Y movement, this could be one if statement, but two would be needed if there were animations for up and down
		{
			m_pPlayer->SetMovement(true);
		}
		else if (m_pPlayer->GetTransform()->position.y < float2[1])
		{
			m_pPlayer->SetMovement(true);
		}

		m_pPlayer->GetTransform()->position = glm::vec2(float2[0], float2[1]);
		
	}
	ImGui::End();
}
