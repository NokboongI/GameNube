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

void Unit::setHp(float value)
{
	this->hp = value;
}

float Unit::getHp()
{
	return this->hp;
}

void Unit::addHp(float value)
{
	setHp(getHp() + value);
}

bool Unit::isHpZero()
{
	return getHp() <= 0.0f;
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
	if (!Unit::init(Size(75, 75), PLAYER_MASK, TAG_PLAYER)) return false;


	setHp(100);
	return true;
}

Ground * Ground::create()
{
	auto ret = new Ground();

	if (ret&&ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);

	return ret;
}

bool Ground::init()
{

	if (!Unit::init(Size(1280, 75), GROUND_MASK, TAG_GROUND)) return false;
	body->setGravityEnable(0);
	return true;
}
