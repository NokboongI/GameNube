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
	bool dashing; // 대쉬 중인지 여부를 나타내는 변수
	float dashCooldown; // 대쉬 쿨다운 시간
	float dashDuration; // 대쉬 지속 시간
	int jumpCount; // 점프 횟수 추적
	bool onGround; // 바닥에 닿았는지 여부 확인
	Vec2 lastDirection; // 플레이어의 마지막 이동 방향을 저장하는 변수
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
