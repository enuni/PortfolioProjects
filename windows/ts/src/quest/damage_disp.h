// DAMAGE_DISP ��u��
// damage_disp ��u��
// DamageDisp ��u��
// ts ��u��
// quest ��u��

#ifndef __DAMAGE_DISP_H__
#define __DAMAGE_DISP_H__

#include "task_save.h"
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
namespace damage_disp {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	STATE_DISP,
	
	STATE_MAX
};

} // namespece damage_disp
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _damage_disp_disp_membar {
	int objIdNumberTop;
} DAMGE_DISP_DISP_MEMBER;

typedef struct _damage_disp_param {
	int moveFrameN;
	int exFrameN;
	int startX;
	int startY;
	int endX;
	int endY;
	int z;
	int blank;
	double startExRate;
	double endExRate;
	int dmageFrameN;
	int dispFrameN;

	GXRgb rgb;
	int alpha;
	int blend;
	
	void (* deleteCBFunc)(void * deleteCBFuncArg);
	void * deleteCBFuncArg;
} DAMAGE_DISP_PARAM;

typedef struct _damage_disp_game_member {
	int moveFrameCnt;
	int exFrameCnt;
	int dispDamge;
	int damage;
	int addDamage;
	int dispFrameCnt;
	bool deleteFlag;

	nn::ts::quest::DAMAGE_DISP_PARAM param;
} DAMAGE_DISP_GAME_MEMBER;
	
class CDamageDisp : public TASK_SAVE {
	
public:
	CDamageDisp();
	CDamageDisp( TASK_SAVE * oya);
	CDamageDisp( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CDamageDisp();
	void constructCommon();					// �����o�[0�N���A�Ȃ�new������constructor�ŕK����鎖
	void task();
	void disp();
	
	void setMember( void * owner);			// �����������TASK����K�v�ȃp�����[�^�����炤 void *�^�͐錾����ς��Ă��ǂ����A�֐����Ō^�ϊ����Ă��ǂ�
	void setMemberLoad( void * owner);		// setMember()�̃��[�h���ŁA���[�h���ĕs�v�Ȃ��̂̓Z�b�g���Ȃ�
	void setMemberCommon( void * owner);	// setMember()��setMemberLoad()�̋��ʏ���
	void start();							// setMember()��ɂ��Task�����グ����
	void startLoad();						// start()�̃��[�h����
	void startCommon();						// start()��startLoad()�̋��ʏ���
	void damage( int damage);
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();


	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	static nn::ts::quest::DAMGE_DISP_DISP_MEMBER dm;
	nn::ts::quest::DAMAGE_DISP_GAME_MEMBER gm;
	
}; // CDamageDisp
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __DAMAGE_DISP_H__
