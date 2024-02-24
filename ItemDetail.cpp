#include "stdafx.h"
#include "ItemDetail.h"



Item::Item(int basicType, int detailedType, int grade)
    : basicWeaponType(basicType), detailedWeoponType(detailedType), itemGrade(grade) {}


Item* Item::create(int basicType, int detailedType, int grade) {
	return new Item(basicType, detailedType, grade);
}

ActiveItem::ActiveItem(int basicType, int detailedType, int grade, float range, float damage, float chance, float speed)
	: Item(basicType, detailedType, grade), attackRange(range), itemDamage(damage), criticalChance(chance), itemAttackSpeed(speed) {}

ActiveItem* ActiveItem::create(int basicType, int detailedType, int grade, float range, float damage, float chance, float speed) {
	return new ActiveItem(basicType, detailedType, grade, range, damage, chance, speed);
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
