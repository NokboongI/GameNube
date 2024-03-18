// PlayerManager.h

#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "stdafx.h"
#include "Unit.h" // Player 클래스에 대한 포함이 필요할 수 있습니다.

class PlayerManager {
public:
	// 싱글톤 인스턴스를 반환하는 함수
	static PlayerManager* getInstance();

	// 플레이어 정보를 설정하고 반환하는 함수
	void setPlayer(Player* player);
	Player* getPlayer();

	void setAliveState(bool value);
	bool getAliveState();

private:
	// 싱글톤 인스턴스
	static PlayerManager* instance;

	// 플레이어 객체
	Player* player;

	bool isAlive;
	// 생성자와 소멸자를 private로 선언하여 외부에서 인스턴스 생성을 막습니다.
	PlayerManager();
	~PlayerManager();
};

#endif // __PLAYER_MANAGER_H__
