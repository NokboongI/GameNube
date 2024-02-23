#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "stdafx.h"

class GameMap : public Node {
public:
	virtual bool init() override;
	CREATE_FUNC(GameMap);

	void createGround(float width, float height);
};

#endif // __GAME_MAP_H__
