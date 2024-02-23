// Ground.cpp
#include "stdafx.h"
#include "Ground.h"

USING_NS_CC;

Ground* Ground::create(float width, float height) {
	Ground* ground = new Ground();
	if (ground && ground->init(width, height)) {
		ground->autorelease();
		return ground;
	}
	CC_SAFE_DELETE(ground);
	return nullptr;
}

bool Ground::init(float width, float height) {
	if (!Node::init()) {
		return false;
	}

	auto body = PhysicsBody::createBox(Size(width, height));
	body->setDynamic(false); // 바닥은 움직이지 않습니다.
	body->setCollisionBitmask(1); // 충돌 비트마스크 설정
	body->setTag(TAG_GROUND);
	body->setContactTestBitmask(GROUND_MASK);
	auto sprite = Sprite::create(); // 빈 Sprite 객체 생성
	sprite->setPhysicsBody(body); // 빈 Sprite 객체에 물리 바디 설정

	addChild(sprite); // 현재 Node에 Sprite를 자식으로 추가합니다.

	return true;
}
