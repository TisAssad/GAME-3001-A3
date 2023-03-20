#pragma once
#ifndef __TEMPLATE_SCENE__
#define __TEMPLATE_SCENE__

#include "Scene.h"
#include "Obstacle.h"
#include "Player.h"
#include "Label.h"
#include "Renderer.h"
#include "InteractionObject.h"
#include "LOSMode.h"
#include "PathNode.h"
#include "TileStatus.h"
#include "Tile.h"
#include "Heuristic.h"

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

	// Create our Division Scheme (Grid of PathNodes)
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	void m_toggleGrid(bool state) const;
	void m_clearNodes();

	// LOS functions
	bool m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const;
	bool m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object) const;
	void m_checkAllNodesWithTarget(DisplayObject* target_object) const;
	void m_checkAllNodesWithBoth() const;
	void m_setPathNodeDistance(int distance) const;

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

	// TileMap Items
	
	Tile* m_getTile(int col, int row) const;
	Tile* m_getTile(glm::vec2 grid_position) const;

	template <typename T>
	void m_addObjectToGrid(T*& object, int col, int row, TileStatus status);

	template <typename T>
	void m_moveGameObject(T*& object, int col, int row, TileStatus status);

	void m_updateTileMap(int col, int row, TileStatus status);
	void m_updateTileMap(glm::vec2 grid_position, TileStatus status);

	std::string m_tileMap;
	void m_initializeTileMap();
	void m_buildTileMap();
	bool m_followsPath = false;
	int m_moveBuffer = 0;

	// Obstacle Items
	std::vector<Obstacle*> m_pObstacles;
	void m_buildObstacles();

	std::vector<Tile*> m_pTileGrid;
	bool m_isTIleEnabled;

	// Grid Items
	void m_buildTileGrid();
	bool m_getGridEnabled() const;
	void m_setGridEnabled(bool state);
	void m_computeTileCosts();

	// heuristic
	Heuristic m_currentHeuristic;

	// Text Box vars
	bool m_textBoxOnScreen = false;
	bool m_changeState = false;
	int m_childrenWithoutTB;
	std::string m_textBoxText;

	// Los variables
	LOSMode m_LOSMode{};
	int m_pathNodeLOSDistance;
	bool m_isGridEnabled;
};

#endif /* defined (__PLAY_SCENE__) */
