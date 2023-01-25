// DISP_3D_BG ��u��
// disp_3d_bg ��u��
// Disp3dBg ��u��
// dxlib ��u��
// disp_3d_bg ��u��

#ifndef __DISP_3D_BG_H__
#define __DISP_3D_BG_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace dxlib {

// enum
namespace e {
namespace disp_3d_bg {

enum STATE {
	STATE_NONE,
	STATE_IDLE,
	STATE_DIRECT,
	
	STATE_MAX
};

enum POLYGON {
	POLYGON_MAX = 16
};

} // namespece disp_3d_bg
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace disp_3d_bg {

typedef struct _polygon_var_member {
	float size;			// XZ����+-size/2�𒸓_�ɂ���(target�w��
	float uvRate;		// �e�N�X�`���g�嗦(0.5����+-0.5*uvRate��UV���W�ɂ���(target�w��
	float r;			// Red(���_�J���[4�܂Ƃ߂�(target�w��
	float g;			// Green(���_�J���[4�܂Ƃ߂�(target�w��
	float b;			// Blue(���_�J���[4�܂Ƃ߂�(target�w��
	float a;			// Alpha(���_�J���[4�܂Ƃ߂�(target�w��
	float y;			// Y���ʒu(target�w��
	float u;			// U���W(target�w��
	float v;			// V���W(target�w��
	float varShiftU;	// U���X�N���[��(1�t���[���ړ��ʎw��(uvRate�̉e���͎󂯂Ȃ�
	float varShiftV;	// V���X�N���[��(1�t���[���ړ��ʎw��(uvRate�̉e���͎󂯂Ȃ�
} POLYGON_VAR_MEMBER;

typedef struct _polygon_member {
	int handle;
	float shiftU;
	float shiftV;
	POLYGON_VAR_MEMBER now;
	POLYGON_VAR_MEMBER target;
} POLYGON_MEMBER;

typedef struct _scene_var_member {
	float fogStart;
	float fogEnd;
	float cameraPositionX;
	float cameraPositionY;
	float cameraPositionZ;
	float cameraTargetX;
	float cameraTargetY;
	float cameraTargetZ;
} SCENE_VAR_MEMBER;

typedef struct _bg_member {
	SCENE_VAR_MEMBER now;
	SCENE_VAR_MEMBER target;
} SCENE_MEMBER;

typedef struct _disp_3d_bg_link_member {
	void (*directEndCBFunc)(void *);
	void * directEndCBFuncArg;
	int directEndCBFuncId;
	void * directEndCBFuncOwner;
} DISP_3D_BG_LINK_MEMBER;

typedef struct _disp_3d_bg_ctrl_member {
	int acc;
	int frame;
	int frameN;
	SCENE_MEMBER scene;
	POLYGON_MEMBER polygons[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int polygonN;
} DISP_3D_BG_CTRL_MEMBER;

typedef struct _disp_3d_bg_member {
	DISP_3D_BG_CTRL_MEMBER cm;
	DISP_3D_BG_LINK_MEMBER lm;
} DISP_3D_BG_MEMBER;
	
class CDisp3dBg : public TASK_SAVE {
	
public:
	CDisp3dBg();
	CDisp3dBg( TASK_SAVE * oya);
	CDisp3dBg( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CDisp3dBg();
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

	void initDirect();
	void setDirectScene( nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER * target);
	void setDirectPolygon( int index, nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER * target);
	void startDirect( int acc, int frame);

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	DISP_3D_BG_MEMBER m;
	
}; // CDisp3dBg
	 
} // namespece disp_3d_bg

} // namespace dxlib
} // namespace nn
	

#endif // __DISP_3D_BG_H__
