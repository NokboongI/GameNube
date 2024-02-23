#ifndef __GROUND_H__
#define __GROUND_H__

#include "stdafx.h"

class Ground : public Node {
public:
	static Ground* create(float width, float height);
	virtual bool init(float width, float height);
};

#endif // __GROUND_H__