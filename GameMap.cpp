#include "stdafx.h"
#include "GameMap.h"

USING_NS_CC;

bool GameMap::init() {
	if (!Node::init()) {
		return false;
	}

	return true;
}

void GameMap::createGround(float width, float height) {
	auto ground = Sprite::create(); // 바닥 이미지 경로에 맞게 수정하세요
	ground->setPosition(Vec2(width / 2, height / 2));

	auto physicsBody = PhysicsBody::createBox(ground->getContentSize());
	physicsBody->setDynamic(false); // 바닥은 움직이지 않으므로 dynamic을 false로 설정
	physicsBody->setCollisionBitmask(1); // 바닥과 충돌을 감지할 bitmask 설정
	physicsBody->setContactTestBitmask(true); // 바닥과의 충돌을 감지하기 위해 contact test bitmask 설정
	ground->addComponent(physicsBody);

	addChild(ground);
}