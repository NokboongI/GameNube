#ifndef __ENVIRONMET_H__
#define __ENVIRONMET_H__

#define DEBUG_MODE true

//땅은 모든 경우 충돌이 일어나야 하므로 최대값으로 지정, tag의 경우는 0으로 지정
//1111 1111 / 1111 1111 / 1111 1111 / 1111 1111(2)
#define GROUND_MASK 0xFFFFFFFF
#define TAG_GROUND 0

#define WORLD_GRAVITY 600

//하위 부분은 플레이어 캐릭터의 기본 속도 및 마스크와 태그 값
//아직 추가될 데이터에 대한 정보가 정해진 것이 없기에 바닥과의 상호작용을 확인하기 위하여 일단 1로 지정
#define PLAYER_MOVEMENT_SPEED 200
#define PLAYER_JUMP_DURATION 1
#define PLAYER_JUMP_SPEED 200
#define PLAYER_GRAVITY 600
#define PLAYER_MASK 0x00000001
#define TAG_PLAYER 1
#define PLAYER_SIZE 75
#define PLAYER_WIDTH 30
#define DASH_SPEED 300
#define DASH_COOLTIME 1.5f
#define DEFAULT_ATTACK_SPEED 0.3f

//일반 몹 관련 정보
#define REGULAR_ENEMY_MASK 0x00000002
#define TAG_REGULAR_ENEMY 2
#define REGULAR_ENEMY_HEIGHT 80
#define REGULAR_ENEMY_WIDTH 30
#define REGULAR_ENEMY_MOVEMENT_SPEED 100



//가장 큰 틀의 장비 타입
enum class basicWeaponType {
	oneHandWeapon, twoHandWeapon, passiveItem

};


//세부적인 장비 타입
enum class detailedWeaponType {
	big_sword, sword, knife, bow, gun, wand, staff, orb, shield, gauntlet
}; 


//장비 등급
enum class itemGrade {
	Common, UnCommon, Rare, Epic, Unique, Legend, Mythic
};


enum class attackType {
	closeRangeAttack, longRangeAttack
};


#endif // !__ENVIRONMET_H__
