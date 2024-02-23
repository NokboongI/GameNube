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
	// ī�޶� ����
	playerCamera = Camera::createPerspective(90, Director::getInstance()->getWinSize().width / Director::getInstance()->getWinSize().height, 1, 1000);
	playerCamera->setPosition3D(Vec3(0, 0, 0)); // ī�޶��� �ʱ� ��ġ ����
	playerCamera->lookAt(Vec3(0, 0, 0)); // ī�޶� �ٶ󺸴� ���� ����
	addChild(playerCamera); // ī�޶� Scene�� �߰��մϴ�.

							// �̴ϸ� ī�޶� ����
	/*minimapCam = Camera::createPerspective(120, // �þ߰��� �� �а� �����Ͽ� �̴ϸ����� ���� �����ϵ��� ��
		Director::getInstance()->getWinSize().width / Director::getInstance()->getWinSize().height,
		1, 1000);
	minimapCam->setPosition3D(Vec3(-Director::getInstance()->getWinSize().width / 2, // ���� ���� ��ġ�ϵ��� ����
		Director::getInstance()->getWinSize().height / 8 -1200,
		800));
	minimapCam->lookAt(Vec3(0, 0, 0)); // ī�޶� �ٶ󺸴� ���� ����

									   // �̴ϸ� ī�޶��� ȸ�� ������ �����Ͽ� �������� ����ϴ�.
	minimapCam->setRotation3D(Vec3(0, 0, 0));

	addChild(minimapCam); // Scene�� �̴ϸ� ī�޶� �߰�*/
	//TODO �̴ϸʰ� ���õ� �ڵ�� ���� �߰�
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
	defaultCam->setPosition3D(Vec3(0, 0, 1000)); // ī�޶��� �ʱ� ��ġ ����
	defaultCam->lookAt(Vec3(0, 0, 0)); // ī�޶� �ٶ󺸴� ���� ����

	defaultCam->setVisible(false);

	auto world = getPhysicsWorld();
	if (DEBUG_MODE) world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	world->setGravity(Vec2(0, -WORLD_GRAVITY));

	// �÷��̾� ����
	player = Player::create();
	player->setPosition(Vec2(1280 / 2, 50));
	addChild(player);

	auto ground = Ground::create(2000, 50); //�ٴ� ����
	ground->setPosition(Vec2(1280 / 2, 0)); // ȭ�� �Ʒ��� ��ġ�ϵ��� ����
	addChild(ground);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(SceneIngame::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}



bool SceneIngame::onContactBegin(PhysicsContact &contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();

	// �ٴڰ� �÷��̾� ������ �浹���� Ȯ��
	if ((bodyA->getTag() == TAG_GROUND && bodyB->getTag() == TAG_PLAYER) ||
		(bodyA->getTag() == TAG_PLAYER && bodyB->getTag() == TAG_GROUND)) {
		// �ٴڰ� �浹�ϸ� ���� Ƚ�� �ʱ�ȭ
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
		lastDirection = Vec2(-1, 0); // �������� �̵��� ������ ���
		break;
	case EventKeyboard::KeyCode::KEY_D:
		right = value;
		lastDirection = Vec2(1, 0); // ���������� �̵��� ������ ���
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		if (onGround) // �ٴڿ� ��� ���� ���� ���� Ű�� ������ ���� ������ ����
			jump = value;
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		if (!dashing && lastDirection != Vec2::ZERO && dashCooldown <= 0.0f) { // �뽬 ���� �ƴϰ� ������ �̵� ������ �����Ǿ� �ְ� ��ٿ��� ������ ���� �뽬 Ȱ��ȭ
			dashing = true;
			dashDuration = DASH_DURATIN; // �뽬 ���� �ð� ����
			dashCooldown = DASH_COOLTIME; // �뽬 ��ٿ� �ð� ����
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
		if (dashing) { // �뽬 ���� ���� �뽬 ���� ����
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

	// �ٴڰ��� �浹�� Ȯ���Ͽ� �ٴڿ� ��� �ִ��� ���θ� �����մϴ�.
	onGround = body->getContactTestBitmask() != 0;
	
	// ���� ����
	if (onGround && jumpCount < 2 && jump) {
		body->setVelocity(Vec2(0, PLAYER_JUMP_SPEED));
		jumpCount++;
		jump = false; // ���� Ű�� �� �� ������ �� ���� �����ϵ��� ����
	}

	// �̵� ����
	float deltaX = 0.0f;
	if (left) deltaX -= PLAYER_MOVEMENT_SPEED;
	if (right) deltaX += PLAYER_MOVEMENT_SPEED;

	if (dashing) {
		// �뽬 Ȱ��ȭ ���¿��� �뽬 �ӵ��� ����
		deltaX = lastDirection.x * DASH_SPEED;
		// �뽬 ���� �ð� ����
		dashDuration -= dt;
		if (dashDuration <= 0) {
			dashing = false; // �뽬 ���� �ð��� ������ �뽬 ���¸� ��Ȱ��ȭ�մϴ�.
		}
	}

	// �뽬 ��ٿ� �ð� ����
	if (dashCooldown > 0) {
		dashCooldown -= dt;
	}

	// �ӵ� ����
	body->setVelocity(Vec2(deltaX, body->getVelocity().y));

	// �Ʒ��� �������� ����
	if (!onGround) {
		// �ٴڿ� ��� ���� ������ �߷��� �����մϴ�.
		body->applyForce(Vec2(0, PLAYER_GRAVITY));
	}

	// ĳ������ ��ġ�� ������Ʈ�մϴ�.
	pos += body->getVelocity() * dt;
	player->setPosition(pos);

	if (attack) {
		player->attack();
	}
	// ī�޶� ��ġ ������Ʈ
	updateCameraPosition();
}

void SceneIngame::updateCameraPosition() {
	if (player) {
		Vec3 playerPos3D = Vec3(player->getPositionX(), player->getPositionY(), 0);
		playerCamera->setPosition3D(Vec3(playerPos3D.x, playerPos3D.y, 500)); // ī�޶��� Z ��ġ�� �����Ͽ� �÷��̾ ���󰡰� �մϴ�.
	}
}

