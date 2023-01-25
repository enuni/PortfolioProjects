// ATTACK_BULLET ��u��
// attack_bullet ��u��
// Bullet ��u��
// ts ��u��
// attack ��u��

#ifndef __ATTACK_BULLET_H__
#define __ATTACK_BULLET_H__

#include "task_save.h"
#include "game_char.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace attack_bullet {

enum STATE {
	STATE_NONE,
	STATE_FIRE,
	
	STATE_MAX
};

enum TYPE {
	TYPE_HOMING,
	TYPE_MACHINEGUN,

	TYPE_MAX
};

} // namespece attack_bullet
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace attack {

typedef struct _bullet_param {
	int fireN;
	int fireBulletN;
	int fireIntervalFrameN;
	double angle[8];
	double addAngle[8];
	int damage;
	nn::ts::quest::CQuest * quest;
	int x;
	int y;
	nn::common::CGameChar * targets[16];
	int targetN;
	int type;
	unsigned int hitTargetBit;
	int attribute;

	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} BULLET_PARAM;

typedef struct _bullet_game_membar {
	nn::ts::attack::BULLET_PARAM param;
	int fireCnt;
	int fireBulletCnt;
	double angle;
	int bulletCnt;
} BULLET_GAME_MEMBER;
	
class CBullet : public TASK_SAVE {
	
public:
	CBullet();
	CBullet( TASK_SAVE * oya);
	CBullet( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CBullet();
	void constructCommon();					// �����o�[0�N���A�Ȃ�new������constructor�ŕK����鎖
	void task();
	void disp();
	
	void setMember( void * owner);			// �����������TASK����K�v�ȃp�����[�^�����炤 void *�^�͐錾����ς��Ă��ǂ����A�֐����Ō^�ϊ����Ă��ǂ�
	void setMemberLoad( void * owner);		// setMember()�̃��[�h���ŁA���[�h���ĕs�v�Ȃ��̂̓Z�b�g���Ȃ�
	void setMemberCommon( void * owner);	// setMember()��setMemberLoad()�̋��ʏ���
	void start();							// setMember()��ɂ��Task�����グ����
	void startLoad();						// start()�̃��[�h����
	void startCommon();						// start()��startLoad()�̋��ʏ���
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();

	static void moveBulletCB( void * argAttackBullet, void * argEffectBullet,
		double x1, double y1, double x2, double y2);
	void moveBullet( void * argEffectBullet,
		double x1, double y1, double x2, double y2);
	static void fireIntervalCB( void * argAttackBullet);
	void fireInterval();
	void fire();
	static void endEffectBulletCB( void * argAttackBullet);
	void endEffectBullet();
	void updateTarget( nn::ts::effect::CBullet * effectBullet);

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::attack::BULLET_GAME_MEMBER gm;
	
}; // CBullet
	 
} // namespece attack

} // namespace ts
} // namespace nn
	

#endif // __ATTACK_BULLET_H__
