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
	auto ground = Sprite::create(); // �ٴ� �̹��� ��ο� �°� �����ϼ���
	ground->setPosition(Vec2(width / 2, height / 2));

	auto physicsBody = PhysicsBody::createBox(ground->getContentSize());
	physicsBody->setDynamic(false); // �ٴ��� �������� �����Ƿ� dynamic�� false�� ����
	physicsBody->setCollisionBitmask(1); // �ٴڰ� �浹�� ������ bitmask ����
	physicsBody->setContactTestBitmask(true); // �ٴڰ��� �浹�� �����ϱ� ���� contact test bitmask ����
	ground->addComponent(physicsBody);

	addChild(ground);
}