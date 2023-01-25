
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "task.h"
#include "type.h"

//------------------------------------------------------------------------------

enum {
	ANIMATION_FLAGS_LOOP = 0x01,		//�Ō�̃t���[���̎���0�t���[���ɖ߂�
	ANIMATION_FLAGS_LAST_WAIT = 0x02,	//�Ō�̃t���[���ő҂���ԂɂȂ�@�Ō�̃t���[���̎��̃t���[����endCallBackFunc���Ăԁ@���̌��end���Ă�endCallBackFunc�͌Ă΂�Ȃ�
	
	ANIMATION_FLAGS_MAX
};

enum {
	ANIMATION_LAYER_TYPE_OBJ_MAIN,
	ANIMATION_LAYER_TYPE_OBJ_SUB,
	ANIMATION_LAYER_TYPE_OBJ_SE,
	ANIMATION_LAYER_TYPE_ANIMATION,
	
	ANIMATION_LAYER_TYPE_OBJ_MAX
};

enum {
	ANIMATION_CHANGE_OBJ_ID_TYPE_LAYER,
	ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID,

	ANIMATION_CHANGE_OBJ_ID_TYPE_MAX
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

typedef struct _animation_disp_data {
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
} ANIMATION_DISP_DATA;

typedef struct _animation_arc_ctrl {
	BOOL loadFlag;
	u8 * buf;
} ANIMATION_ARC_CTRL;

typedef struct _animation_change_obj_id_data {
	int type;
	int dst;
	int src;
} ANIMATION_CHAGE_OBJ_ID_DATA;

typedef struct _animation {
	
	TASK * taskThis;
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	void (* endCallBackFunc)( void *);	//�f�X�g���N�^���1F�����Ă�
	void * endCallBackFuncArg;	//�A�j�����ʏ�ʂ�I�������ꍇ�����\�����Ȃ������t���[��
	//�̍Ō��destructor�̃R�[���o�b�N���ĂԂ�
	//�����\�����Ȃ������u�ԂɌĂ�(�ʏ�priority = 64�̃^�X�N��)�̂ŃV�[�����X�Ɏ��̕\�����\�ɂȂ�
	
	char fileName[256];
	int animationId;
	u8 * buf;
	int * keyFrameCnt;
	int frameCnt;
	
	int x;
	int y;
	int z;
	int alpha;
	
	/*
	//ANIMATION_CHAGE_OBJ_ID_DATA changeObjIdArray[]�ɕύX
	int changeObjIdTypeArray[16];	//0�Ȃ��@1�u�������@2�V�t�g
	int changeObjIdLayerNoArray[16];	//���샌�C���[�ԍ�
	int changeObjIdNArray[16];	//����l*/

	ANIMATION_CHAGE_OBJ_ID_DATA changeObjIdArray[32];
	int changeObjIdN;
	
	int changeObjIdAInATypeArray[16];	//0�Ȃ��@1�u�������@2�V�t�g
	int changeObjIdAInALayerNoArray[16];	//���샌�C���[�ԍ�
	int changeObjIdAInANArray[16];	//����l
	
	BOOL loopFlag;
	BOOL waitFlag;
	BOOL lastWaitFlag;
	float speedRate;
	
	int vec;
	BOOL lastWaitCallBackLateFlag;
	BOOL lastWaitExecFlag;
	
	BOOL frameWaitFlag;
	int waitFrame;
	void (* frameWaitCallBackFunc)( void *);
	void * frameWaitCallBackFuncArg;
	
	BOOL hideFlag;
	
} ANIMATION;

//------------------------------------------------------------------------------

//������
void animationArcInit( void * arcInfo, int arcN);
//�I������
void animationArcEnd( void);
//�ǂݍ���
BOOL animationLoadArc( int animationArcId);
//���
BOOL animationReleaseArc( int animationArcId);

//�N�������A�j���̐���
void animationSetSpeedRate( ANIMATION * animationArg, float rate);
void animationEnd( ANIMATION * animationArg);
void animationWait( ANIMATION * animationArg);
void animationMove( ANIMATION * animationArg);
void animationBack( ANIMATION * animationArg);
void animationHide( ANIMATION * animationArg);
void animationVisible( ANIMATION * animationArg);
void animationFrameWait( ANIMATION * animationArg, int waitFrame, void (* frameWaitCallBackFunc)( void *), void * frameWaitCallBackFuncArg);
void animationChangeObjId( ANIMATION * animationArg, int type, int dst, int src);

//�N��
TASK * animationExec( TASK * oya, int animationId, int x, int y, int z, int flags, void (* endCallBackFunc)( void *), void * endCallBackFuncArg);
//�N���@�჌�x���@animationId�w��
TASK * animationExecArc( TASK * oya, ANIMATION * animationArg);
//�N���@�჌�x���@bin�t�@�C�������ڎw��
TASK * animationExecBinFile( TASK * oya, ANIMATION * animationArg);
//�N���@�჌�x��
TASK * animationCreateTaskMain( TASK * oya, ANIMATION * animationArg);

//�w��̃��C���[�̃t���[���ڂ̕\������animationDispData�ɓ����
void animationGetDispData( ANIMATION_DISP_DATA * animationDispData, int animationId, ANIMATION_DISP_DATA * animationDispDataMaster, int layerNo, int frameCnt);

//�w��̃t���[���ڂ̂ݕ`��
void animationDisp( int animationId, ANIMATION_DISP_DATA * animationDispDataMaster, int frameCnt, BOOL stopFlag);

//�p�����[�^�擾
s16 animationGetFrame( int animationId);
s16 animationGetLayerN( int animationId);
u8 * animationGetData( int animationId);

#endif //__ANIMATION_H__

