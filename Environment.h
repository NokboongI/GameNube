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
#define DASH_SPEED 6000
#define DASH_COOLTIME 1.0f
#define DASH_DURATIN 0.01f
#define DEFAULT_ATTACK_SPEED 0.3f


#define ITEM_SIZE 10


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





#endif // !__ENVIRONMET_H__
