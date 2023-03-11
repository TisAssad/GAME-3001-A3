#include "TemplateScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "Util.h"

TemplateScene::TemplateScene()
{
	TemplateScene::Start();
}

TemplateScene::~TemplateScene()
= default;

void TemplateScene::Draw()
{
	TextureManager::Instance().Draw("bedroom", 0, 0);

	if (m_debugView) { // basic debug, shows bounding boxes
		SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 255, 0, 255);
		for (const auto display_object : GetDisplayList())
		{
			if (display_object->GetType() == GameObjectType::TEXT_BOX)
			{
				continue;
			}
			Util::DrawRect(glm::vec2(display_object->GetTransform()->position.x,
				display_object->GetTransform()->position.y),
				display_object->GetWidth(), display_object->GetHeight());

		}
	}
	DrawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 0, 0, 255);
}

void TemplateScene::Update()
{
	// Wall collision
	bool wallCollsion = false;
	if(m_pPlayer->GetTransform()->position.x < 100 || m_pPlayer->GetTransform()->position.x > 700 - 32
		|| m_pPlayer->GetTransform()->position.y < 265 
		|| m_pPlayer->GetTransform()->position.y > 725 - m_pPlayer->GetHeight())
	{
		wallCollsion = true;
	}

	// object collision
	// basic aabb checks between the player and all gameobjects
	// can be optimized
	bool collision1 = CollisionManager::AABBCheck(m_pBed, m_pPlayer);
	bool collision2 = CollisionManager::AABBCheck(m_pDresser, m_pPlayer);
	bool collision3 = CollisionManager::AABBCheck(m_pWardrobe, m_pPlayer);
	bool collision4 = CollisionManager::AABBCheck(m_pTable, m_pPlayer);

	m_pPlayer->GetRigidBody()->isColliding = collision1 || collision2 || collision3 || collision4 || wallCollsion;

	if(m_pPlayer->GetRigidBody()->isColliding)
	{
		switch(m_pPlayer->GetPlayerDirection())
		{
		case PlayerDirection::RIGHT:
			m_pPlayer->GetRigidBody()->velocity.x = -5.0f;
			m_right = true;
			break;
		case PlayerDirection::LEFT:
			m_pPlayer->GetRigidBody()->velocity.x = 5.0f;
			m_left = true;
			break;
		case PlayerDirection::UP:
			m_pPlayer->GetRigidBody()->velocity.y = 5.0f;
			m_up = true;
			break;
		case PlayerDirection::DOWN:
			m_down = true;
			m_pPlayer->GetRigidBody()->velocity.y = -5.0f;
			break;
		}
	}
	UpdateDisplayList();
}

void TemplateScene::Clean()
{
	RemoveAllChildren();
}

