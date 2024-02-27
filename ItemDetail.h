#ifndef __ITEM_DETAIL_H__
#define __ITEM_DETAIL_H__
#include "stdafx.h"

class Item {
private:
	
	basicWeaponType basicType;
	detailedWeaponType detailedType;
	itemGrade grade;
	


public:

	Item(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade);
	static Item* create(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade);

	basicWeaponType getBasicType();
	detailedWeaponType getDetailedType();
	itemGrade getItemGrade();

};

class ActiveItem :public Item {
private:
	float attackRange;
	float itemDamage;
	float criticalChance;
	float itemAttackSpeed;
	float itemCriticalDamageBonus;

public:

	ActiveItem(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade, float range, float damage, float chance, float speed, float criticalDamage);
	static ActiveItem* create(basicWeaponType basicType, detailedWeaponType detailedType, itemGrade grade, float range, float damage, float chance, float speed, float criticalDamage);


	void setDamage(float value);
	float getDamage();
	
	void setAttackRange(float value);
	float getAttackRange();

	void setCriticalChance(float value);
	float getCriticalChance();

	void setItemAttackSpeed(float value);
	float getItemAttackSpeed();

	void setItemCriticalDamageBonus(float value);
	float getCriticalDamage();

};





#endif
