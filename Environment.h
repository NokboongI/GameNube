#ifndef __ENVIRONMET_H__
#define __ENVIRONMET_H__

#define DEBUG_MODE true

//���� ��� ��� �浹�� �Ͼ�� �ϹǷ� �ִ밪���� ����, tag�� ���� 0���� ����
//1111 1111 / 1111 1111 / 1111 1111 / 1111 1111(2)
#define GROUND_MASK 0xFFFFFFFF
#define TAG_GROUND 0

#define WORLD_GRAVITY 600

//���� �κ��� �÷��̾� ĳ������ �⺻ �ӵ� �� ����ũ�� �±� ��
//���� �߰��� �����Ϳ� ���� ������ ������ ���� ���⿡ �ٴڰ��� ��ȣ�ۿ��� Ȯ���ϱ� ���Ͽ� �ϴ� 1�� ����
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

//�Ϲ� �� ���� ����
#define REGULAR_ENEMY_MASK 0x00000002
#define TAG_REGULAR_ENEMY 2
#define REGULAR_ENEMY_HEIGHT 80
#define REGULAR_ENEMY_WIDTH 30
#define REGULAR_ENEMY_MOVEMENT_SPEED 100



//���� ū Ʋ�� ��� Ÿ��
enum class basicWeaponType {
	oneHandWeapon, twoHandWeapon, passiveItem

};


//�������� ��� Ÿ��
enum class detailedWeaponType {
	big_sword, sword, knife, bow, gun, wand, staff, orb, shield, gauntlet
}; 


//��� ���
enum class itemGrade {
	Common, UnCommon, Rare, Epic, Unique, Legend, Mythic
};


enum class attackType {
	closeRangeAttack, longRangeAttack
};


#endif // !__ENVIRONMET_H__