void TemplateScene::HandleEvents()
{
	EventManager::Instance().Update();

	if (m_pPlayer->GetTransform()->position.x > 800)
	{
		m_pPlayer->GetTransform()->position = glm::vec2(400.0f, 400.0f);
	}
	else if (m_pPlayer->GetTransform()->position.x < -100)
	{
		m_pPlayer->GetTransform()->position = glm::vec2(400.0f, 400.0f);
	}
	if (m_pPlayer->GetTransform()->position.y > 800)
	{
		m_pPlayer->GetTransform()->position = glm::vec2(400.0f, 400.0f);
	}
	else if (m_pPlayer->GetTransform()->position.y < -100)
	{
		m_pPlayer->GetTransform()->position = glm::vec2(400.0f, 400.0f);
	}

	//if (m_pPlayer->GetTransform()->position.y < -800 || m_pPlayer->GetTransform()->position.y > 1000);
	//{
	//	m_pPlayer->GetTransform()->position = glm::vec2(400.0f, 400.0f);
	//}

	std::cout << m_left << m_right << m_up << m_down << std::endl;

	// handle player movement if no Game Controllers found & there is no text box on screen
	if (SDL_NumJoysticks() < 1 && !m_textBoxOnScreen) 
	{
		// movement, can be simply copied over between all scenes
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			if (!m_pPlayer->GetRigidBody()->isColliding && !m_left) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::LEFT);
				m_pPlayer->SetMovement(true);
				m_right = m_up = m_down = false;
				
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			if (!m_pPlayer->GetRigidBody()->isColliding && !m_right) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::RIGHT);
				m_pPlayer->SetMovement(true);
				m_left = m_up = m_down = false;
			}
		}

		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_W))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_UP);
			if (!m_pPlayer->GetRigidBody()->isColliding && !m_up) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::UP);
				m_pPlayer->SetMovement(true);
				m_right = m_left = m_down = false;
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_S))
		{
			m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_DOWN);
			if (!m_pPlayer->GetRigidBody()->isColliding && !m_down) {
				m_pPlayer->SetPlayerDirection(PlayerDirection::DOWN);
				m_pPlayer->SetMovement(true);
				m_right = m_up = m_left = false;
			}

		} else
		{
			m_pPlayer->SetMovement(false);
		}

		if (!m_pPlayer->GetMovement()) // idle state 
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

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		// should probably add pause screen here
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

	// Object interaction
	// Place text box related interaction within the first if statement
	// IMPORTANT: ensure that you set m_textBoxOnScreen = true
	// without this the text boxes won't work
	if(EventManager::Instance().KeyPressed(SDL_SCANCODE_SPACE))
	{
		if (!m_textBoxOnScreen) { // If there isn't a text box
			if (m_pBed->ObjectInteration(m_pPlayer)) // bed interaction
			{
				GetTextBox("This is your bed", glm::vec2{ 0, 680 });
				m_textBoxOnScreen = true;
				std::cout << "Interacting with Bed" << std::endl;
			}
			else if (m_pDresser->ObjectInteration(m_pPlayer)) // dresser interaction
			{
				if (/*InventoryManager::Instance().GetInventoryMap().empty() || */!InventoryManager::Instance().GetInventory("bedroomkey")) // if the player doesn't have the key
				{
					GetTextBox("You find a key in the top shelf", glm::vec2{ 0, 680 });
					InventoryManager::Instance().SetInventory("bedroomkey", true); // set the roomkey to true
				}
				else { // default text
					GetTextBox("There's nothing else in here", glm::vec2{ 0, 680 }); 
				}
				std::cout << "Interacting with Dresser" << std::endl;
				m_textBoxOnScreen = true;
			}
			else if (m_pTable->ObjectInteration(m_pPlayer)) // table interaction
			{
				GetTextBox("This is your table", glm::vec2{ 0, 680 });
				std::cout << "Interacting with Table" << std::endl;
				m_textBoxOnScreen = true;
			}
			else if (m_pWardrobe->ObjectInteration(m_pPlayer)) // wardrobe interaction
			{
				GetTextBox("This is your wardrobe", glm::vec2{ 0, 680 }, 5); // Setting the layer index is important if you want more than 1 text box
				GetTextBox("Full of 15 of the same outfits, as it should be", glm::vec2{ 0, 680 }, 4); // highest is displayed first
				m_textBoxOnScreen = true;
				std::cout << "Interacting with Wardrobe" << std::endl;
			}
			else if(m_pIO->ObjectInteration(m_pPlayer)) // interaction object, the door
			{
				if (InventoryManager::Instance().GetInventory("bedroommaster"))
				{
					GetTextBox("The door to the hallway", glm::vec2{ 0, 680 }, 5);
					m_changeState = true;
				}
				else {
					GetTextBox("The door is locked", glm::vec2{ 0, 680 }, 5);
					if (InventoryManager::Instance().GetInventory("bedroomkey")) // if the player has the key
					{
						GetTextBox("You use the key to unlock the door", glm::vec2{ 0, 680 }, 4);
						m_changeState = true; // change state
						InventoryManager::Instance().SetInventory("bedroomkey", false);
						InventoryManager::Instance().SetInventory("bedroommaster", true);
					}
				}
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
				if (m_changeState) // if we are changing state
				{
					Game::Instance().ChangeSceneState(SceneState::HALLWAY); // this can be any state, ensure there is a case for it though
				}

			}
		}
		else // default
		{
			std::cout << "Not within an interaction radius" << std::endl;
		}
	}
	
}

void TemplateScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Always draw background first

	TextureManager::Instance().Load("../Assets/textures/backgrounds/bedroom.png", "bedroom");

	m_pBed = new Bed;
	AddChild(m_pBed, 0);
	m_pBed->GetTransform()->position = glm::vec2(100,250);

	m_pDresser = new Dresser;
	AddChild(m_pDresser, 0);
	m_pDresser->GetTransform()->position = glm::vec2(100 + m_pBed->GetWidth(), 250);
	m_pDresser->GetTransform()->position = glm::vec2(100 + m_pBed->GetWidth(), 250);

	m_pTable = new Table;
	AddChild(m_pTable, 0);
	m_pTable->GetTransform()->position = glm::vec2(500, 500);

	m_pWardrobe = new Wardrobe;
	AddChild(m_pWardrobe,0);
	m_pWardrobe->GetTransform()->position = glm::vec2(500, 150);

	// an interaction object
	// a simple object with no texture, and is only used for interaction
	m_pIO = new InteractionObject(100, 100);
	AddChild(m_pIO, 0);
	m_pIO->GetTransform()->position = glm::vec2(400, 700);

	// Player Sprite
	m_pPlayer = new Player();
	AddChild(m_pPlayer, 1);
	m_pPlayer->SetPlayerDirection(PlayerDirection::DOWN);
	m_pPlayer->GetRigidBody()->isColliding = false;
	m_pPlayer->GetTransform()->position = glm::vec2(400,400);
	m_pPlayer->InitHPBar();

	m_childrenWithoutTB = GetDisplayList().size(); // need to incremented any time you add a new display object that isnt a textbox

	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&TemplateScene::GUI_Function, this));
}

void TemplateScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001_Lab1", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	if(ImGui::Button("Reset Player")) // simple position reset
	{
		m_pPlayer->GetTransform()->position = glm::vec2(400.0f, 400.0f);
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
