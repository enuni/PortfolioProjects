// ��3����A�j�����C�u���� disp.h .cpp �g�p��

#ifndef __ANIM_H__
#define __ANIM_H__

#include "task_save.h"
#include "type.h"

//------------------------------------------------------------------------------

enum {
	ANIM_FLAGS_LOOP = 0x01,		//�Ō�̃t���[���̎���0�t���[���ɖ߂�
	ANIM_FLAGS_LAST_WAIT = 0x02,	//�Ō�̃t���[���ő҂���ԂɂȂ�@�Ō�̃t���[���̎��̃t���[����endCallBackFunc���Ăԁ@���̌��end���Ă�endCallBackFunc�͌Ă΂�Ȃ�
	ANIM_FLAGS_CAMERA = 0x04,	// disp��cameraFlag default=false
	
	ANIM_FLAGS_MAX
};

enum {
	ANIM_LAYER_TYPE_OBJ_MAIN,
	ANIM_LAYER_TYPE_OBJ_SUB,
	ANIM_LAYER_TYPE_OBJ_SE,
	ANIM_LAYER_TYPE_ANIM,
	
	ANIM_LAYER_TYPE_OBJ_MAX
};

enum {
	ANIM_CHANGE_OBJ_ID_TYPE_LAYER,
	ANIM_CHANGE_OBJ_ID_TYPE_OBJ_ID,

	ANIM_CHANGE_OBJ_ID_TYPE_MAX
};

//AnimationConverter.rb�@�́@-c�I�v�V�����Ŏ������������t�@�C��
#include "animation_class.h"
#ifndef __ANIMATION_CLASS_H__
//animation_class.h�@���p�ӂł��Ȃ��������p�@�����o�[���ύX����Ă��鎖������̂Œ��ӂ���

typedef struct _animation_data {
	short dataOffset;
	short layerN;
} ANIMATION_DATA;

typedef struct _layer_data {
	short objId;
	short mainFlag;
	short keyFrameN;
} LAYER_DATA;

typedef struct _key_frame_data {
	short startFrame;
	short enableFrame;
	short x;
	short y;
	short tweenFlag;
	short acc;
} KEY_FRAME_DATA;

typedef struct _animation_arc_header {
	short dataOffset;
} ANIMATION_ARC_HEADER;

typedef struct _animation_arc_info {
	char * arcFileName;
	short animationOffset;
	short animationN;
} ANIMATION_ARC_INFO;

#endif //__ANIMATION_CLASS_H__

namespace nn {
namespace dxlib {

typedef struct _anim_disp_data {
	int objId;
	int x;
	int y;
	int wr;			//����10000�{
	int hr;			//�c��10000�{
	int z;
	int angle;
	BOOL flipH;
	BOOL flipV;
	int r;
	int g;
	int b;
	int a;
	int blendMode;
} ANIM_DISP_DATA;

typedef struct _anim_arc_ctrl {
	BOOL loadFlag;
	u8 * buf;
} ANIM_ARC_CTRL;

typedef struct _anim_change_obj_id_data {
	int type;
	int dst;
	int src;
} ANIM_CHAGE_OBJ_ID_DATA;

class CAnim : public TASK_SAVE {
	
public:
	CAnim();
	CAnim( TASK_SAVE * oya);								// TASK�Ƃ��Ď��s������������NULL�ł���������new
	CAnim( TASK_SAVE * oya, nn_common::SaveManager * m);
	~CAnim();
	void task();
	//void disp();
	void constructorCommon();

	static void arcInit( void * arcInfo, int arcN);		// ������
	static void arcEnd( void);							// �I������
	static BOOL loadArc( int animArcId);				// �ǂݍ���
	static BOOL releaseArc( int animArcId);				// ���
	static void animDispSimpleShift( int animId, int layoutId, int frameCnt,
		BOOL stopFlag);		// �w��̃t���[���ڂ̕\�����邾�� stopFlag == TRUE ��SE����
	static void animDisp( int animId, nn::dxlib::ANIM_DISP_DATA * animDispDataMaster,
		int frameCnt, BOOL stopFlag);	// �w��̃t���[���ڂ̕\�����邾�� stopFlag == TRUE ��SE����
	static BOOL getDispData( nn::dxlib::ANIM_DISP_DATA * animDispData, int animId,
		nn::dxlib::ANIM_DISP_DATA * animDispDataMaster,int layerNo,
		int frameCnt);	// �w��̃��C���[�̃t���[���ڂ̕\������animDispData�ɓ����
	
