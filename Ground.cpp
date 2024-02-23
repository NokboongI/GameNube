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
	body->setDynamic(false); // �ٴ��� �������� �ʽ��ϴ�.
	body->setCollisionBitmask(1); // �浹 ��Ʈ����ũ ����
	body->setTag(TAG_GROUND);
	body->setContactTestBitmask(GROUND_MASK);
	auto sprite = Sprite::create(); // �� Sprite ��ü ����
	sprite->setPhysicsBody(body); // �� Sprite ��ü�� ���� �ٵ� ����

	addChild(sprite); // ���� Node�� Sprite�� �ڽ����� �߰��մϴ�.

	return true;
}
