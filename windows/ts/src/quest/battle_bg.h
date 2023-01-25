// BATTLE_BG ��u��
// battle_bg ��u��
// BattleBg ��u��
// ts ��u��
// quest ��u��

#ifndef __BATTLE_BG_H__
#define __BATTLE_BG_H__

#include "task_save.h"
#include "master.h"
#include "disp_3d_bg.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace battle_bg {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

enum DIRECT {
	DIRECT_MAX = 16
};

} // namespece battle_bg
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct  _battle_bg_direct_member {
	nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER scene;													// Camera��Fog�̈ړ�����Scene���
	nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER polygons[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int polygonIndexTbl[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int polygonN;
	bool sceneFlag;																					// Camera��Fog�̈ړ�����Scene���̗L��
	int frame;
	int acc;
} BATTLE_BG_DIRECT_MEMBER;

typedef struct _battle_bg_link_member {
	void (*directEndCBFunc)(void *);
	void * directEndCBFuncArg;
	int directEndCBFuncId;
	void * directEndCBFuncOwner;
	nn::dxlib::disp_3d_bg::CDisp3dBg * disp3dBg;
} BATTLE_BG_LINK_MEMBER;

typedef struct _battle_bg_ctrl_member {
	nn::ts::quest::BATTLE_BG_DIRECT_MEMBER dm[nn::ts::e::battle_bg::DIRECT_MAX];
	int dmN;
	int dmIndex;
	nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER initScene;
	nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER initPolygons[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int initPolygonHandles[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int initPolygonN;
	int battleIndex;																					// ���o�J�n���ɃC���N�������g���� �o�g��1�̉��o��=1
} BATTLE_BG_CTRL_MEMBER;

typedef struct _battle_bg_member {
	nn::ts::quest::BATTLE_BG_CTRL_MEMBER cm;
	nn::ts::quest::BATTLE_BG_LINK_MEMBER lm;
} BATTLE_BG_MEMBER;
	
class CBattleBg : public TASK_SAVE {
	
public:
	CBattleBg();
	CBattleBg( TASK_SAVE * oya);
	CBattleBg( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CBattleBg();
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
	
	static void nextDirectCB( void * argBattleBg);
	void nextDirect();
	void startDirect();

	void dump();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::BATTLE_BG_MEMBER m;
	
}; // CBattleBg
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __BATTLE_BG_H__
