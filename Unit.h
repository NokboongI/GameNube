#ifndef __UNIT_H__
#define __UNIT_H__
#include "stdafx.h"
#include "Environment.h"
#include "ItemDetail.h"


class Unit :public Node {
private:
	DrawNode* dn;
	float max_hp = 100;
	float curr_hp = 100;
	float movementSpeed; //유닛 이동속도
	float max_mana;
	float curr_mana;          //유닛 마나
	float attackSpeed;   //유닛 공격 속도
	float physics_power; //유닛 공격력
	float magic_power;   //유닛 마법공격력
	float ciriticalChance; //유닛 치확
	float criticalDamage; //유닛 치뎀


protected:
	Sprite* spr;

public:
	static Unit* create(const Size& size, int bitmask, int tag);
	//Size는 충돌 판정 범위를 나타내기 위해 작성
	virtual bool init(const Size& size, int bitmask, int tag);

	PhysicsBody* getBody();
	PhysicsBody* body;

	void setHp(float value_1, float value_2); //체력 설정
	float getCurrHp(); //현재 체력 반환
	float getMaxHp();//최대체력 반환
	void addHp(float value_1, float value_2); //아이템 획득등으로 인한 최대 체력 변경시 사용
	void setMana(float value); //최대 마나 설정
	void addMana(float value_1, float value_2);
	float getMana();
	void setMovementSpeed(float value);
	float getMovementSpeed();
	void setPhysicsPower(float value);
	float getPhysicsPower();
	void setMagicPower(float value);
	float getMagicPower();
	void setAttackSpeed(float value);
	float getAttackSpeed();


	bool isHpZero();
};

class Player :public Unit {
private:
	ActiveItem* item_1 = nullptr;
	ActiveItem* item_2 = nullptr;
	int currentUsingItem = 1;
	ActiveItem* currentWeapon = nullptr;

	bool isAttacking = false; // 공격 중인지를 나타내는 변수


public:
	static Player* create();
	virtual bool init();
	void defaultAttack(ActiveItem* currentItem);
	ActiveItem* getActiveItemInfo(int num);
	int getCurrentUsingItem();
	void changeWeapon();
	void acquireItem(ActiveItem* newItem);

};



class RegularEnemy :public Unit {

private:
	attackType attackType;

public:
	static RegularEnemy* create();
	virtual bool init();



};
#endif
