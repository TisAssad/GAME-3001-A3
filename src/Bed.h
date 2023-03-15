#pragma once
#ifndef __BED__
#define __BED__
#include "DisplayObject.h"

class Bed final : public DisplayObject
{
public:
	// constructors
	Bed();
	
	// destructor
	~Bed() override;
	
	// life cycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;
private:
	
};

#endif /* defined (__OBSTACLE__) */