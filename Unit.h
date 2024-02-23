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
	float movementSpeed; //���� �̵��ӵ�
	float max_mana;
	float curr_mana;          //���� ����
	float attackSpeed;   //���� ���� �ӵ�
	float physics_power; //���� ���ݷ�
	float magic_power;   //���� �������ݷ�
	float ciriticalChance; //���� ġȮ
	float criticalDamage; //���� ġ��


protected:
	Sprite* spr;

public:
	static Unit* create(const Size& size, int bitmask, int tag);
	//Size�� �浹 ���� ������ ��Ÿ���� ���� �ۼ�
	virtual bool init(const Size& size, int bitmask, int tag);

	PhysicsBody* getBody();
	PhysicsBody* body;

	void setHp(float value_1, float value_2); //ü�� ����
	float getCurrHp(); //���� ü�� ��ȯ
	float getMaxHp();//�ִ�ü�� ��ȯ
	void addHp(float value_1, float value_2); //������ ȹ������� ���� �ִ� ü�� ����� ���
	void setMana(float value); //�ִ� ���� ����
	void addMana(float value_1, float value_2);
	float getMana();
	void setMovementSpeed(float value);
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

public:
	static Player* create();
	virtual bool init();
	void attack();
	ActiveItem* getActiveItemInfo(int num);

	void changeWeapon();


};

#endif