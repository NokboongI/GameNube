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

float Unit::getMovementSpeed()
{
	return this->movementSpeed;
}

void Unit::setPhysicsPower(float value)
{	
	this->physics_power = value;
}

float Unit::getPhysicsPower()
{
	return this->physics_power;
}

/*void Unit::setMagicPower(float value)
{
	this->magic_power = value;
}

float Unit::getMagicPower()
{
	return this->magic_power;
}*/

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

void Player::defaultAttack(ActiveItem* currentItem) {
	// 이미 공격 중인 경우, 공격 딜레이를 기다립니다.

	if (currentItem != nullptr) {
		if (isAttacking) {
			CCLOG("Already attacking. Please wait for the attack cooldown.");
			return;
		}

		// 공격 중 상태로 변경
		isAttacking = true;

		// 타이머를 사용하여 공격 딜레이를 적용합니다.
		this->scheduleOnce([=](float dt) {
			// 적을 탐지하고 공격 범위 내에 있는지 확인
			// 이 부분은 게임에서 실제 적을 탐지하고 공격 범위 내에 있는지 확인하는 로직으로 대체되어야 합니다.

			// 공격 범위 내에 있는 적에게 공격
			// 예시로 적에게 데미지를 가하는 방식을 구현
			// 실제 게임에서는 데미지를 가하는 방식이나 적에 대한 처리 방식은 게임 로직에 따라 다를 수 있습니다.
			float damage = currentItem->getDamage();
			float criticalChance = currentItem->getCriticalChance();
			float criticalMultiplier = 1.0f; // 치명타 발생 여부에 따른 추가 데미지를 계산하기 위한 변수

											 // 치명타 확률에 따라 치명타 여부를 결정
			float random = CCRANDOM_0_1(); // 0부터 1 사이의 랜덤한 값을 생성
			if (random < criticalChance) {
				CCLOG("Critical hit!");
				criticalMultiplier = currentItem->getCriticalDamage()*0.01; // 치명타 발생 시 추가 데미지 적용
			}

			// 실제로 적에게 데미지를 입히는 부분
			// 이 부분은 게임에서 실제 적에게 데미지를 가하는 방식으로 대체되어야 합니다.
			// 여기서는 간단하게 콘솔에 데미지를 출력하는 예시를 제시합니다.
			float totalDamage = damage * (criticalMultiplier);
			CCLOG("Dealing %.2f damage to the enemy.", totalDamage);

			// 공격 종료 후 상태 변경
			isAttacking = false;
		}, currentItem->getItemAttackSpeed(), "attack_timer"); // 공격 딜레이만큼 타이머를 설정합니다.
	}
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

int Player::getCurrentUsingItem()
{
	return this->currentUsingItem;
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

void Player::acquireItem(ActiveItem * newItem)
{
	// 획득한 아이템의 타입이 oneHandWeapon인 경우
	if (newItem->getBasicType() == basicWeaponType::oneHandWeapon) {
		// 이미 두 손에 아이템이 장착되어 있는 경우, 새로운 아이템을 첫 번째 슬롯에 추가
		if (item_1 != nullptr && item_2 != nullptr) {
			CCLOG("Both hands are occupied. Adding item to the first slot.");
			item_1 = newItem;
		}
		// 하나의 손에만 아이템이 장착되어 있는 경우, 두 번째 슬롯에 추가
		else if (item_1 != nullptr) {
			CCLOG("Second hand is empty. Adding item to the second slot.");
			item_2 = newItem;
		}
		else if (item_2 != nullptr) {
			CCLOG("First hand is empty. Adding item to the second slot.");
			item_1 = newItem;
		}
		// 양손에 아이템이 장착되어 있지 않은 경우, 첫 번째 슬롯에 추가
		else {
			CCLOG("No item equipped. Adding item to the first slot.");
			item_1 = newItem;
		}
	}
	// 획득한 아이템의 타입이 twoHandWeapon인 경우
	else if (newItem->getBasicType() == basicWeaponType::twoHandWeapon) {
		// 양손에 아이템이 이미 장착되어 있는 경우, 첫 번째 슬롯에 추가
		if (item_1 != nullptr ){
			
			CCLOG("Both hands are occupied. Adding item to the first slot.");
			item_1 = newItem;
			item_2 = nullptr;
		}
		// 하나의 손에만 아이템이 장착되어 있는 경우, 첫 번째 슬롯에 추가 (기존 아이템 제거)
		else if (item_2 != nullptr) {
			CCLOG("Second hand is empty. Adding item to the first slot and removing the item in the second slot.");
			item_2 = newItem;
			item_1 = nullptr;
		}
		// 양손에 아이템이 장착되어 있지 않은 경우, 첫 번째 슬롯에 추가
		else {
			CCLOG("No item equipped. Adding item to the first slot.");
			item_1 = newItem;
		}
	}
}

RegularEnemy * RegularEnemy::create()
{
	auto ret = new RegularEnemy();
	if (ret&&ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);

	return ret;
}

bool RegularEnemy::init()
{
	if (!Unit::init(Size( REGULAR_ENEMY_WIDTH, REGULAR_ENEMY_HEIGHT), REGULAR_ENEMY_MASK, TAG_REGULAR_ENEMY)) return false;


	setHp(100, 100);

	return true;
}
