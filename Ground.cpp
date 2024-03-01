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

	// 타일맵 생성 및 읽기
	auto tilemap = TMXTiledMap::create(filename);
	addChild(tilemap);

	// 바닥 레이어 가져오기
	auto backgroundLayer = tilemap->getLayer("background");

	// 바닥 레이어의 사이즈를 화면 크기로 조정
	backgroundLayer->setContentSize(Director::getInstance()->getWinSize());

	// 바닥 레이어의 위치를 조정하여 화면 아래에 위치하도록 설정
	backgroundLayer->setPosition(Vec2(0, 0));

	// meta 레이어가 존재하는지 확인
	if (backgroundLayer) {
		// 타일맵의 크기 구하기
		auto mapSize = tilemap->getMapSize();

		// 연속된 Ground 타일을 식별하여 묶기 위한 변수들 초기화
		bool isGroundTile = false;
		Size groundTileSize;
		Vec2 groundTilePosition;

		// 타일맵에서 모든 타일을 반복하여 바닥 타일을 식별하고 충돌 처리를 추가
		for (int x = 0; x < mapSize.width; ++x) {
			for (int y = 0; y < mapSize.height; ++y) {
				auto tile = backgroundLayer->getTileAt(Vec2(x, y));
				if (tile) {
					// 해당 타일의 속성 가져오기
					auto gid = backgroundLayer->getTileGIDAt(Vec2(x, y));
					auto properties = tilemap->getPropertiesForGID(gid).asValueMap();
					if (!properties.empty()) {
						auto collisionType = properties["Ground"].asString();
						if (collisionType == "Ground") {
							if (!isGroundTile) {
								// 연속된 Ground 타일의 시작점 설정
								groundTileSize = tile->getContentSize();
								groundTilePosition = tile->getPosition();
								isGroundTile = true;
							}
						}
						else {
							if (isGroundTile) {
								// 연속된 Ground 타일이 끝난 경우, 하나의 큰 몸체로 만들기
								createGroundPhysicsBody(groundTileSize, groundTilePosition);
								isGroundTile = false;
							}
						}
					}
				}
			}
			// 타일맵의 가장자리에 도달한 경우, 마지막 연속된 Ground 타일 처리
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
	// 충돌 처리 추가
	auto tileBody = PhysicsBody::createBox(tileSize, PhysicsMaterial(0.0f, 1.0f, 0.0f));
	tileBody->setDynamic(false);
	tileBody->setCollisionBitmask(GROUND_MASK);
	tileBody->setCategoryBitmask(GROUND_MASK);  // 바닥 타일의 카테고리 비트마스크 설정
	tileBody->setContactTestBitmask(true);
	tileBody->setTag(TAG_GROUND);
	tileBody->getShape(0)->setRestitution(0.0f);
	// 타일 몸체의 위치 설정
	auto bodyPosition = Vec2(position.x + tileSize.width / 2, position.y + tileSize.height / 2);
	auto tileNode = Node::create();
	tileNode->setPosition(bodyPosition);
	tileNode->setPhysicsBody(tileBody);
	addChild(tileNode);
}
