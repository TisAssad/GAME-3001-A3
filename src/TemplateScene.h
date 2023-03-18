#pragma once
#ifndef __TEMPLATE_SCENE__
#define __TEMPLATE_SCENE__

#include "Scene.h"
#include "Player.h"
#include "Label.h"
#include "Renderer.h"
#include "InteractionObject.h"
#include "LOSMode.h"
#include "PathNode.h"

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