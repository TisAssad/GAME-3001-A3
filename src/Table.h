#pragma once
#ifndef __TABLE__
#define __TABLE__
#include "DisplayObject.h"

class Table final : public DisplayObject
{
public:
	// constructors
	Table();
	
	// destructor
	~Table() override;
	
	// life cycle functions
	void Draw() override;
	void Update() override;
	void Clean() override;
private:
	
};

#endif /* defined (__TABLE__) */