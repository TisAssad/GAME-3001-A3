#pragma once
#ifndef __INTERACTION_OBJECT__
#define __INTERACTION_OBJECT__
#include "DisplayObject.h"

class InteractionObject final : public DisplayObject
{
public:
	// constructors
	InteractionObject(int width, int height);
	
	// destructor
	~InteractionObject() override;
	
	// life cycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;
private:
	
};

#endif /* defined (__OBSTACLE__) */