#pragma once
#ifndef __WARDROBE__
#define __WARDROBE__
#include "DisplayObject.h"

class Wardrobe final : public DisplayObject
{
public:
	// constructors
	Wardrobe();
	
	// destructor
	~Wardrobe() override;
	
	// life cycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;
private:
	
};

#endif /* defined (__WARDROBE__) */