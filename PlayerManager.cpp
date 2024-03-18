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
	// 생성자에서 플레이어 객체 초기화 등의 코드 작성
}

PlayerManager::~PlayerManager() {
	// 소멸자에서 메모리 해제 등의 코드 작성
}
