#include "stdafx.h"
#include "Unit.h"
#include "Environment.h"
#include <vector>
#include <list>

#include "PlayerManager.h"

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
	body->setCollisionBitmask(bitmask);
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

void Unit::setCriticalChance(float value)
{
	this->ciriticalChance = value;
}

float Unit::getCriticalChance()
{
	return this->ciriticalChance;
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
																  //this->getPhysicsBody()->setCategoryBitmask(REGULAR_ENEMY_MASK);
	return true;
}

void Player::defaultAttack(ActiveItem* currentItem) {
	if (currentItem != nullptr) {
		if (isAttacking) {
			CCLOG("Already attacking. Please wait for the attack cooldown.");
			return;
		}

		// 공격 중 상태로 변경
		isAttacking = true;
		vector<RegularEnemy*> tempRegularEnemyInfo = getRegularEnemyInfo(); // 임시 벡터에 복사

		float damage = currentItem->getDamage();
		float criticalChance = currentItem->getCriticalChance();
		float attackRange = currentItem->getAttackRange();

		// 타이머를 사용하여 공격 딜레이를 적용합니다.
		this->scheduleOnce([=](float dt) {
			// 살아있는 적을 담을 임시 벡터
			vector<RegularEnemy*> aliveEnemies;

			for (auto iter = tempRegularEnemyInfo.begin(); iter != tempRegularEnemyInfo.end(); ++iter) {
				auto enemy = *iter;
				// 적의 위치를 얻어옴
				Vec2 enemyPos = enemy->getPosition();
				Vec2 playerPos = this->getPosition();
				CCLOG("Player %.2f %.2f", playerPos.x, playerPos.y);
				CCLOG("Enemy %.2f %.2f", enemyPos.x, enemyPos.y);

				// 플레이어와 적의 거리를 계산
				float distance = this->getPosition().distance(enemyPos);
				Vec2 currDirection = getLastDirection();
				// 적이 공격 범위 내에 있는지 확인
				if ((distance <= attackRange) && ((currDirection.x < 0 && playerPos.x - enemyPos.x > 0) || currDirection.x > 0 && playerPos.x - enemyPos.x < 0)) {

					CCLOG(" %.2f %.2f", currDirection.x, playerPos.x - enemyPos.x);

					// 적에게 데미지를 가하는 로직 구현
					float criticalMultiplier = 1.0f;

					// 치명타 확률에 따라 치명타 여부를 결정
					float random = CCRANDOM_0_1();
					if (random < criticalChance) {
						CCLOG("Critical hit!");
						criticalMultiplier = currentItem->getCriticalDamage() * 0.01;
					}

					// 적에게 데미지를 입히는 로직
					float totalDamage = damage * criticalMultiplier;
					bool isDead = enemy->getDamaged(totalDamage);

					CCLOG("Dealing %.2f damage to the enemy.", totalDamage);

					// 살아있는 적 리스트에 추가
					if (!isDead) {
						aliveEnemies.push_back(enemy);
					}
				}
				else {
					CCLOG("Enemy is out of attack range.");
					// 적이 공격 범위를 벗어난 경우에도 살아있는 적 리스트에 추가
					aliveEnemies.push_back(enemy);
				}
			}

			// 원본 벡터를 임시 벡터로 업데이트
			regularEnemyInfo = aliveEnemies;
			CCLOG("Attacked");
			// 공격 종료 후 상태 변경
			isAttacking = false;
		}, currentItem->getItemAttackSpeed(), "attack_timer");
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
	if (currentUsingItem == 1 && getActiveItemInfo(2) != nullptr) {

		currentUsingItem = 2;
		CCLOG("Weapon in Changed");

	}
	else if (currentUsingItem == 2 && getActiveItemInfo(1) != nullptr) {
		currentUsingItem = 1;
		CCLOG("Weapon in Changed");

	}
	else {
		CCLOG("Weapon is not Changed");

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
		//TODO: 나중에 수정, 뭔가 이상함
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

bool Player::dashState()
{
	return this->dashing;
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

Vec2 Player::getLastDirection()
{
	return this->lastDirection;
}

void Player::setDashCount(int value) {
	dashCount += value;
	// dashCount가 음수가 되지 않도록 보정합니다.
	dashCount = std::max(dashCount, 0);
}

void Player::setRegularEnemyInfo(RegularEnemy* value)
{
	this->regularEnemyInfo.push_back(value);
}

vector<RegularEnemy*> Player::getRegularEnemyInfo()
{
	return this->regularEnemyInfo;
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

	//this->getPhysicsBody()->setCategoryBitmask(PLAYER_MASK);
	this->schedule(CC_SCHEDULE_SELECTOR(RegularEnemy::update), 1.0f); // 1초 뒤에 호출

	setHp(100, 100);
	setDetectRange(500.f);
	setAttackSpeed(2.5f);
	setCriticalChance(0.2);
	setPhysicsPower(20);
	setAttackDuration(1.5f);
	return true;
}

bool Unit::getDamaged(float value)
{
	float curr_hp = getCurrHp();
	if (curr_hp - value <= 0) {
		removeFromParent();
		CCLOG("Dead");
		return true;
	}
	else {
		addHp(0, -value);
		CCLOG("get damaged, current Hp is %.2f", getCurrHp());
		return false;
	}
}


void RegularEnemy::update(float dt)
{
	if (attackState) {

		return;
	}
	else if (playerDetect() && !followPlayer) {
		followPlayer = true;
		this->stopAllActions();
		// 플레이어를 따라가는 동작을 스케줄러에 추가합니다.
		schedule(CC_SCHEDULE_SELECTOR(RegularEnemy::followPlayerSchedule), 0.1f);
	}
	else if (!playerDetect() && !randomlyMoving) {
		randomlyMoving = true;
		this->stopAllActions();
		// 랜덤 이동 동작에 딜레이를 줍니다.
		this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
			moveRandomly(dt);
		}), nullptr));
	}
}

