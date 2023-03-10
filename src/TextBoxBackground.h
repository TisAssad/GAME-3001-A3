#pragma once
#ifndef __TEXT_BOX_BACKGROUND__
#define __TEXT_BOX_BACKGROUND__
#include "DisplayObject.h"

class TextBoxBackground final : public DisplayObject
{
public:
	// constructors
	TextBoxBackground();
	
	// destructor
	~TextBoxBackground() override;
	
	// life cycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;
private:
	
};

#endif 