// ATTACK_EXPLOSION ��u��
// explosion ��u��
// Explosion ��u��
// ts ��u��
// attack ��u��

#ifndef __ATTACK_EXPLOSION_H__
#define __ATTACK_EXPLOSION_H__

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
namespace explosion {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

} // namespece explosion
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace attack {

typedef struct _explosion_param {
	int x;
	int y;
	int r;
	int damage;
	unsigned int hitTargetBit;
	nn::ts::quest::CBattle * battle;
	nn::ts::quest::CQuest * quest;
	nn::ts::quest::CBall * ball;

	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} EXPLOSION_PARAM;

typedef struct _explosion_game_membar {
	EXPLOSION_PARAM param;
} EXPLOSION_GAME_MEMBER;
	
class CExplosion : public TASK_SAVE {
	
public:
	CExplosion();
	CExplosion( TASK_SAVE * oya);
	CExplosion( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CExplosion();
	void constructCommon();					// �����o�[0�N���A�Ȃ�new������constructor�ŕK����鎖
	void task();
	void disp();
	
	void setMember( void * owner);			// �����������TASK����K�v�ȃp�����[�^�����炤 void *�^�͐錾����ς��Ă��ǂ����A�֐����Ō^�ϊ����Ă��ǂ�
	void setMemberLoad( void * owner);		// setMember()�̃��[�h���ŁA���[�h���ĕs�v�Ȃ��̂̓Z�b�g���Ȃ�
	void setMemberCommon( void * owner);	// setMember()��setMemberLoad()�̋��ʏ���
	void start();							// setMember()��ɂ��Task�����グ����
	void startLoad();						// start()�̃��[�h����
	void startCommon();						// start()��startLoad()�̋��ʏ���
	static void endEffectExplosionCB( void * argLaser);
	void endEffectExplosion();
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();

	static void landingCB( void * argExplosion);
	void landing();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::attack::EXPLOSION_GAME_MEMBER gm;
	
}; // CExplosion
	 
} // namespece attack

} // namespace ts
} // namespace nn
	

#endif // __ATTACK_EXPLOSION_H__
