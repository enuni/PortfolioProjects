
#ifndef __ANIME_H__
#define __ANIME_H__

#include "task_save.h"
#include "type.h"

//------------------------------------------------------------------------------

enum {
	ANIME_FLAGS_LOOP = 0x01,		//�Ō�̃t���[���̎���0�t���[���ɖ߂�
	ANIME_FLAGS_LAST_WAIT = 0x02,	//�Ō�̃t���[���ő҂���ԂɂȂ�@�Ō�̃t���[���̎��̃t���[����endCallBackFunc���Ăԁ@���̌��end���Ă�endCallBackFunc�͌Ă΂�Ȃ�
	
	ANIME_FLAGS_MAX
};

enum {
	ANIME_LAYER_TYPE_OBJ_MAIN,
	ANIME_LAYER_TYPE_OBJ_SUB,
	ANIME_LAYER_TYPE_OBJ_SE,
	ANIME_LAYER_TYPE_ANIME,
	
	ANIME_LAYER_TYPE_OBJ_MAX
};

enum {
	ANIME_CHANGE_OBJ_ID_TYPE_LAYER,
	ANIME_CHANGE_OBJ_ID_TYPE_OBJ_ID,

	ANIME_CHANGE_OBJ_ID_TYPE_MAX
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
namespace common {
namespace dxlib {

typedef struct _anime_disp_data {
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
} ANIME_DISP_DATA;

typedef struct _anime_arc_ctrl {
	BOOL loadFlag;
	u8 * buf;
} ANIME_ARC_CTRL;

typedef struct _anime_change_obj_id_data {
	int type;
	int dst;
	int src;
} ANIME_CHAGE_OBJ_ID_DATA;

class CAnime : public TASK_SAVE {
	
public:
	CAnime();
	CAnime( TASK_SAVE * oya);
	~CAnime();
	void task();
	void disp();
	void constructorCommon();

	static void arcInit( void * arcInfo, int arcN);		// ������
	static void arcEnd( void);							// �I������
	static BOOL loadArc( int animeArcId);				// �ǂݍ���
	static BOOL releaseArc( int animeArcId);			// ���
	static void animeDisp( int animeId, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int frameCnt, BOOL stopFlag);												// �w��̃t���[���ڂ̕\�����邾��
	static void getDispData( nn::common::dxlib::ANIME_DISP_DATA * animeDispData, int animeId, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int layerNo, int frameCnt);	// �w��̃��C���[�̃t���[���ڂ̕\������animeDispData�ɓ����
	
	static CAnime * exec( TASK_SAVE * oya, int animeId, int x, int y, int z, int flags, void (* endCallBackFunc)( void *), void * endCallBackFuncArg);	// �N��
	static CAnime * execArc( int animeId, CAnime * animeArg);																							// �N���@�჌�x���@animeId�w��
	static CAnime * execBinFile( char * fileName, CAnime * animeArg);																					// �N���@�჌�x���@bin�t�@�C�������ڎw��
	
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
	
	void disp( int animeId, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int frameCnt, BOOL stopFlag);	// �w��̃t���[���ڂ̂ݕ`��

	//�p�����[�^�擾
	static s16 getFrame( int animeId);
	static s16 getLayerN( int animeId);
	static u8 * getData( int animeId);
	
private:
	static void dump_anime_arc( int animeArcId);
	static s16 get_frame_n( u8 * animeBuf);
	static s16 get_layer_n( u8 * animeBuf);
	static int get_arc_offset( u8 * arcBuf, int arcId, int animeId);
	static int get_anime_arc_id( int animeId);
	static u8 * get_anime_data( int animeId);
	static u8 * get_layer_data( int animeId, int layerNo);
	static BOOL get_disp_data( nn::common::dxlib::ANIME_DISP_DATA * animeDispData, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, int animeId, int layerNo, int frameCnt, int objW, int objH, float speedRate);
	void disp_anime( int animeId, ANIMATION_DATA * animeData, int frameCnt, int stepCnt, nn::common::dxlib::ANIME_DISP_DATA * animeDispDataMaster, BOOL stopFlag);
	
public:
	static void * sArcInfo;
	static int sArcN;
	static ANIME_ARC_CTRL * sArcCtrl;
	
	void (* destructorFunc)( void *);
	void * destructorFuncArg;
	
	void (* endCallBackFunc)( void *);	// �f�X�g���N�^���1F�����Ă�
	void * endCallBackFuncArg;			// �A�j�����ʏ�ʂ�I�������ꍇ�����\�����Ȃ������t���[��
										// �̍Ō��destructor�̃R�[���o�b�N���ĂԂ�
										// �����\�����Ȃ������u�ԂɌĂ�(�ʏ�priority = 64�̃^�X�N��)�̂ŃV�[�����X�Ɏ��̕\�����\�ɂȂ�
	
	int animeId;
	u8 * buf;
	int * keyFrameCnt;
	int frameCnt;
	
	int x;
	int y;
	int z;
	int alpha;

	ANIME_CHAGE_OBJ_ID_DATA changeObjIdArray[32];
	int changeObjIdN;

	ANIME_CHAGE_OBJ_ID_DATA changeObjIdAinAArray[32];	// �A�j�����A�j���p
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
};

} // namespcae dxlib
} // namespace common
} // namespace nn

//------------------------------------------------------------------------------


#endif //__ANIME_H__

