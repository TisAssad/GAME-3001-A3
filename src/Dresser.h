#pragma once
#ifndef __DRESSER__
#define __DRESSER__
#include "DisplayObject.h"

class Dresser final : public DisplayObject
{
public:
	// constructors
	Dresser();
	
	// destructor
	~Dresser() override;
	
	// life cycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;
private:
	
};

#endif /* defined (__DRESSER__) */