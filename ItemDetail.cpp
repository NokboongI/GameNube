#include "stdafx.h"
#include "ItemDetail.h"



Item::Item(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade)
	: basicType(basicType), detailedType(detailedType), grade(grade) {}

Item* Item::create(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade) {
	return new Item(basicType, detailedType, grade);
}

basicWeaponType Item::getBasicType()
{
	return this->basicType;
}

detailedWeaponType Item::getDetailedType()
{
	return this->detailedType;
}

itemGrade Item::getItemGrade()
{
	return this->grade;
}


ActiveItem::ActiveItem(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade, float range, float damage, float chance, float speed, float criticalDamage)
	: Item(basicType, detailedType, grade), attackRange(range), itemDamage(damage), criticalChance(chance), itemAttackSpeed(speed), itemCriticalDamageBonus(criticalDamage) {}

ActiveItem* ActiveItem::create(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade, float range, float damage, float chance, float speed, float criticalDamage) {
	return new ActiveItem(basicType, detailedType, grade, range, damage, chance, speed, criticalDamage);
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

void ActiveItem::setItemCriticalDamageBonus(float value)
{
	this->itemCriticalDamageBonus = value;
}

float ActiveItem::getCriticalDamage()
{
	return this->itemCriticalDamageBonus;
}