	static CAnim * exec( TASK_SAVE * oya, int animId, int x, int y, int z, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// �N��
	static CAnim * execArc( int animId, CAnim * animArg);				// �N���@�჌�x���@animId�w��
	static CAnim * execBinFile( char * fileName, CAnim * animArg);		// �N���@�჌�x���@bin�t�@�C�������ڎw��
	static CAnim * execSimple( TASK_SAVE * oya, int animId, int layoutId, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// �N�� x y z alpha ��layoutId���玝���Ă���
	static CAnim * execAlpha( TASK_SAVE * oya, int animId, int x, int y, int z, int alpha, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// �N�� ���l�w���
	static CAnim * execSave( TASK_SAVE * oya, nn_common::SaveManager * saveManager,
		int animId, int x, int y, int z, int alpha, int flags,
		void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// �N�� ���l�w���

	//�N�������A�j���̐���
	void setSpeedRate( float rate);
	void end();
	void wait();
	void move();
	void back();
	void hide();
	void visible();
	void frameWait( int waitFrame, void (* frameWaitCallBackFunc)( void *), void * frameWaitCallBackFuncArg);
	void changeObjId( int type, int dst, int src);
	
	//void disp( int animId, nn::dxlib::ANIM_DISP_DATA * animDispDataMaster, int frameCnt, BOOL stopFlag);	// �w��̃t���[���ڂ̂ݕ`��

	//�p�����[�^�擾
	static s16 getFrame( int animId);
	static s16 getLayerN( int animId);
	static u8 * getData( int animId);

	static int utilCalcMove( int start, int end, int acc, int frame, int now);		// acc=0�ő���� acc=33���� acc=66�ő匸��
	static float utilCalcMove( float start, float end, int acc, int frame, int now);
	
private:
	static void dump_anim_arc( int animArcId);
	static s16 get_frame_n( u8 * animBuf);
	static s16 get_layer_n( u8 * animBuf);
	static int get_arc_offset( u8 * arcBuf, int arcId, int animId);
	static int get_anim_arc_id( int animId);
	static u8 * get_anim_data( int animId);
	static u8 * get_layer_data( int animId, int layerNo);
	static BOOL get_disp_data( nn::dxlib::ANIM_DISP_DATA * animDispData,
		nn::dxlib::ANIM_DISP_DATA * animDispDataMaster, int animId, int layerNo, int frameCnt,
		int objW, int objH, float speedRate);
	void disp_anim( int animId, ANIMATION_DATA * animData, int frameCnt, int stepCnt,
		nn::dxlib::ANIM_DISP_DATA * animDispDataMaster, BOOL stopFlag);
	
public:
	static void * sArcInfo;
	static int sArcN;
	static ANIM_ARC_CTRL * sArcCtrl;
	
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	void (* endCallBackFunc)( void *);	// �f�X�g���N�^���1F�����Ă�
	void * endCallBackFuncArg;			// �A�j�����ʏ�ʂ�I�������ꍇ�����\�����Ȃ������t���[��
										// �̍Ō��destructor�̃R�[���o�b�N���ĂԂ�
										// �����\�����Ȃ������u�ԂɌĂ�(�ʏ�priority = 64�̃^�X�N��)�̂ŃV�[�����X�Ɏ��̕\�����\�ɂȂ�
	
	// �^�X�N�Z�[�u�Ή�
	virtual void loadPostProc();
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();

	int animId;
	u8 * buf;
	int * keyFrameCnt;
	int frameCnt;
	
	int x;
	int y;
	int z;
	int alpha;

	ANIM_CHAGE_OBJ_ID_DATA changeObjIdArray[32];
	int changeObjIdN;

	ANIM_CHAGE_OBJ_ID_DATA changeObjIdAinAArray[32];	// �A�j�����A�j���p
	int changeObjIdAinAN;								// �A�j�����A�j���p
	
	BOOL loopFlag;
	BOOL waitFlag;
	BOOL lastWaitFlag;
	float speedRate;
	
	int vec;
	BOOL lastWaitCallBackLateFlag;	// lastWaitFlag = TRUE���AendCallBackFunc�̌Ăяo����1�t���[���x�点�邽�߂̓�������t���O
#if 0
	// ���Ԃ�A����Ȃ�
	BOOL lastWaitExecFlag;			// lastWaitFlag = TRUE���AendCallBackFunc���Ăяo���ς݂Ȃ�TRUE
#endif
	
	BOOL frameWaitFlag;							// ����t���[���ɗ�����A������~����悤�ɂ���t���O
	int waitFrame;								// ����t���[���ɗ�����A������~����t���[����
	void (* frameWaitCallBackFunc)( void *);	// ����t���[���ɗ������̎�����~�Ŏ�����~�������̃R�[���o�b�N�֐�
	void * frameWaitCallBackFuncArg;			// ����t���[���ɗ������̎�����~�Ŏ�����~�������̃R�[���o�b�N�֐��̈���
	
	BOOL hideFlag;								// �A�j���̕\�����������Ȃ��t���O
	bool cameraFlag;
	
	// �^�X�N�Z�[�u�Ή�
	static nn_common::DATARECODE dataRecode[];
};

} // namespcae dxlib
} // namespace nn

//------------------------------------------------------------------------------


#endif //__ANIM_H__

