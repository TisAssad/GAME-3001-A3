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
	TextureManager::Instance().Draw("ForestBG", 0, 0);
	if (m_debugView) { // basic debug, shows bounding boxes
		SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 255, 0, 255);
		for (const auto display_object : GetDisplayList())
		{
			if (display_object->GetType() == GameObjectType::TEXT_BOX 
				|| display_object->GetType() == GameObjectType::HEALTH_BAR
				|| display_object->GetType() == GameObjectType::PLAYER
				|| display_object->GetType() == GameObjectType::PATH_NODE)
			{
				continue;
			}
			Util::DrawRect(glm::vec2(display_object->GetTransform()->position.x,
				display_object->GetTransform()->position.y),
				display_object->GetWidth(), display_object->GetHeight());
		}
		Util::DrawRect({ m_pPlayer->GetTransform()->position.x - m_pPlayer->GetWidth() / 2,
			m_pPlayer->GetTransform()->position.y - m_pPlayer->GetHeight() / 2 },
			m_pPlayer->GetWidth(), m_pPlayer->GetHeight());
	}
	
	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 0, 0, 0, 255);
}

void TemplateScene::Update()
{
	// Wall collision
	bool wallCollsion = false;
	if(m_pPlayer->GetTransform()->position.x < 0 + m_pPlayer->GetWidth() / 2 || m_pPlayer->GetTransform()->position.x > 800 - m_pPlayer->GetWidth()/2
		|| m_pPlayer->GetTransform()->position.y < 0 + m_pPlayer->GetHeight() / 2
		|| m_pPlayer->GetTransform()->position.y > 800 - m_pPlayer->GetHeight()/2)
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

	//std::cout << m_left << m_right << m_up << m_down << std::endl;

	// handle player movement if no Game Controllers found & there is no text box on screen
	if (SDL_NumJoysticks() < 1 && !m_textBoxOnScreen && !m_pPlayer->GetAttacking()) 
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

	if(EventManager::Instance().KeyPressed(SDL_SCANCODE_E))
	{
		m_melee = !m_melee;
		m_pPlayer->SetMelee(m_melee);
	}
	if(EventManager::Instance().KeyPressed(SDL_SCANCODE_SPACE))
	{
		m_pPlayer->Attack(m_melee);
	}

	if(EventManager::Instance().MousePressed(1))
	{
		m_pPlayer->GetHPBar()->TakeDamage(6);
	}
	
}

void TemplateScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Always draw background first

	TextureManager::Instance().Load("../Assets/textures/backgrounds/bedroom.png", "bedroom");
	TextureManager::Instance().Load("../Assets/textures/backgrounds/ForestBG.png", "ForestBG");
	// an interaction object
	// a simple object with no texture, and is only used for interaction
	

	// Player Sprite

	m_pPlayer = new Player();
	AddChild(m_pPlayer, 1);
	m_pPlayer->SetPlayerDirection(PlayerDirection::DOWN);
	m_pPlayer->GetRigidBody()->isColliding = false;
	m_pPlayer->GetTransform()->position = glm::vec2(400,400);
	m_pPlayer->InitHPBar();

	m_buildGrid();

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
		m_toggleGrid(debug);
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

void TemplateScene::m_buildGrid()
{
	constexpr auto tile_size = Config::TILE_SIZE;
	constexpr auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	m_clearNodes(); // we will need to clear nodes because we will rebuild/redraw the grid if we move an obstacle

	// lay out a grid of path_nodes
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->GetTransform()->position = glm::vec2(static_cast<float>(col) * tile_size + offset.x,
				static_cast<float>(row) * tile_size + offset.y);

			// only show grid where there are no obstacles
			bool keep_node = true;
			//for (const auto obstacle : m_pObstacles)
			//{
			//	// determine which path_nodes to keep
			//	if (CollisionManager::AABBRadiusCheck(path_node, obstacle, 20))
			//	{
			//		keep_node = false;
			//	}
			//}
			if (keep_node)
			{
				AddChild(path_node);
				m_pGrid.push_back(path_node);
			}
			else
			{
				delete path_node;
			}
		}

		// if Grid is supposed to be hidden - make it so!
		m_toggleGrid(m_isGridEnabled);
	}
}

void TemplateScene::m_toggleGrid(const bool state) const
{
	for (const auto path_node : m_pGrid)
	{
		path_node->SetVisible(state);
	}
}

void TemplateScene::m_clearNodes()
{
	m_pGrid.clear();
	for (const auto display_object : GetDisplayList())
	{
		if (display_object->GetType() == GameObjectType::PATH_NODE)
		{
			RemoveChild(display_object);
		}
	}
}

bool TemplateScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const
{
	bool has_LOS = false;
	agent->SetHasLOS(has_LOS); // default - no LOS
	const glm::vec2 agent_LOS_endPoint = agent->GetTransform()->position + agent->GetCurrentDirection() * agent->GetLOSDistance();

	// if ship to target distance is less than or equal to the LOS Distance (Range)
	const auto agent_to_range = Util::GetClosestEdge(agent->GetTransform()->position, target_object);
	if (agent_to_range <= agent->GetLOSDistance())
	{
		// we are in range
		std::vector<DisplayObject*> contact_list;
		for (auto display_object : GetDisplayList())
		{
			const auto agent_to_object_distance = Util::GetClosestEdge(agent->GetTransform()->position, display_object);
			if (agent_to_object_distance > agent_to_range) { continue; }

			if ((display_object->GetType() != GameObjectType::AGENT)
				&& (display_object->GetType() != GameObjectType::PATH_NODE)
				&& display_object->GetType() != GameObjectType::TARGET)
			{
				contact_list.push_back(display_object);
			}
		}

		has_LOS = CollisionManager::LOSCheck(agent, agent_LOS_endPoint, contact_list, target_object);

		const auto LOSColour = (target_object->GetType() == GameObjectType::AGENT)
			? glm::vec4(0, 0, 1, 1)
			: glm::vec4(0, 1, 0, 1);
		agent->SetHasLOS(has_LOS, LOSColour);
	}
	return has_LOS;
}

bool TemplateScene::m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object) const
{
	// check angle to target so we can still use LOS Distance for path_nodes
	const auto target_direction = target_object->GetTransform()->position - path_node->GetTransform()->position;
	const auto normalized_direction = Util::Normalize(target_direction); // normalizes target direction
	path_node->SetCurrentDirection(normalized_direction);
	return m_checkAgentLOS(path_node, target_object);
}

void TemplateScene::m_checkAllNodesWithTarget(DisplayObject* target_object) const
{
	for (const auto path_node : m_pGrid)
	{
		m_checkPathNodeLOS(path_node, target_object);
	}
}

void TemplateScene::m_checkAllNodesWithBoth() const
{
	for (const auto path_node : m_pGrid)
	{
		const bool LOSWithStarship = m_checkPathNodeLOS(path_node, m_pPlayer);
		//const bool LOSWithTarget = m_checkPathNodeLOS(path_node, m_pTarget);
		path_node->SetHasLOS(LOSWithStarship /*&& LOSWithTarget*/, glm::vec4(0, 1, 1, 1));
	}
}

void TemplateScene::m_setPathNodeDistance(const int distance) const
{
	for (const auto path_node : m_pGrid)
	{
		path_node->SetLOSDistance(static_cast<float>(distance));
	}
}
