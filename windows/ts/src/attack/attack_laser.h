// LASER ��u��
// laser ��u��
// Laser ��u��
// ts ��u��
// attack ��u��

#ifndef __ATTACK_LASER_H__
#define __ATTACK_LASER_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace attack_laser {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	
	STATE_MAX
};

enum LASER_TYPE {
	LASER_TYPE_MASTER_SPARK,
	LASER_TYPE_REFLECTION,

	LASER_TYPE_MAX
};

} // namespece attack_laser
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace attack {

typedef struct _laser_param {
	int x;
	int y;
	int angle;
	int width;
	int damage;
	nn::ts::quest::CQuest * quest;
	int type;
	int reflectionLaserN;				// �ݒ肵���{���̃��[�U�[���o�锽�ˉ񐔂͐ݒ�l -1
	int attribute;
	unsigned int hitTargetBit;

	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} LASER_PARAM;

typedef struct _laser_game_membar {
	LASER_PARAM param;
	int reflectionCnt;
	int reflectionX;
	int reflectionY;
	int reflectionAngle;
	
	int x;
	int y;
	int angle;
	int len;
	int laserCnt;
} LASER_GAME_MEMBER;
	
class CLaser : public TASK_SAVE {
	
public:
	CLaser();
	CLaser( TASK_SAVE * oya);
	CLaser( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CLaser();
	void constructCommon();					// �����o�[0�N���A�Ȃ�new������constructor�ŕK����鎖
	void task();
	void disp();
	
	void setMember( void * owner);			// �����������TASK����K�v�ȃp�����[�^�����炤 void *�^�͐錾����ς��Ă��ǂ����A�֐����Ō^�ϊ����Ă��ǂ�
	void setMemberLoad( void * owner);		// setMember()�̃��[�h���ŁA���[�h���ĕs�v�Ȃ��̂̓Z�b�g���Ȃ�
	void setMemberCommon( void * owner);	// setMember()��setMemberLoad()�̋��ʏ���
	void start();							// setMember()��ɂ��Task�����グ����
	void startLoad();						// start()�̃��[�h����
	void startCommon();						// start()��startLoad()�̋��ʏ���
	
	static void damgeCB( void * argLaser);
	void damage();
	static void endEffectLaserCB( void * argLaser);
	void endEffectLaser();
	static void landingCB( void * argLaser);
	void landing();
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void nextReflection( int x, int y, int angle);

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::attack::LASER_GAME_MEMBER gm;
	
}; // CLaser
	 
} // namespece attack

} // namespace ts
} // namespace nn
	

#endif // __ATTACK_LASER_H__
