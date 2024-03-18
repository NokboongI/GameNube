// PlayerManager.cpp
#include "stdafx.h"
#include "PlayerManager.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::getInstance() {
	if (!instance) {
		instance = new PlayerManager();
	}
	return instance;
}

void PlayerManager::setPlayer(Player* player) {
	this->player = player;
}

Player* PlayerManager::getPlayer() {
	return player;
}

void PlayerManager::setAliveState(bool value)
{
	this->isAlive = value;
}

bool PlayerManager::getAliveState()
{
	return this->isAlive;
}

PlayerManager::PlayerManager() {
	// �����ڿ��� �÷��̾� ��ü �ʱ�ȭ ���� �ڵ� �ۼ�
}

PlayerManager::~PlayerManager() {
	// �Ҹ��ڿ��� �޸� ���� ���� �ڵ� �ۼ�
}
