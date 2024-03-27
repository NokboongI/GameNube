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
	backgroundLayer->setPosition(Vec2::ZERO);

	// 타일 사이즈 및 맵 크기 가져오기
	auto tileSize = tilemap->getTileSize();
	auto mapSize = tilemap->getMapSize();

	// 모든 타일을 확인하며 묶음으로 처리
	int consecutiveGroundStartX = -1;
	int consecutiveGroundEndX = -1;

	int consecutiveGroundStartX_notGround = -1;
	int consecutiveGroundEndX_notGround = -1;

	bool hasGround = false;
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
					hasGround = true;
					CCLOG("Tile at (%d, %d) is Ground", x, y); // 추가된 로그
				}
				else if(!properties.empty() && properties.find("Ground") != properties.end() && properties["Ground"].asString() == "notGround"){
					// Ground 타일이 아닌 경우
					// Ground 타일인 경우
					if (consecutiveGroundStartX_notGround == -1) {
						// 연속된 Ground 타일의 시작 위치 설정
						consecutiveGroundStartX_notGround = x;
					}
					// 연속된 Ground 타일의 종료 위치 갱신
					consecutiveGroundEndX_notGround = x;
					hasGround = true;
					CCLOG("Tile at (%d, %d) is not Ground", x, y); // 추가된 로그
				}
			}
			else {
				// 타일이 없는 경우
				if ((consecutiveGroundStartX != -1 && hasGround)) {
					// 이전 타일이 Ground이고 현재 타일이 없는 경우에만 처리
					int consecutiveGroundLength = consecutiveGroundEndX - consecutiveGroundStartX + 1;
					CCLOG("%d is length", consecutiveGroundLength);
					if (consecutiveGroundLength > 1) {
						// 연속된 Ground 타일 사이에 공백이 있는 경우
						createPhysicsBodyForGround(tilemap, consecutiveGroundStartX, consecutiveGroundEndX, y, tileSize, true);
					}
					consecutiveGroundStartX = -1;
					consecutiveGroundEndX = -1;
					hasGround = false;
				}
				else if ((consecutiveGroundStartX_notGround != -1 && hasGround)) {
					int consecutiveGroundLength = consecutiveGroundEndX_notGround - consecutiveGroundStartX_notGround + 1;
					CCLOG("%d is length", consecutiveGroundLength);

					if (consecutiveGroundLength > 1) {
						// 연속된 Ground 타일 사이에 공백이 있는 경우
						createPhysicsBodyForGround(tilemap, consecutiveGroundStartX_notGround, consecutiveGroundEndX_notGround, y, tileSize, false);
					}
					consecutiveGroundStartX_notGround = -1;
					consecutiveGroundEndX_notGround = -1;
					hasGround = false;
				}
				//CCLOG("There is no tile at (%d, %d)", x, y); // 추가된 로그
			}
		}
		// 행이 바뀔 때마다 마지막 연속된 Ground 타일의 범위에 대해 처리
		if (consecutiveGroundStartX != -1 && hasGround) {
			int consecutiveGroundLength = consecutiveGroundEndX - consecutiveGroundStartX + 1;
			if (consecutiveGroundLength > 1) {
				// 연속된 Ground 타일 사이에 공백이 있는 경우
				createPhysicsBodyForGround(tilemap, consecutiveGroundStartX, consecutiveGroundEndX, y, tileSize, true);
			}
			consecutiveGroundStartX = -1;
			consecutiveGroundEndX = -1;
			hasGround = false;
		}
		else if ((consecutiveGroundStartX_notGround != -1 && hasGround)) {
			int consecutiveGroundLength = consecutiveGroundEndX_notGround - consecutiveGroundStartX_notGround + 1;
			if (consecutiveGroundLength > 1) {
				// 연속된 Ground 타일 사이에 공백이 있는 경우
				createPhysicsBodyForGround(tilemap, consecutiveGroundStartX_notGround, consecutiveGroundEndX_notGround, y, tileSize, false);
			}
			consecutiveGroundStartX_notGround = -1;
			consecutiveGroundEndX_notGround = -1;
			hasGround = false;
		}
	}
	return true;
}


void Ground::createPhysicsBodyForGround(TMXTiledMap* tilemap, int startX, int endX, int y, const Size& tileSize, bool type) {
	// Ground 타일의 범위에 해당하는 물리 바디 생성
	float bodyWidth = (endX - startX + 1) * tileSize.width;
	float posX = startX * tileSize.width + bodyWidth * 0.5f; // 시작 위치에 맞춰서 계산
	float posY = (tilemap->getMapSize().height - y - 1) * tileSize.height + tileSize.height * 0.5f; // posY 뒤집기

																									// 더 단단한 PhysicsBody를 만들기 위해 밀도, 마찰력, 탄성 수정
	auto tileBody = PhysicsBody::createBox(Size(bodyWidth, tileSize.height), PhysicsMaterial(1.0f, 1.0f, 0.0f));
	tileBody->setDynamic(false);
	if (type) {
	tileBody->setCollisionBitmask(GROUND_MASK);
	tileBody->setCategoryBitmask(GROUND_MASK);
	tileBody->setContactTestBitmask(true);

	
		tileBody->setTag(TAG_GROUND);
	}

	tileBody->getShape(0)->setRestitution(0.0f);

	auto tileNode = Node::create();
	tileNode->setPosition(posX, posY);
	tileNode->setPhysicsBody(tileBody);

	addChild(tileNode);
}

