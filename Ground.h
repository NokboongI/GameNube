#ifndef __GROUND_H__
#define __GROUND_H__

#include "stdafx.h"


class Ground : public cocos2d::Node {
public:
	static Ground* create(const std::string& filename);

	virtual bool init(const std::string& filename);

	void createPhysicsBodyForGround(TMXTiledMap* tilemap, int startX, int endX, int y, const Size& tileSize);
};

#endif // __GROUND_H__
