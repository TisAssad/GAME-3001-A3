#pragma once
#ifndef __TRANSFORM__
#define __TRANSFORM__
#include <glm/vec2.hpp>

struct Transform
{
	glm::vec2 position;
	glm::vec2 rotation;
	glm::vec2 scale = {1.0f, 1.0f};
};
#endif /* defined (__TRANSFORM__) */