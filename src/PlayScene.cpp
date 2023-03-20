//#include "PlayScene.h"
//#include "Game.h"
//#include "EventManager.h"
//#include <windows.h>
//#include <fstream>
//
//// required for IMGUI
//#include "imgui.h"
//#include "imgui_sdl.h"
//#include "Renderer.h"
//#include "Util.h"
//
//PlayScene::PlayScene()
//{
//	PlayScene::Start();
//}
//
//PlayScene::~PlayScene()
//= default;
//
//void PlayScene::Draw()
//{
//	DrawDisplayList();
//
//	if (m_isGridEnabled) {
//		for (const auto obstacle : m_pObstacles)
//		{
//			Util::DrawRect(obstacle->GetTransform()->position - glm::vec2(obstacle->GetWidth() * 0.5f,
//				obstacle->GetHeight() * 0.5f), obstacle->GetWidth(), obstacle->GetHeight());
//		}
//	}
//
//	if(m_directPathFound)
//	{
//		Util::DrawLine(m_pStarShip->GetTransform()->position, m_pTarget->GetTransform()->position, { 1,0,1,1 });
//	}
//	if(m_pathFound)
//	{
//		Util::DrawLine(m_pStarShip->GetTransform()->position, m_pPathNodes.front()->GetTransform()->position, { 1,0,1,1 });
//		for (int i = 1; i < m_pPathNodes.size(); ++i)
//		{
//			Util::DrawLine(m_pPathNodes[i-1]->GetTransform()->position, m_pPathNodes[i]->GetTransform()->position, {1,0,1,1});
//		}
//		Util::DrawLine(m_pPathNodes.back()->GetTransform()->position,m_pTarget->GetTransform()->position, {1,0,1,1});
//	}
//
//	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
//}
//
//void PlayScene::Update()
//{
//	UpdateDisplayList();
//	m_checkAgentLOS(m_pStarShip, m_pTarget);
//	switch(m_LOSMode)
//	{
//	case LOSMode::TARGET:
//		m_checkAllNodesWithTarget(m_pTarget);
//		break;
//	case LOSMode::SHIP:
//		m_checkAllNodesWithTarget(m_pStarShip);
//		break;
//	case LOSMode::BOTH:
//		m_checkAllNodesWithBoth();
//		break;
//	}
//	m_pStarShip->Seek();
//}
//
//void PlayScene::Clean()
//{
//	RemoveAllChildren();
//}
//
//void PlayScene::HandleEvents()
//{
//	EventManager::Instance().Update();
//
//	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
//	{
//		Game::Instance().Quit();
//	}
//
//	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
//	{
//		Game::Instance().ChangeSceneState(SceneState::START);
//	}
//
//	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
//	{
//		Game::Instance().ChangeSceneState(SceneState::END);
//	}
//}
//
//void PlayScene::Start()
//{
//	// Set GUI Title
//	m_guiTitle = "Play Scene";
//
//	// set up a few fields
//	m_LOSMode = LOSMode::TARGET;
//	m_pathNodeLOSDistance = 1000;
//	m_setPathNodeDistance(m_pathNodeLOSDistance);
//
//	// Add Game Objects
//	m_pTarget = new Target();
//	m_pTarget->GetTransform()->position = glm::vec2(550.0f, 300.0f);
//	AddChild(m_pTarget);
//
//	m_pStarShip = new StarShip();
//	m_pStarShip->GetTransform()->position = glm::vec2(150.0f, 300.0f);
//	AddChild(m_pStarShip);
//	m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
//
//	// Add Obstacles
//	BuildObstaclePool();
//
//	// Setup the Grid
//	m_isGridEnabled = false;
//	m_buildGrid();
//	m_toggleGrid(m_isGridEnabled);
//
//	// Preload Sounds
//
//	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);
//	SoundManager::Instance().Load("../Assets/Audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);
//
//	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&PlayScene::GUI_Function, this));
//}
//
//void PlayScene::GUI_Function()
//{
//	// Always open with a NewFrame
//	ImGui::NewFrame();
//
//	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
//	//ImGui::ShowDemoWindow();
//	
//	ImGui::Begin("GAME3001 - W2023 - Lab 6", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar );
//
//	ImGui::Separator();
//
//	// Debug Properties
//	if(ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
//	{
//		m_toggleGrid(m_isGridEnabled);
//	}
//
//	ImGui::Separator();
//
//	static int LOS_mode = static_cast<int>(m_LOSMode);
//	ImGui::Text("Path Node LOS");
//	ImGui::RadioButton("Target", &LOS_mode, static_cast<int>(LOSMode::TARGET)); ImGui::SameLine();
//	ImGui::RadioButton("Ship", &LOS_mode, static_cast<int>(LOSMode::SHIP)); ImGui::SameLine();
//	ImGui::RadioButton("Both", &LOS_mode, static_cast<int>(LOSMode::BOTH));
//
//	m_LOSMode = static_cast<LOSMode>(LOS_mode);
//
//	ImGui::Separator();
//
//	if(ImGui::Button("Find Path"))
//	{
//		SetLOSNodes();
//		FindPath(m_pNodesWithShipLOS, m_pNodesWithTargetLOS, m_pStarShip);
//	}
//
//	ImGui::Separator();
//
//	if(ImGui::SliderInt("Path node los distance", &m_pathNodeLOSDistance, 0, 1000))
//	{
//		m_setPathNodeDistance(m_pathNodeLOSDistance);
//	}
//
//	ImGui::Separator();
//
//	// StarShip Properties
//	static int shipPosition[] = { static_cast<int>(m_pStarShip->GetTransform()->position.x),
//		static_cast<int>(m_pStarShip->GetTransform()->position.y) };
//	if(ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
//	{
//		m_pStarShip->GetTransform()->position.x = static_cast<float>(shipPosition[0]);
//		m_pStarShip->GetTransform()->position.y = static_cast<float>(shipPosition[1]);
//		m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
//		ResetPathFinding();
//	}
//
//	// allow the ship to rotate
//	static int angle;
//	if(ImGui::SliderInt("Ship Direction", &angle, -360, 360))
//	{
//		m_pStarShip->SetCurrentHeading(static_cast<float>(angle));
//	}
//
//	ImGui::Separator();
//
//	// Target Properties
//	static int targetPosition[] = { static_cast<int>(m_pTarget->GetTransform()->position.x),
//		static_cast<int>(m_pTarget->GetTransform()->position.y) };
//	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
//	{
//		m_pTarget->GetTransform()->position.x = static_cast<float>(targetPosition[0]);
//		m_pTarget->GetTransform()->position.y = static_cast<float>(targetPosition[1]);
//		m_pStarShip->SetTargetPosition(m_pTarget->GetTransform()->position);
//		ResetPathFinding();
//	}
//
//	ImGui::Separator();
//
//	// Add Obstacle position control for each obstacle
//	for(unsigned i = 0; i < m_pObstacles.size(); ++i)
//	{
//		int obstaclePosition[] = { static_cast<int>(m_pObstacles[i]->GetTransform()->position.x),
//		static_cast<int>(m_pObstacles[i]->GetTransform()->position.y) };
//		std::string label = "Obstacle" + std::to_string(i + 1) + " Position";
//		if(ImGui::SliderInt2(label.c_str(), obstaclePosition, 0, 800))
//		{
//			m_pObstacles[i]->GetTransform()->position.x = static_cast<float>(obstaclePosition[0]);
//			m_pObstacles[i]->GetTransform()->position.y = static_cast<float>(obstaclePosition[1]);
//			m_buildGrid();
//			ResetPathFinding();
//		}
//	}
//
//
//	ImGui::End();
//}
//
//void PlayScene::BuildObstaclePool()
//{
//	std::ifstream inFile("../Assets/data/obstacles.txt");
//
//	while (!inFile.eof())
//	{
//		std::cout << "Obstacle" << std::endl;
//		auto obstacle = new Obstacle();
//		float x, y, w, h; // same way that the file is organized
//		inFile >> x >> y >> w >> h; // reading data from file, line by line
//		obstacle->GetTransform()->position = glm::vec2(x, y);
//		obstacle->SetWidth(static_cast<int>(w));
//		obstacle->SetHeight(static_cast<int>(h ));
//		AddChild(obstacle, 0);
//		m_pObstacles.push_back(obstacle);
//	}
//	inFile.close();
//}
//
//void PlayScene::m_buildGrid()
//{
//	constexpr auto tile_size = Config::TILE_SIZE;
//	constexpr auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
//
//	m_clearNodes(); // we will need to clear nodes because we will rebuild/redraw the grid if we move an obstacle
//
//	// lay out a grid of path_nodes
//	for (int row = 0; row < Config::ROW_NUM; ++row)
//	{
//		for (int col = 0; col < Config::COL_NUM; ++col)
//		{
//			PathNode* path_node = new PathNode();
//			path_node->GetTransform()->position = glm::vec2(static_cast<float>(col) * tile_size + offset.x,
//				static_cast<float>(row) * tile_size + offset.y);
//
//			// only show grid where there are no obstacles
//			bool keep_node = true;
//			for (const auto obstacle : m_pObstacles)
//			{
//				// determine which path_nodes to keep
//				if(CollisionManager::AABBRadiusCheck(path_node, obstacle, 20))
//				{
//					keep_node = false;
//				}
//			}
//			if(keep_node)
//			{
//				AddChild(path_node);
//				m_pGrid.push_back(path_node);
//			}
//			else
//			{
//				delete path_node;
//			}
//		}
//
//		// if Grid is supposed to be hidden - make it so!
//		m_toggleGrid(m_isGridEnabled);
//	}
//}
//
//void PlayScene::m_toggleGrid(const bool state) const
//{
//	for (const auto path_node : m_pGrid)
//	{
//		path_node->SetVisible(state);
//	}
//}
//
//void PlayScene::m_clearNodes()
//{
//	m_pGrid.clear();
//	for (const auto display_object : GetDisplayList())
//	{
//		if(display_object->GetType() == GameObjectType::PATH_NODE)
//		{
//			RemoveChild(display_object);
//		}
//	}
//}
//
//bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const
//{
//	bool has_LOS = false;
//	agent->SetHasLOS(has_LOS); // default - no LOS
//	const glm::vec2 agent_LOS_endPoint = agent->GetTransform()->position + agent->GetCurrentDirection() * agent->GetLOSDistance();
//
//	// if ship to target distance is less than or equal to the LOS Distance (Range)
//	const auto agent_to_range = Util::GetClosestEdge(agent->GetTransform()->position, target_object);
//	if (agent_to_range <= agent->GetLOSDistance())
//	{
//		// we are in range
//		std::vector<DisplayObject*> contact_list;
//		for (auto display_object : GetDisplayList())
//		{
//			const auto agent_to_object_distance = Util::GetClosestEdge(agent->GetTransform()->position, display_object);
//			if (agent_to_object_distance > agent_to_range) { continue; }
//			
//			if((display_object->GetType() != GameObjectType::AGENT)
//				&& (display_object->GetType() != GameObjectType::PATH_NODE)
//				&& display_object->GetType() != GameObjectType::TARGET)
//			{
//				contact_list.push_back(display_object);
//			}
//		}
//
//		has_LOS = CollisionManager::LOSCheck(agent, agent_LOS_endPoint, contact_list, target_object);
//
//		const auto LOSColour = (target_object->GetType() == GameObjectType::AGENT)
//			                      ? glm::vec4(0, 0, 1, 1)
//			                      : glm::vec4(0, 1, 0, 1);
//		agent->SetHasLOS(has_LOS, LOSColour);
//	}
//	return has_LOS;
//}
//
//bool PlayScene::m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object) const
//{
//	// check angle to target so we can still use LOS Distance for path_nodes
//	const auto target_direction = target_object->GetTransform()->position - path_node->GetTransform()->position;
//	const auto normalized_direction = Util::Normalize(target_direction); // normalizes target direction
//	path_node->SetCurrentDirection(normalized_direction);
//	return m_checkAgentLOS(path_node, target_object);
//}
//
//void PlayScene::m_checkAllNodesWithTarget(DisplayObject* target_object) const
//{
//	for (const auto path_node : m_pGrid)
//	{
//		m_checkPathNodeLOS(path_node, target_object);
//	}
//}
//
//void PlayScene::m_checkAllNodesWithBoth() const
//{
//	for (const auto path_node : m_pGrid)
//	{
//		const bool LOSWithStarship = m_checkPathNodeLOS(path_node, m_pStarShip);
//		const bool LOSWithTarget = m_checkPathNodeLOS(path_node, m_pTarget);
//		path_node->SetHasLOS(LOSWithStarship && LOSWithTarget, glm::vec4(0, 1, 1, 1));
//	}
//}
//
//void PlayScene::m_setPathNodeDistance(const int distance) const
//{
//	for (const auto path_node : m_pGrid)
//	{
//		path_node->SetLOSDistance(static_cast<float>(distance));
//	}
//}
//
//void PlayScene::ResetPathFinding()
//{
//	m_pNodesWithBothLOS.clear();
//	m_pNodesWithShipLOS.clear();
//	m_pNodesWithTargetLOS.clear();
//	m_pPathNodes.clear();
//	m_pNodesWithBothLOS.shrink_to_fit();
//	m_pNodesWithShipLOS.shrink_to_fit();
//	m_pNodesWithTargetLOS.shrink_to_fit();
//	m_pPathNodes.shrink_to_fit();
//	m_pathFound = false;
//	m_directPathFound = false;
//}
//
//void PlayScene::SetLOSNodes()
//{
//	ResetPathFinding();
//	for(const auto node : m_pGrid)
//	{
//		const bool LOSWithStarship = m_checkPathNodeLOS(node, m_pStarShip);
//		const bool LOSWithTarget = m_checkPathNodeLOS(node, m_pTarget);
//		if(LOSWithStarship && LOSWithTarget)
//		{
//			m_pNodesWithBothLOS.push_back(node);
//		}
//		else if(LOSWithTarget)
//		{
//			m_pNodesWithTargetLOS.push_back(node);
//		} else if(LOSWithStarship)
//		{
//			m_pNodesWithShipLOS.push_back(node);
//		}
//	}
//	m_pNodesWithBothLOS.shrink_to_fit();
//	m_pNodesWithShipLOS.shrink_to_fit();
//	m_pNodesWithTargetLOS.shrink_to_fit();
//}
//
//void PlayScene::FindPath(std::vector<PathNode*> startingNodes, std::vector<PathNode*> targetNodes, DisplayObject* startingPoint)
//{
//	PathNode* closestToStartLOSNode = nullptr;
//	PathNode* bestTargetNode = nullptr;
//	float smallestDistance = INT_MAX;
//	bool has_target_node_los = false;
//	if(m_checkAgentLOS(m_pStarShip,m_pTarget))
//	{
//		m_directPathFound = true;
//	}
//	else {
//		if (!m_pNodesWithBothLOS.empty())
//		{
//			for (const auto node : m_pNodesWithBothLOS) // If any nodes can see both ship and target
//			{
//				float tileToShipDistance = Util::GetClosestEdge(node->GetTransform()->position, startingPoint);
//				if (tileToShipDistance < smallestDistance)
//				{
//					smallestDistance = tileToShipDistance;
//					closestToStartLOSNode = node;
//					has_target_node_los = true;
//				}
//			}
//			m_pPathNodes.push_back(closestToStartLOSNode);
//			m_pathFound = true;
//		}
//		else
//		{
//			for (auto targetnode : targetNodes) // all nodes with target los
//			{
//				for (auto startingnode : startingNodes) // all nodes with starting position los
//				{
//					const bool node_to_node_los = m_checkPathNodeLOS(startingnode, targetnode);
//					if (node_to_node_los) // if the target los and ship los nodes can see eachother
//					{
//						float tileToShipDistance = Util::GetClosestEdge(startingnode->GetTransform()->position, startingPoint);
//						if (tileToShipDistance < smallestDistance)
//						{
//							smallestDistance = tileToShipDistance;
//							closestToStartLOSNode = startingnode;
//							bestTargetNode = targetnode;
//							has_target_node_los = true;
//						}
//					}
//				}
//			}
//			if (has_target_node_los) {
//				m_pPathNodes.push_back(closestToStartLOSNode);
//				m_pPathNodes.push_back(bestTargetNode);
//				m_pathFound = true;
//			}
//		}
//		if (!has_target_node_los)
//			// Very bad no good way to find a path if there isn't LOS for in between nodes, literally only works once and takes forever
//		{
//			int x = 0;
//			bool LOSWithTargetNode;
//			bool LOSWithStartNode;
//			PathNode* tileWithBothLOS = nullptr;
//			for (const auto node : m_pGrid)
//			{
//				for (auto targetnode : targetNodes) // all nodes with target los
//				{
//					LOSWithTargetNode = m_checkPathNodeLOS(node, targetnode);
//				}
//				for (auto startingnode : startingNodes) // all nodes with starting position los
//				{
//					LOSWithStartNode = m_checkPathNodeLOS(node, startingnode);
//				}
//				if (LOSWithStartNode && LOSWithTargetNode)
//				{
//					tileWithBothLOS = node;
//					break;
//				}
//				std::cout << ++x << std::endl;
//			}
//			if (tileWithBothLOS != nullptr) {
//				for (auto startingnode : startingNodes) // all nodes with starting position los
//				{
//					float bothToStartDistance = Util::GetClosestEdge(startingnode->GetTransform()->position, tileWithBothLOS);
//					if (bothToStartDistance < smallestDistance)
//					{
//						smallestDistance = bothToStartDistance;
//						closestToStartLOSNode = startingnode;
//					}
//				}
//
//				m_pPathNodes.push_back(closestToStartLOSNode);
//
//				std::vector<PathNode*> newStartingList;
//				for (const auto node : m_pGrid)
//				{
//					const bool hasLOS = m_checkPathNodeLOS(node, closestToStartLOSNode);
//					if (hasLOS)
//					{
//						newStartingList.push_back(node);
//					}
//				}
//
//				FindPath(newStartingList, targetNodes, closestToStartLOSNode);
//			}
//			else
//			{
//				std::cout << "Couldn't find path (no in between node found)" << std::endl;
//			}
//
//
//		}
//	}
//
//	std::cout << "Path Found" << std::endl;
//	m_pPathNodes.shrink_to_fit();
//}
//
//
///* To find a path
// *
// * put tiles into own vectors
// * for(all tiles)
// *		if(tile can see target)
// *			add to targetTileVector
// *		else if(tile can see ship)
// *			add to shipTileVector
// *	end for
// *
// * first step
// * for(all target tiles)
// *		for(all ship tiles)
// *		if(target tile has LOS with ship tile)
// *			getclosestEdge between the shiptile and targettile
// *			set to a var
// *			if(var < current smallest distance)
// *				set that shiptile as the corect tile
// *			foundTile = true
// *			break
// *		end if
// * end fors
// * if(foundTile)
// *		drawline from ship to that shiptile
// *		drawline from shiptile to target tile
// *		drawline from targetTile to target
// *
// * if(!foundTile)
// *		do this step again, but find a tile that has los with both a targettile and a shiptile
// *
// */