void RegularEnemy::followPlayerSchedule(float dt)
{
	if (!playerDetect()) {
		// 플레이어를 더 이상 인식하지 못하면 스케줄러를 중지합니다.
		unschedule(CC_SCHEDULE_SELECTOR(RegularEnemy::followPlayerSchedule));
		followPlayer = false;
		randomlyMoving = true; // 랜덤 이동으로 전환합니다.
		moveRandomly(dt);
		return;
	}

	moveToPlayer(dt); // 플레이어를 따라 이동합니다.
}


void RegularEnemy::moveToPlayer(float dt)
{
	CCLOG("is following player");
	Vec2 currentPos = this->getPosition();
	Vec2 playerPos = PlayerManager::getInstance()->getPlayer()->getPosition();
	Vec2 direction = (playerPos - currentPos).getNormalized();

	// 적이 플레이어에게 다가가는 동안의 이동 속도를 조절하여 부드러운 추적 구현
	float moveSpeed = REGULAR_ENEMY_MOVEMENT_SPEED * dt;

	// 플레이어를 향해 이동하는 벡터
	Vec2 moveVector = direction * moveSpeed;

	// 적이 플레이어와의 거리를 확인
	float distanceToPlayer = (playerPos - currentPos).length();


	// 플레이어를 향해 이동
	auto moveTo = MoveBy::create(dt, moveVector);
	this->runAction(moveTo);

}

void RegularEnemy::attackMotion()
{
	CCLOG("delay_2");
	//TODO: 공격 모션 코드의 작성 필요
}


void RegularEnemy::closeAttackPlayer()
{
	//TODO: 공격 모션을 위한 이미지가 생길 경우 여기에 공격 모션 애니메이션 추가


	// 공격 로직을 여기에 구현
	CCLOG("attack player");
	Vec2 playerPos = PlayerManager::getInstance()->getPlayer()->getPosition();
	Vec2 enemyPos = this->getPosition();
	int destination = getDestination().x;
	// 플레이어와 적의 거리 계산
	float distanceToPlayer = enemyPos.distance(playerPos);
	float attackRange = getAttackRange();

	// 플레이어가 공격 범위 내에 있는지 확인
	if (PlayerManager::getInstance()->getPlayer()->dashState()) {
		CCLOG("Player is Dashing, no damage");
	}
	else if (distanceToPlayer <= attackRange && ((destination < 0 && playerPos.x - enemyPos.x > 0) || destination > 0 && playerPos.x - enemyPos.x < 0)) {
		// 플레이어를 공격
		float damage = getPhysicsPower();
		float criticalChance = getCriticalChance();
		float criticalMultiplier = 1.0f;

		// 치명타 확률에 따라 치명타 여부 결정
		float random = CCRANDOM_0_1();
		if (random < criticalChance) {
			CCLOG("Critical hit!");
			criticalMultiplier = 110 * 0.01;
		}

		// 플레이어에게 데미지를 가하는 로직 구현
		float totalDamage = damage * criticalMultiplier;
		bool isDead = PlayerManager::getInstance()->getPlayer()->getDamaged(totalDamage);

		CCLOG("Dealing %.2f damage to the player.", totalDamage);

		// 플레이어에게 데미지를 입힌 후 상태 변경 등의 추가 작업이 필요하다면 여기에 구현

		if (isDead) {
			CCLOG("Player defeated!");
			PlayerManager::getInstance()->setAliveState(false);
		}
	}

	else {
		CCLOG("Player is out of attack range.");
		// 공격 범위를 벗어난 경우 추가 작업이 필요하다면 여기에 구현
	}

	// 공격 중에는 이동을 멈춥니다.
	stopAllActions();

	// 공격이 끝난 후 스케줄러를 시작하여 다시 플레이어를 따라가도록 합니다.
	schedule(CC_SCHEDULE_SELECTOR(RegularEnemy::followPlayerSchedule), 0.1f);
	attackState = false;

}



