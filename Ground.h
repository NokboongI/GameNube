#ifndef __GROUND_H__
#define __GROUND_H__

#include "stdafx.h"

class Ground : public Node {
public:
	static Ground* create(const std::string& filename);
	virtual bool init(const std::string& filename);

	void createGroundPhysicsBody(const Size& tileSize, const Vec2& position);
};

#endif // __GROUND_H__