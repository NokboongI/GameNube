#include "stdafx.h"
#include "Unit.h"
#include "Environment.h"

Unit * Unit::create(const Size & size, int bitmask, int tag)
{
	auto ret = new Unit();
	if (ret && ret->init(size, bitmask, tag)) ret->autorelease();
	else CC_SAFE_DELETE(ret);

	return ret;
}

bool Unit::init(const Size & size, int bitmask, int tag)
{
	if (!Node::init()) return false;
	body = PhysicsBody::createBox(size);
	body->setTag(tag);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(bitmask);
	addComponent(body);
	body->setRotationEnable(false);


	return true;
}

PhysicsBody * Unit::getBody()
{
	return body;
}

void Unit::setHp(float value_1, float value_2)
{
	this->max_hp = value_1;
	this->curr_hp = value_2;
}

float Unit::getCurrHp()
{
	return this->curr_hp;
}

float Unit::getMaxHp()
{
	return this->max_hp;
}

void Unit::addHp(float value_1, float value_2)
{
	setHp(getMaxHp()+value_1, getCurrHp() + value_2);
}

void Unit::setMana(float value)
{
	this->max_mana = value;
	this->curr_mana = value;
}

void Unit::addMana(float value_1, float value_2)
{
	this->max_mana += value_1;
	this->curr_mana += value_2;
}

float Unit::getMana()
{
	return this->curr_mana;
}

void Unit::setMovementSpeed(float value)
{
	this->movementSpeed = value;
}

void Unit::setPhysicsPower(float value)
{	
	this->physics_power = value;
}

float Unit::getPhysicsPower()
{
	return this->physics_power;
}

void Unit::setMagicPower(float value)
{
	this->magic_power = value;
}

float Unit::getMagicPower()
{
	return this->magic_power;
}

void Unit::setAttackSpeed(float value)
{
	this->attackSpeed = value;
}

float Unit::getAttackSpeed()
{
	return this->attackSpeed;
}

bool Unit::isHpZero()
{
	return getCurrHp() <= 0.0f;
}

Player * Player::create()
{
	auto ret = new Player();
	if (ret&&ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);

	return ret;
}

bool Player::init()
{
	if (!Unit::init(Size(PLAYER_WIDTH, PLAYER_SIZE), PLAYER_MASK, TAG_PLAYER)) return false;


	setHp(100, 100);

	return true;
}

void Player::attack()
{
	float playerDefaultAttackSpeed = getAttackSpeed();
	if (currentUsingItem == 1 && item_1 != nullptr) {
		currentWeapon = getActiveItemInfo(1);
	}
	else if (currentUsingItem == 2 && item_2 != nullptr) {
		currentWeapon = getActiveItemInfo(2);
	}
	float damage = currentWeapon->getDamage();
	float attackRange = currentWeapon->getAttackRange();
	float attackSpeed = currentWeapon->getItemAttackSpeed();
	//TODO 공격 코드 작성
}

ActiveItem* Player::getActiveItemInfo(int num)
{
	if (num == 1) {
		return this->item_1;
	}
	else {
		return this->item_2;
	}
}

void Player::changeWeapon()
{
	if (currentUsingItem == 1) {
		currentUsingItem = 2;
	}
	else {
		currentUsingItem = 1;
	}
}

