// LASER ��u��
// laser ��u��
// Laser ��u��
// ts ��u��
// effect ��u��

#ifndef __EFFECT_LASER_H__
#define __EFFECT_LASER_H__

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
namespace effect_laser {

enum STATE {
	STATE_NONE,
	STATE_FIRE,
	STATE_WAIT,
	STATE_FADE_OUT,
	
	STATE_MAX
};
enum FRAME_N {
	FRAME_N_WAIT = 30,
	FRAME_N_FADE_OUT = 10,

	FRAME_N_MAX
};

} // namespece effect_laser
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {

typedef struct _laser_param {
	int w;
	int h;
	int x;
	int y;
	int angle;
	int blank;
	bool cameraFlag;
	int speed;
	int atribute;

	void (* landingCBFunc)(void *);
	void * landingCBFuncArg;
	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} LASER_PARAM;

typedef struct _laser_game_membar {
	nn::ts::effect::LASER_PARAM param;
	int dispW;
	int dispX;
	int dispY;
	int stateFrameCnt;
} LASER_GAME_MEMBER;

typedef struct _laser_disp_membar {
	int objIdLaser[5];
} LASER_DISP_MEMBER;
	
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
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();


	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::effect::LASER_GAME_MEMBER gm;
	static nn::ts::effect::LASER_DISP_MEMBER dm;
	
}; // CLaser
	 
} // namespece effect

} // namespace ts
} // namespace nn
	

#endif // __EFFECT_LASER_H__
