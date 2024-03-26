#include "stdafx.h"
#include "Ground.h"

USING_NS_CC;

Ground* Ground::create(const std::string& filename) {
	auto ground = new (std::nothrow) Ground();
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

	// 타일맵 생성 및 추가
	auto tilemap = TMXTiledMap::create(filename);
	addChild(tilemap);

	// 타일 레이어 가져오기
	auto backgroundLayer = tilemap->getLayer("background");

	// 타일 레이어 크기 설정
	backgroundLayer->setContentSize(Director::getInstance()->getWinSize());
	backgroundLayer->setPosition(Vec2::ZERO);

	// 타일 사이즈 및 맵 크기 가져오기
	auto tileSize = tilemap->getTileSize();
	auto mapSize = tilemap->getMapSize();

	// 모든 타일을 확인하며 묶음으로 처리
	int consecutiveGroundStartX = -1;
	int consecutiveGroundEndX = -1;
	for (int y = 0; y < mapSize.height; ++y) {
		for (int x = 0; x < mapSize.width; ++x) {
			auto tile = backgroundLayer->getTileAt(Vec2(x, y));
			if (tile) {
				auto gid = backgroundLayer->getTileGIDAt(Vec2(x, y));
				auto properties = tilemap->getPropertiesForGID(gid).asValueMap();
				if (!properties.empty() && properties.find("Ground") != properties.end() && properties["Ground"].asString() == "Ground") {
					// Ground 타일인 경우
					if (consecutiveGroundStartX == -1) {
						// 연속된 Ground 타일의 시작 위치 설정
						consecutiveGroundStartX = x;
					}
					// 연속된 Ground 타일의 종료 위치 갱신
					consecutiveGroundEndX = x;
				}
				else {
					// Ground 타일이 아닌 경우
					if (consecutiveGroundStartX != -1) {
						// 연속된 Ground 타일의 범위에 해당하는 물리 바디 생성
						int consecutiveGroundLength = consecutiveGroundEndX - consecutiveGroundStartX + 1;
						createPhysicsBodyForGround(tilemap, consecutiveGroundStartX, consecutiveGroundEndX, y, tileSize);
						// 연속된 Ground 타일의 범위 초기화
						consecutiveGroundStartX = -1;
						consecutiveGroundEndX = -1;
					}
				}
			}
		}

		// 행이 바뀔 때마다 마지막 연속된 Ground 타일의 범위에 대해 처리
		if (consecutiveGroundStartX != -1) {
			int consecutiveGroundLength = consecutiveGroundEndX - consecutiveGroundStartX + 1;
			createPhysicsBodyForGround(tilemap, consecutiveGroundStartX, consecutiveGroundEndX, y, tileSize);
			consecutiveGroundStartX = -1;
			consecutiveGroundEndX = -1;
		}
	}

	return true;
}

void Ground::createPhysicsBodyForGround(TMXTiledMap* tilemap, int startX, int endX, int y, const Size& tileSize) {
	// Ground 타일의 범위에 해당하는 물리 바디 생성
	float bodyWidth = (endX - startX + 1) * tileSize.width;
	float posX = startX * tileSize.width + bodyWidth * 0.5f; // 시작 위치에 맞춰서 계산
	float posY = (tilemap->getMapSize().height - y - 1) * tileSize.height + tileSize.height * 0.5f; // posY 뒤집기

	auto tileBody = PhysicsBody::createBox(Size(bodyWidth, tileSize.height), PhysicsMaterial(0.0f, 1.0f, 0.0f));
	tileBody->setDynamic(false);
	tileBody->setCollisionBitmask(GROUND_MASK);
	tileBody->setCategoryBitmask(GROUND_MASK);
	tileBody->setContactTestBitmask(true);
	tileBody->setTag(TAG_GROUND);
	tileBody->getShape(0)->setRestitution(0.0f);

	auto tileNode = Node::create();
	tileNode->setPosition(posX, posY);
	tileNode->setPhysicsBody(tileBody);

	addChild(tileNode);
}
