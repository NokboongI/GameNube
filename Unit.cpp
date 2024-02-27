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
	setHp(getMaxHp() + value_1, getCurrHp() + value_2);
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

	dashCooltime = DASH_COOLTIME;
	setHp(100, 100);
	this->schedule(CC_SCHEDULE_SELECTOR(Player::dashCool), 1.0f); // 매 초마다 대쉬 쿨다운 함수 호출

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
		if (item_1 != nullptr) {

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

/*void Player::dash() {
	// 대쉬 사용 횟수가 0보다 크면서 현재 대쉬 중이 아닌 경우에만 대쉬를 실행합니다.
	if (dashCount > 0) {
		// 마지막으로 바라보던 방향으로 대쉬를 실행합니다.
		Vec2 newPosition = this->getPosition() + lastDirection * DASH_SPEED;
		this->setPosition(newPosition);

		// 대쉬 사용 횟수를 감소시킵니다.
		setDashCount(-1);

	}
}*/
void Player::dash() {
	// 대쉬 사용 횟수가 0보다 크면서 현재 대쉬 중이 아닌 경우에만 대쉬를 실행합니다.
	if (dashCount > 0) {
		// 대쉬 중 상태로 설정합니다.
		dashing = true;

		// 대쉬 시작 위치와 목표 위치를 설정합니다.
		Vec2 startPosition = this->getPosition();
		Vec2 targetPosition = startPosition + lastDirection * DASH_SPEED;

		// 대쉬를 부드럽게 만들기 위해 일정 간격으로 플레이어의 위치를 변경합니다.
		float dashDuration = 0.1f; // 대쉬 지속 시간 (초)
		int numberOfSteps = 50; // 대쉬를 부드럽게 만들기 위한 단계 수
		float stepDuration = dashDuration / numberOfSteps; // 각 단계의 지속 시간
		float elapsedTime = 0.0f; // 경과 시간 초기화

								  // Schedule 함수를 사용하여 일정 간격으로 플레이어의 위치를 변경합니다.
		this->schedule([=](float dt) mutable {
			elapsedTime += dt; // 경과 시간 누적

							   // 각 단계마다 새로운 위치 계산
			Vec2 newPosition = startPosition + (targetPosition - startPosition) * (elapsedTime / dashDuration);
			this->setPosition(newPosition);

			// 대쉬가 끝나면 스케쥴 해제 및 대쉬 사용 횟수 감소
			if (elapsedTime >= dashDuration) {
				this->unschedule("dash_movement"); // 스케쥴 해제
				dashing = false; // 대쉬 종료
				setDashCount(-1); // 대쉬 사용 횟수 감소
			}
		}, stepDuration, "dash_movement");
	}
}


void Player::dashCool(float dt) {
	// 대쉬 사용 횟수가 이미 최대치인 경우에는 더 이상 대쉬를 적용하지 않습니다.
	if (dashCount >= 2) {
		return;
	}

	// 대쉬 쿨타임이 끝나면 대쉬 사용 횟수를 증가시킵니다.
	if (dashCooltime <= 0) {
		dashCount++;
		dashCooltime = DASH_COOLTIME;
	}
	// 대쉬 쿨타임 감소
	dashCooltime -= dt;
}




void Player::setLastDirection(Vec2 currentDirection)
{
	this->lastDirection = currentDirection;
}

void Player::setDashCount(int value) {
	dashCount += value;
	// dashCount가 음수가 되지 않도록 보정합니다.
	dashCount = std::max(dashCount, 0);
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
	if (!Unit::init(Size(REGULAR_ENEMY_WIDTH, REGULAR_ENEMY_HEIGHT), REGULAR_ENEMY_MASK, TAG_REGULAR_ENEMY)) return false;


	setHp(100, 100);

	return true;
}