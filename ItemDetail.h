#ifndef __ITEM_DETAIL_H__
#define __ITEM_DETAIL_H__
#include "stdafx.h"

class Item {
private:
	
	int basicWeaponType;
	int detailedWeoponType;
	int itemGrade;
	


public:

	Item(int basicType, int detailedType, int grade);

	static Item* create(int basicType, int detailedType, int grade);


};

class ActiveItem :public Item {
private:
	float attackRange;
	float itemDamage;
	float criticalChance;
	float itemAttackSpeed;

public:

	ActiveItem(int basicType, int detailedType, int grade, float range, float damage, float chance, float speed);

	static ActiveItem* create(int basicType, int detailedType, int grade, float range, float damage, float chance, float speed);


	void setDamage(float value);
	float getDamage();
	
	void setAttackRange(float value);
	float getAttackRange();

	void setCriticalChance(float value);
	float getCriticalChance();

	void setItemAttackSpeed(float value);
	float getItemAttackSpeed();


};





#endif
