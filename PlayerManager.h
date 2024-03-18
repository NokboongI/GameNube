// PlayerManager.h

#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include "stdafx.h"
#include "Unit.h" // Player Ŭ������ ���� ������ �ʿ��� �� �ֽ��ϴ�.

class PlayerManager {
public:
	// �̱��� �ν��Ͻ��� ��ȯ�ϴ� �Լ�
	static PlayerManager* getInstance();

	// �÷��̾� ������ �����ϰ� ��ȯ�ϴ� �Լ�
	void setPlayer(Player* player);
	Player* getPlayer();

	void setAliveState(bool value);
	bool getAliveState();

private:
	// �̱��� �ν��Ͻ�
	static PlayerManager* instance;

	// �÷��̾� ��ü
	Player* player;

	bool isAlive;
	// �����ڿ� �Ҹ��ڸ� private�� �����Ͽ� �ܺο��� �ν��Ͻ� ������ �����ϴ�.
	PlayerManager();
	~PlayerManager();
};

#endif // __PLAYER_MANAGER_H__
