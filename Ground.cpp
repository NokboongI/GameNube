// Ground.cpp
#include "stdafx.h"
#include "Ground.h"

USING_NS_CC;

Ground* Ground::create(const std::string& filename) {
	Ground* ground = new Ground();
	if (ground && ground->init(filename)) {
		ground->autorelease();
		return ground;
	}
	CC_SAFE_DELETE(ground);
	return nullptr;
}

bool Ground::init(const std::string& filename) {
	if (!Node::init()) {
		return false;
	}

	// Ÿ�ϸ� ���� �� �б�
	auto tilemap = TMXTiledMap::create(filename);
	addChild(tilemap);

	// �ٴ� ���̾� ��������
	auto backgroundLayer = tilemap->getLayer("background");

	// �ٴ� ���̾��� ����� ȭ�� ũ��� ����
	backgroundLayer->setContentSize(Director::getInstance()->getWinSize());

	// �ٴ� ���̾��� ��ġ�� �����Ͽ� ȭ�� �Ʒ��� ��ġ�ϵ��� ����
	backgroundLayer->setPosition(Vec2(0, 0));

	// meta ���̾ �����ϴ��� Ȯ��
	if (backgroundLayer) {
		// Ÿ�ϸ��� ũ�� ���ϱ�
		auto mapSize = tilemap->getMapSize();

		// ���ӵ� Ground Ÿ���� �ĺ��Ͽ� ���� ���� ������ �ʱ�ȭ
		bool isGroundTile = false;
		Size groundTileSize;
		Vec2 groundTilePosition;

		// Ÿ�ϸʿ��� ��� Ÿ���� �ݺ��Ͽ� �ٴ� Ÿ���� �ĺ��ϰ� �浹 ó���� �߰�
		for (int x = 0; x < mapSize.width; ++x) {
			for (int y = 0; y < mapSize.height; ++y) {
				auto tile = backgroundLayer->getTileAt(Vec2(x, y));
				if (tile) {
					// �ش� Ÿ���� �Ӽ� ��������
					auto gid = backgroundLayer->getTileGIDAt(Vec2(x, y));
					auto properties = tilemap->getPropertiesForGID(gid).asValueMap();
					if (!properties.empty()) {
						auto collisionType = properties["Ground"].asString();
						if (collisionType == "Ground") {
							if (!isGroundTile) {
								// ���ӵ� Ground Ÿ���� ������ ����
								groundTileSize = tile->getContentSize();
								groundTilePosition = tile->getPosition();
								isGroundTile = true;
							}
						}
						else {
							if (isGroundTile) {
								// ���ӵ� Ground Ÿ���� ���� ���, �ϳ��� ū ��ü�� �����
								createGroundPhysicsBody(groundTileSize, groundTilePosition);
								isGroundTile = false;
							}
						}
					}
				}
			}
			// Ÿ�ϸ��� �����ڸ��� ������ ���, ������ ���ӵ� Ground Ÿ�� ó��
			if (isGroundTile) {
				createGroundPhysicsBody(groundTileSize, groundTilePosition);
				isGroundTile = false;
			}
		}
	}

	return true;
}

void Ground::createGroundPhysicsBody(const Size & tileSize, const Vec2 & position)
{
	// �浹 ó�� �߰�
	auto tileBody = PhysicsBody::createBox(tileSize, PhysicsMaterial(0.0f, 1.0f, 0.0f));
	tileBody->setDynamic(false);
	tileBody->setCollisionBitmask(GROUND_MASK);
	tileBody->setCategoryBitmask(GROUND_MASK);  // �ٴ� Ÿ���� ī�װ� ��Ʈ����ũ ����
	tileBody->setContactTestBitmask(true);
	tileBody->setTag(TAG_GROUND);
	tileBody->getShape(0)->setRestitution(0.0f);
	// Ÿ�� ��ü�� ��ġ ����
	auto bodyPosition = Vec2(position.x + tileSize.width / 2, position.y + tileSize.height / 2);
	auto tileNode = Node::create();
	tileNode->setPosition(bodyPosition);
	tileNode->setPhysicsBody(tileBody);
	addChild(tileNode);
}
