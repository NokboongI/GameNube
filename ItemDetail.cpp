#include "stdafx.h"
#include "ItemDetail.h"

Item* Item::create(const Size& size) {
	auto ret = new Item();
	if (ret && ret->init(size))
		ret->autorelease();
	else
		CC_SAFE_DELETE(ret);
	return ret;
}

bool Item::init(const Size& size) {
	if (!Node::init())
		return false;

	// 노드 초기화 코드

	return true;
}

PhysicsBody* Item::getBody() {
	return body;
}

ActiveItem* ActiveItem::create() {
	auto ret = new ActiveItem();
	if (ret && ret->init())
		ret->autorelease();
	else
		CC_SAFE_DELETE(ret);
	return ret;
}

bool ActiveItem::init() {
	if (!Item::init(Size(ITEM_SIZE, ITEM_SIZE)))
		return false;

	// ActiveItem 초기화 코드

	return true;
}

void ActiveItem::setDamage(float value) {
	itemDamage = value;
}

float ActiveItem::getDamage() {
	return itemDamage;
}

void ActiveItem::setAttackRange(float value)
{
	this->attackRange = value;
}

float ActiveItem::getAttackRange()
{
	return this->attackRange;
}

void ActiveItem::setCriticalChance(float value)
{
	this->criticalChance = value;
}

float ActiveItem::getCriticalChance()
{
	return this->criticalChance;
}

void ActiveItem::setItemAttackSpeed(float value)
{
	this->itemAttackSpeed = value;
}

float ActiveItem::getItemAttackSpeed()
{
	return this->itemAttackSpeed;
}
