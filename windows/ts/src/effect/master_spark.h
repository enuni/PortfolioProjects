// MASTER_SPARK ��u��
// master_spark ��u��
// MasterSpark ��u��
// ts ��u��
// effect ��u��

#ifndef __MASTER_SPARK_H__
#define __MASTER_SPARK_H__

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
namespace master_spark {

enum STATE {
	STATE_NONE,
	STATE_FIRE,
	STATE_WAIT,
	STATE_FADE_OUT,
	
	STATE_MAX
};

enum FRAME_N {
	FRAME_N_FIRE = 10,
	FRAME_N_WAIT = 30,
	FRAME_N_FADE_OUT = 10,

	FRAME_N_MAX
};

} // namespece master_spark
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {

typedef struct _master_spark_param {
	int w;
	int h;
	int x;
	int y;
	int angle;
	int blank;
	bool cameraFlag;

	void (* endCBFunc)(void *);
	void * endCBFuncArg;
} MASTER_SPARK_PARAM;

typedef struct _master_spark_game_membar {
	MASTER_SPARK_PARAM param;
	int dispX;
	int dispY;
	int stateFrameCnt;
} MASTER_SPARK_GAME_MEMBER;

typedef struct _master_spark_disp_membar {
	int objIdMasterSpark;
	int objIdMasterSparkBase;
} MASTER_SPARK_DISP_MEMBER;
	
class CMasterSpark : public TASK_SAVE {
	
public:
	CMasterSpark();
	CMasterSpark( TASK_SAVE * oya);
	CMasterSpark( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CMasterSpark();
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

	static nn::ts::effect::MASTER_SPARK_DISP_MEMBER dm;
	MASTER_SPARK_GAME_MEMBER gm;
	
}; // CMasterSpark
	 
} // namespece effect

} // namespace ts
} // namespace nn
	

#endif // __MASTER_SPARK_H__
