#ifndef __ITEM_DETAIL_H__
#define __ITEM_DETAIL_H__
#include "stdafx.h"

class Item :public Node {
private:
	
	int basicWeaponType;
	int detailedWeoponType;
	int itemGrade;
	

protected:
	Sprite* spr;

public:
	static Item* create(const Size& size);
	virtual bool init(const Size& size);

	PhysicsBody* getBody();
	PhysicsBody* body;

	

};

class ActiveItem :public Item {
private:
	float attackRange;
	float itemDamage;
	float criticalChance;
	float itemAttackSpeed;

public:
	static ActiveItem* create();
	virtual bool init();


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