void RegularEnemy::moveRandomly(float dt) {
	CCLOG("is randomly moving");
	followPlayer = false;
	Vec2 currentPos = this->getPosition();
	float distance = RandomHelper::random_real(1.0f, 5.0f) * REGULAR_ENEMY_MOVEMENT_SPEED;
	int direction = RandomHelper::random_int(0, 1) == 0 ? -1 : 1;
	float deltaX = direction * distance;
	this->setDestination(Vec2(direction, 0));
	// 목표 위치 설정
	Vec2 targetPos = currentPos + Vec2(deltaX, 0.0f);

	// 적을 목표 위치로 이동
	float duration = distance / REGULAR_ENEMY_MOVEMENT_SPEED;
	auto moveTo = MoveTo::create(duration, targetPos);
	auto moveDone = CallFunc::create([=]() {
		// 이동이 완료되면 다시 moveRandomly 메서드를 호출하여 다음 이동을 시작합니다.
		this->moveRandomly(0.0f);
	});

	auto sequence = Sequence::create(moveTo, moveDone, nullptr);
	this->runAction(sequence);
}



void RegularEnemy::setDestination(Vec2 value)
{
	this->destination = value;
}

Vec2 RegularEnemy::getDestination()
{
	return this->destination;
}

void RegularEnemy::setDetectRange(float value)
{
	this->detectRange = value;
}

float RegularEnemy::getDetectRange()
{
	return this->detectRange;
}

void RegularEnemy::setAttackRange(float value)
{
	this->attackRange = value;
}

float RegularEnemy::getAttackRange()
{
	return this->attackRange;
}

void RegularEnemy::setAttackDuration(float value)
{
	this->attackDuration = value;
}

float RegularEnemy::getAttackDuration()
{
	return this->attackDuration;
}

bool RegularEnemy::playerDetect() {
	// 플레이어를 가져옵니다.
	Player* player = PlayerManager::getInstance()->getPlayer();
	bool isALive = PlayerManager::getInstance()->getAliveState();

	// 플레이어가 null이면 감지하지 않습니다.
	if (!isALive) {
		return false;
	}

	Vec2 currPosition = this->getPosition();
	Vec2 playerPosition = player->getPosition();
	Vec2 currDestination = this->getDestination();
	float attackRange = getAttackRange();

	float distance = (playerPosition - currPosition).length();
	// 적이 플레이어를 감지하는 로직
	float detectRangeX = getDetectRange(); // x 방향으로의 감지 범위
	float detectRangeY = 150.0f; // y 방향으로의 감지 범위 (적당한 값으로 조정해야 함)

	if (followPlayer) {
		if (fabs(playerPosition.y - currPosition.y) < detectRangeY) {
			// 적이 플레이어의 y축 위치 범위 내에 있다면
			if (distance <= detectRangeX) {
				// 적이 플레이어를 바라보는 방향과 플레이어의 x축 위치가 일정 범위 내에 있다면 감지

				if (distance < attackRange) {
					if (((currDestination.x < 0 && playerPosition.x - currPosition.x < 0) || currDestination.x > 0 && playerPosition.x - currPosition.x > 0)) {
						setDestination(Vec2(getDestination().x*-1, 0));
					}
					attackState = true;
					CCLOG("player is in attackRange %d", attackState);
					unschedule(CC_SCHEDULE_SELECTOR(RegularEnemy::followPlayerSchedule));
					//TODO: 추후 공격 모션을 위한 자료가 생겨 추가될 경우 밑의 딜레이는 지워야 함.
					auto delay = DelayTime::create(2.0f); // 2초 동안 대기
					auto callback = CallFunc::create(CC_CALLBACK_0(RegularEnemy::closeAttackPlayer, this));
					this->runAction(Sequence::create(delay, callback, nullptr));
				}

				//CCLOG("Player detected");
				return true;
			}
		}
		return false;
	}
	else {
		// 플레이어와의 y축 거리 비교
		if (fabs(playerPosition.y - currPosition.y) < detectRangeY) {
			// 적이 플레이어의 y축 위치 범위 내에 있다면
			if ((currDestination.x == -1 && (playerPosition.x - currPosition.x > -detectRangeX && playerPosition.x - currPosition.x < 0)) ||
				(currDestination.x == 1 && (playerPosition.x - currPosition.x < detectRangeX && playerPosition.x - currPosition.x > 0))) {
				// 적이 플레이어를 바라보는 방향과 플레이어의 x축 위치가 일정 범위 내에 있다면 감지
				//CCLOG("Player detected");
				return true;
			}
		}

		// 플레이어를 감지하지 못한 경우
		//CCLOG("Player not detected");
		return false;
	}
}
