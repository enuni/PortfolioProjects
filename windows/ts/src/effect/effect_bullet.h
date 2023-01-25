// EFFECT_BULLET ��u��
// bullet ��u��
// Bullet ��u��
// ts ��u��
// effect ��u��

#ifndef __EFFECT_BULLET_H__
#define __EFFECT_BULLET_H__

#include "task_save.h"
#include "game_char.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace effect_bullet {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	STATE_FIRE,
	
	STATE_MAX
};

enum TYPE {
	TYPE_STRAIGHT,
	TYPE_HOMING,
	
	TYPE_MAX
};

} // namespece effect_bullet
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {

typedef struct _bullet_param {
	int x;
	int y;
	int angle;
	nn::common::CGameChar * target;
	double speed;
	double angleSpeed;
	int type;
	int attribute;
	double speedAdd;
	double angleSpeedAdd;

	void (* moveCBFunc)( void *, void *, double, double, double, double);
	void * moveCBuncArg;
	void (* hitCBFunc)(void *, void *);
	void * hitCBFuncArg;
	void (* endCBFunc)(void *);
	void * endCBFuncArg;

	nn::common::def::graphics2d::RECT liveRect;
} BULLET_PARAM;

typedef struct _bullet_game_membar {
	nn::ts::effect::BULLET_PARAM param;
	double x;
	double y;
	double angle;
	double speed;
	double angleSpeed;
} BULLET_GAME_MEMBER;

typedef struct _bullet_disp_membar {
	int objIdHoming[5];
	int objIdNormal[5];
} BULLET_DISP_MEMBER;
	
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


	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::effect::BULLET_GAME_MEMBER gm;
	static nn::ts::effect::BULLET_DISP_MEMBER dm;
	
}; // CBullet
	 
} // namespece effect

} // namespace ts
} // namespace nn
	

#endif // __EFFECT_BULLET_H__
