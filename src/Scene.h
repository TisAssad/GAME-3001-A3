#pragma once
#ifndef __SCENE__
#define __SCENE__

#include <vector>
#include <optional>
#include <SDL_pixels.h>

#include "GameObject.h"


class Scene : public GameObject
{
public:
	friend class DisplayObject;
	Scene();
	virtual ~Scene();

	// Inherited via GameObject
	virtual void Draw() override = 0;
	virtual void Update() override = 0;
	virtual void Clean() override = 0;
	virtual void HandleEvents() = 0;
	virtual void Start() = 0;

	void AddChild(DisplayObject* child, uint32_t layer_index = 0, std::optional<uint32_t> order_index = std::nullopt);
	void RemoveChild(DisplayObject* child);

	void RemoveAllChildren();
	[[nodiscard]] int NumberOfChildren() const;

	void UpdateDisplayList();
	void DrawDisplayList();

	[[nodiscard]] std::vector<DisplayObject*> GetDisplayList() const;

	void TextBox(const char* text, const std::string& font_name, const int font_size,
		const SDL_Color colour, const glm::vec2 position, const int font_style, const bool is_centered, int layer_index);

	void GetTextBox(const char* info, glm::vec2 position, int layer_index = 5);
	//void GetTextBox(const char* info, glm::vec2 position, int layer_index = 5, SDL_Color colour = { 255,255,255,255 });

private:
	uint32_t m_nextLayerIndex = 0;
	std::vector<DisplayObject*> m_displayList;

	static bool SortObjects(DisplayObject* left, DisplayObject* right);
};

#endif /* defined (__SCENE__) */
