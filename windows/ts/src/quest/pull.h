// PULL ��u��
// pull ��u��
// Pull ��u��
// ts ��u��
// quest ��u��

#ifndef __PULL_H__
#define __PULL_H__

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
namespace pull {

enum STATE {
	STATE_NONE,
	STATE_IDLE,
	STATE_PULL,
	
	STATE_MAX
};

} // namespece pull
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _pull_game_member {
	int decideAngle;
	double nowAngle;
	double nowPullLength;
	int dispX;
	int dispY;
} PULL_GAME_MEMBER;

typedef struct _pull_disp_membar {
	int objIdArrorw;
} PULL_DISP_MEMBER;

typedef struct _pull_link_member {
	int decideCBFuncId;
	void * decideCBFuncOwner;
} PULL_LINK_MEMBER;

class CPull : public TASK_SAVE {
	
public:
	CPull();
	CPull( TASK_SAVE * oya);
	CPull( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CPull();
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

	void taskIdle();
	void taskPull();

	void dump();

	void decide( int angle);

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;


	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::PULL_GAME_MEMBER gm;
	static nn::ts::quest::PULL_DISP_MEMBER dm;
	nn::ts::quest::PULL_LINK_MEMBER lm;
	
}; // CPull
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __PULL_H__
