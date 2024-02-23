#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__
#include "stdafx.h"
#include "Unit.h"

class SceneIngame : public Scene {
private:
	Player* player = nullptr;
	Unit* ground = nullptr;
	bool up = false, down = false, left = false, right = false;
	bool jump = false;
	bool attack = false;
	bool dashing; // �뽬 ������ ���θ� ��Ÿ���� ����
	float dashCooldown; // �뽬 ��ٿ� �ð�
	float dashDuration; // �뽬 ���� �ð�
	int jumpCount; // ���� Ƚ�� ����
	bool onGround; // �ٴڿ� ��Ҵ��� ���� Ȯ��
	Vec2 lastDirection; // �÷��̾��� ������ �̵� ������ �����ϴ� ����
	Camera* playerCamera;
	Camera* minimapCam;
	Camera* defaultCam;

public:
	SceneIngame();
	static SceneIngame* create();

	virtual bool init() override;
	virtual void onEnter() override;

	bool onContactBegin(PhysicsContact& contact);
	Unit* getPlayer();

	void onKeyPressed(EventKeyboard::KeyCode c, Event* e);
	void onKeyReleased(EventKeyboard::KeyCode c, Event* e);

	void logic(float dt);

	void updateCameraPosition();

};


#endif
