#include "stdafx.h"
#include "SceneIngame.h"
#include "Unit.h"
#include "Ground.h"
USING_NS_CC;

SceneIngame *SceneIngame::create() {
	auto ret = new SceneIngame();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

SceneIngame::SceneIngame() : playerCamera(nullptr), minimapCam(nullptr) {
	// 카메라 생성
	playerCamera = Camera::createPerspective(90, Director::getInstance()->getWinSize().width / Director::getInstance()->getWinSize().height, 1, 1000);
	playerCamera->setPosition3D(Vec3(0, 0, 0)); // 카메라의 초기 위치 설정
	playerCamera->lookAt(Vec3(0, 0, 0)); // 카메라가 바라보는 방향 설정
	addChild(playerCamera); // 카메라를 Scene에 추가합니다.

							// 미니맵 카메라 설정
	/*minimapCam = Camera::createPerspective(120, // 시야각을 더 넓게 조정하여 미니맵으로 보기 적합하도록 함
		Director::getInstance()->getWinSize().width / Director::getInstance()->getWinSize().height,
		1, 1000);
	minimapCam->setPosition3D(Vec3(-Director::getInstance()->getWinSize().width / 2, // 왼쪽 위에 위치하도록 설정
		Director::getInstance()->getWinSize().height / 8 -1200,
		800));
	minimapCam->lookAt(Vec3(0, 0, 0)); // 카메라가 바라보는 방향 설정

									   // 미니맵 카메라의 회전 각도를 조정하여 수평으로 만듭니다.
	minimapCam->setRotation3D(Vec3(0, 0, 0));

	addChild(minimapCam); // Scene에 미니맵 카메라 추가*/
	//TODO 미니맵과 관련된 코드는 추후 추가
}



bool SceneIngame::init() {
	if (!Scene::initWithPhysics()) return false;
	jumpCount = 0;
	auto keybd = EventListenerKeyboard::create();
	keybd->onKeyPressed = std::bind(&SceneIngame::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keybd->onKeyReleased = std::bind(&SceneIngame::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keybd, this);
	schedule(CC_SCHEDULE_SELECTOR(SceneIngame::logic));


	return true;
}

void SceneIngame::onEnter() {
	Scene::onEnter();
	
	defaultCam = Camera::getDefaultCamera();
	defaultCam->setPosition3D(Vec3(0, 0, 1000)); // 카메라의 초기 위치 설정
	defaultCam->lookAt(Vec3(0, 0, 0)); // 카메라가 바라보는 방향 설정

	defaultCam->setVisible(false);

	auto world = getPhysicsWorld();
	if (DEBUG_MODE) world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	world->setGravity(Vec2(0, -WORLD_GRAVITY));

	// 플레이어 생성
	player = Player::create();
	player->setPosition(Vec2(1280 / 2, 50));
	addChild(player);

	auto ground = Ground::create(2000, 50); //바닥 생성
	ground->setPosition(Vec2(1280 / 2, 0)); // 화면 아래에 위치하도록 설정
	addChild(ground);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(SceneIngame::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}



bool SceneIngame::onContactBegin(PhysicsContact &contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();

	// 바닥과 플레이어 사이의 충돌인지 확인
	if ((bodyA->getTag() == TAG_GROUND && bodyB->getTag() == TAG_PLAYER) ||
		(bodyA->getTag() == TAG_PLAYER && bodyB->getTag() == TAG_GROUND)) {
		// 바닥과 충돌하면 점프 횟수 초기화
		jumpCount = 0;
	}
	return true;
}

Unit *SceneIngame::getPlayer() {
	return player;
}

void SceneIngame::onKeyPressed(EventKeyboard::KeyCode c, Event *e) {
	const bool value = true;
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W:
		up = value;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		down = value;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		left = value;
		lastDirection = Vec2(-1, 0); // 왼쪽으로 이동한 것으로 기억
		break;
	case EventKeyboard::KeyCode::KEY_D:
		right = value;
		lastDirection = Vec2(1, 0); // 오른쪽으로 이동한 것으로 기억
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		if (onGround) // 바닥에 닿아 있을 때만 점프 키를 누르면 점프 변수를 설정
			jump = value;
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		if (!dashing && lastDirection != Vec2::ZERO && dashCooldown <= 0.0f) { // 대쉬 중이 아니고 마지막 이동 방향이 설정되어 있고 쿨다운이 끝났을 때만 대쉬 활성화
			dashing = true;
			dashDuration = DASH_DURATIN; // 대쉬 지속 시간 설정
			dashCooldown = DASH_COOLTIME; // 대쉬 쿨다운 시간 설정
		}
		break;

	case EventKeyboard::KeyCode::KEY_J:
		attack = value;
		break;
	}
}

void SceneIngame::onKeyReleased(EventKeyboard::KeyCode c, Event *e) {
	const bool value = false;
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W:
		up = value;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		down = value;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		left = value;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		right = value;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		jump = value;
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		if (dashing) { // 대쉬 중일 때만 대쉬 상태 해제
			dashing = value;
		}
		break;
	case EventKeyboard::KeyCode::KEY_J:
		attack = value;
		break;
	}

}

void SceneIngame::logic(float dt) {
	if (!player) return;
	Vec2 pos = player->getPosition();
	auto body = player->getBody();

	// 바닥과의 충돌을 확인하여 바닥에 닿아 있는지 여부를 결정합니다.
	onGround = body->getContactTestBitmask() != 0;
	
	// 점프 동작
	if (onGround && jumpCount < 2 && jump) {
		body->setVelocity(Vec2(0, PLAYER_JUMP_SPEED));
		jumpCount++;
		jump = false; // 점프 키를 한 번 누르면 한 번만 점프하도록 설정
	}

	// 이동 동작
	float deltaX = 0.0f;
	if (left) deltaX -= PLAYER_MOVEMENT_SPEED;
	if (right) deltaX += PLAYER_MOVEMENT_SPEED;

	if (dashing) {
		// 대쉬 활성화 상태에서 대쉬 속도로 설정
		deltaX = lastDirection.x * DASH_SPEED;
		// 대쉬 지속 시간 감소
		dashDuration -= dt;
		if (dashDuration <= 0) {
			dashing = false; // 대쉬 지속 시간이 지나면 대쉬 상태를 비활성화합니다.
		}
	}

	// 대쉬 쿨다운 시간 감소
	if (dashCooldown > 0) {
		dashCooldown -= dt;
	}

	// 속도 적용
	body->setVelocity(Vec2(deltaX, body->getVelocity().y));

	// 아래로 떨어지는 동작
	if (!onGround) {
		// 바닥에 닿아 있지 않으면 중력을 적용합니다.
		body->applyForce(Vec2(0, PLAYER_GRAVITY));
	}

	// 캐릭터의 위치를 업데이트합니다.
	pos += body->getVelocity() * dt;
	player->setPosition(pos);

	if (attack) {
		player->attack();
	}
	// 카메라 위치 업데이트
	updateCameraPosition();
}

void SceneIngame::updateCameraPosition() {
	if (player) {
		Vec3 playerPos3D = Vec3(player->getPositionX(), player->getPositionY(), 0);
		playerCamera->setPosition3D(Vec3(playerPos3D.x, playerPos3D.y, 500)); // 카메라의 Z 위치를 조정하여 플레이어를 따라가게 합니다.
	}
}

