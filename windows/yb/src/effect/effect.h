
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "task_save.h"

////namespace yb {

// enum
namespace nn {
namespace yb {

namespace enum_effect {

enum {
	SLIDE_MODE_FRAME,
	SLIDE_MODE_DISP,

	SLIDE_MODE_MAX
};

enum SPELL_EFFECT_KIND{
	KIND_MUSOFUIN,//�아�u���z����v
	KIND_MUSOFUIN_SAN,//�����u���z����E�U�v
	KIND_SHINREI_MUSOFUIN,//�_��u���z����v
	KIND_FUUMAZIN,//�����u�����w�v
	KIND_NIJUUKEKKAI,//�����u��d���E�v
	KIND_NIJUUDAIKEKKAI,//�����u��d�匋�E�v

	KIND_GUNGNIR,//�_���u�X�s�A�E�U�E�O���O�j���v

	EFFECT_SPELL_KIND_MAX
};


enum {
	EFFECT_ANIME_FLAT,
	EFFECT_ANIME_ADD,
	EFFECT_ANIME_SUB,

	EFFECT_ANIME_MAX
};

}

//-----------------------------------------------------------------------------

namespace main {
	
class CEffectCtrl;

class EFFECT_THROW : public ::TASK_SAVE {

public:
	EFFECT_THROW();
	EFFECT_THROW( ::TASK_SAVE * parent, nn::yb::main::EFFECT_THROW * effectThrowArg);
	virtual ~EFFECT_THROW();
	void task();
	void disp();
	
	int dispObjId;
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int frame;
	int dispX;
	int dispY;
	int varX;
	int varY;

private:
	int cnt;

};

//-----------------------------------------------------------------------------

//srcPosXy����dstPosXy�܂ł�objDispId���ړ�����G�t�F�N�g�^�X�N�𐶐�����
//PosXy�}�X��̍��W
//�I���̒ʒm��ctrl�ɑ΂��Ă�evet_pop()�ōs��
//�e�^�X�N��ݒ肵�Ă����� �e�^�X�N���폜�����ƍ폜�����悤�ɏo����
nn::yb::main::EFFECT_THROW * effect_throw( TASK_SAVE * parent, int srcPosXy, int dstPosXy, int objDispId, nn::yb::main::CEffectCtrl * ctrlArg);

//-----------------------------------------------------------------------------

class EFFECT_SLIDE : public ::TASK_SAVE {

public:
	EFFECT_SLIDE();
	EFFECT_SLIDE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_SLIDE * effectSlideArg);
	virtual ~EFFECT_SLIDE();
	virtual void task();
	virtual void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int mode;	//�X���C�h���[�h
	int objId;	//�\���I�u�W�FID
	
	bool flipH;	//�㉺���]�`��t���O
	bool flipV;	//���E���]�`��t���O
	
	float x;	//�\��x���W
	float y;	//�\��y���W
	float vX;	//�\��x���W��1�t���[���ł̕ω���
	float vY;	//�\��y���W��1�t���[���ł̕ω���
	int z;	//�\���D�揇��
	int frame;	//�X���C�h����t���[�� ���[�h���t���[�����̂ݗL��
	float w;	//�c
	float h;	//��
	float vW;	//����1�t���[���ł̕ω���
	float vH;	//�c��1�t���[���ł̕ω���
	float r;	//�p�x
	float vR;	//�p�x��1�t���[���ł̕ω���
	float a;	//�s�����x
	float vA;	//�s�����x��1�t���[���ł̕ω���
	
	int blend;	//�u�����h���[�h(enum DISP_COMMON_BLEND_MODE�Q��)
	float aX;	//�\��x���W�̉����x
	float aY;	//�\��y���W�̉����x
	float aW;	//���̉����x
	float aH;	//�c�̉����x
	float aR;	//�p�x�̉����x
	float aA;	//�s�����x�̉����x
	
	float lX;	//�\��x���W�̌��E�l
	float lY;	//�\��y���W�̉����x
	float lW;	//���̉����x
	float lH;	//�c�̉����x
	float lR;	//�p�x�̉����x
	float lA;	//�s�����x�̉����x
	
	int camera_offset_x;//�J�����I�t�Z�b�gX
	int camera_offset_y;//�J�����I�t�Z�b�gY
protected:
	int cnt;
	
};

//-----------------------------------------------------------------------------

//�I�u�W�F��xy������ˏ�Ɉړ�����G�t�F�N�g�^�X�N�𐶐�����
//�I���̒ʒm��ctrl�ɑ΂��Ă�evet_pop()�ōs��
//�e�^�X�N��ݒ肵�Ă����� �e�^�X�N���폜�����ƍ폜�����悤�ɏo����
nn::yb::main::EFFECT_SLIDE * effect_out_simple( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, nn::yb::main::CEffectCtrl * ctrlArg);
nn::yb::main::EFFECT_SLIDE * effect_out( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, int h, int r, int a, nn::yb::main::CEffectCtrl * ctrlArg);
////nn::yb::main::EFFECT_SLIDE * effect_out_ex( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg);
nn::yb::main::EFFECT_SLIDE * effect_out_ex( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg, int frame = 60);
nn::yb::main::EFFECT_SLIDE * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_SLIDE * effectSlideArg);

//-----------------------------------------------------------------------------

//added by nullpo 20090112 
//-----------------------------------------------------------------------------
//�X�y���J�[�h���ʃG�t�F�N�g
nn::yb::main::EFFECT_SLIDE * effect_out_ex_a( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg,
													 float aX = 0.0f, float aY = 0.0f, float aW = 0.0f, float aH = 0.0f, float aR = 0.0f, float aA = 0.0f);//�����x�ݒ�L��

nn::yb::main::EFFECT_SLIDE * effect_out_ex_a_spd( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg,
													 float aSpeed = 0.0f, float aW = 0.0f, float aH = 0.0f, float aR = 0.0f, float aA = 0.0f, bool flipH = FALSE, bool flipV = FALSE, int cameraX = 0, int cameraY = 0);//�����x�ݒ�L��(speed�̉����x��ݒ�)

nn::yb::main::EFFECT_SLIDE * effect_out_ex_limit( TASK_SAVE * parent, int objId,
													 int x, int y, int z,
													 int angle, int speed, 
													 int w, float vW, int h, float vH, int r, float vR, int a, float vA,
													 int blend, nn::yb::main::CEffectCtrl * ctrlArg,
													 float aX = 0.0f, float aY = 0.0f, float aW = 0.0f, float aH = 0.0f, float aR = 0.0f, float aA = 0.0f,
													 float lX = 640.0f, float lY = 480.0f, float lW = 1024.0f, float lH = 1024.0f, float lR = 359.0f, float lA = 31.0f
													 );//�����x�{���E�ݒ�L��
nn::yb::main::EFFECT_SLIDE * effect_bg_mahouzin( TASK_SAVE * parent, int x, int y, int z, nn::yb::main::CEffectCtrl * ctrlArg);//�w�i�����w
nn::yb::main::EFFECT_SLIDE * effect_first_flash( TASK_SAVE * parent, int x, int y, int z, nn::yb::main::CEffectCtrl * ctrlArg);//�J�n���Ռ��g


//-----------------------------------------------------------------------------
//�ړ��E�g��k���E���ω��Ȃǂ̃A�j���[�V�������s���G�t�F�N�g
class EFFECT_ANIM : public ::TASK_SAVE {

public:
	EFFECT_ANIM();
	EFFECT_ANIM( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM * effectAnimArg);
	virtual ~EFFECT_ANIM();
	virtual void task();
	virtual void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int mode;	//�X���C�h���[�h
	int objId;	//�\���I�u�W�FID
	
	float x;	//�\��x���W
	float y;	//�\��y���W
	float vX;	//�\��x���W��1�t���[���ł̕ω���
	float vY;	//�\��y���W��1�t���[���ł̕ω���
	int z;	//�\���D�揇��
	int frame;	//�X���C�h����t���[�� ���[�h���t���[�����̂ݗL��
	float w;	//�c
	float h;	//��
	float vW;	//����1�t���[���ł̕ω���
	float vH;	//�c��1�t���[���ł̕ω���
	float r;	//�p�x
	float vR;	//�p�x��1�t���[���ł̕ω���
	float a;	//�s�����x
	float vA;	//�s�����x��1�t���[���ł̕ω���
	
	int blend;	//�u�����h���[�h(enum DISP_COMMON_BLEND_MODE�Q��)
	int kind;	//�A�j���[�V�����X�s�[�h(enum EFFECT_ANIME�Q��)

	float aX;	//�\��x���W�̉����x
	float aY;	//�\��y���W�̉����x
	float aW;	//���̉����x
	float aH;	//�c�̉����x
	float aR;	//�p�x�̉����x
	float aA;	//�s�����x�̉����x
	
	//�n�_�E�I�_�E�n�_����I�_�܂łɂ����鎞��
	int sx; int ex; int tx;
	int sy; int ey; int ty;
	int sw; int ew; int tw;
	int sh; int eh; int th;
	int sr; int er; int tr;
	int sa; int ea; int ta;

	
	int camera_offset_x;//�J�����I�t�Z�b�gX
	int camera_offset_y;//�J�����I�t�Z�b�gY
	GXRgb color;

protected:
	int cnt;
	
};
//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_ANIM * effect_start_end_total( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int sx, int ex, int tx,
													 int sy, int ey, int ty,
													 int z, 
													 int sw, int ew, int tw,
													 int sh, int eh, int th,
													 int sr, int er, int tr,
													 int sa, int ea, int ta,
													 int blend, 
													 int kind, 
													 GXRgb color,
													 nn::yb::main::CEffectCtrl * ctrlArg
													 );//�����l�E�I���l�E�S�̒l�ݒ�

//-----------------------------------------------------------------------------
//�ړ��E�g��k���E���ω��Ȃǂ̃A�j���[�V�������s���G�t�F�N�g(fade in/animation/fade out��3��ԑJ��)
class EFFECT_ANIM_FADE : public ::TASK_SAVE {

public:
	EFFECT_ANIM_FADE();
	EFFECT_ANIM_FADE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_FADE * effectAnimArg);
	virtual ~EFFECT_ANIM_FADE();
	void task();
	void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int mode;	//�X���C�h���[�h
	int objId;	//�\���I�u�W�FID
	
	float x;	//�\��x���W
	float y;	//�\��y���W
	int z;	//�\���D�揇��
	int frame;	//�X���C�h����t���[�� ���[�h���t���[�����̂ݗL��
	float w;	//�c
	float h;	//��
	float r;	//�p�x
	float a;	//�s�����x
	
	int blend;	//�u�����h���[�h(enum DISP_COMMON_BLEND_MODE�Q��)
	int kind;	//�A�j���[�V�����X�s�[�h(enum EFFECT_ANIME�Q��)

	
	//�n�_�E�I�_�E�n�_����I�_�܂łɂ����鎞��
	 //fade in
	 int intime;
	 int isx; int iex; 
	 int isy; int iey;
	 int isw; int iew; 
	 int ish; int ieh; 
	 int isr; int ier; 
	 int isa; int iea; 
	 //animation
	 int animtime;
	 int asx; int aex; 
	 int asy; int aey;
	 int asw; int aew; 
	 int ash; int aeh; 
	 int asr; int aer; 
	 int asa; int aea; 
	 //fade out
	 int outtime;
	 int osx; int oex; 
	 int osy; int oey;
	 int osw; int oew; 
	 int osh; int oeh; 
	 int osr; int oer; 
	 int osa; int oea; 
	 
	int camera_offset_x;//�J�����I�t�Z�b�gX
	int camera_offset_y;//�J�����I�t�Z�b�gY

protected:
	int cnt;
	
};

//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_FADE * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_FADE * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_ANIM_FADE * effect_fadein_out( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int z, 
													 int blend, 
													 int kind, 
													 //fade in
													 int intime,
													 int isx, int iex, 
													 int isy, int iey,
													 int isw, int iew, 
													 int ish, int ieh, 
													 int isr, int ier, 
													 int isa, int iea, 
													 //animation
													 int animtime,
													 int asx, int aex, 
													 int asy, int aey,
													 int asw, int aew, 
													 int ash, int aeh, 
													 int asr, int aer, 
													 int asa, int aea, 
													 //fade out
													 int outtime,
													 int osx, int oex, 
													 int osy, int oey,
													 int osw, int oew, 
													 int osh, int oeh, 
													 int osr, int oer, 
													 int osa, int oea, 
													 nn::yb::main::CEffectCtrl * ctrlArg
													 );//�����l�E�I���l�E�S�̒l�ݒ�
//-----------------------------------------------------------------------------

//���[�v�A�j���[�V�����G�t�F�N�g
class EFFECT_LOOP : public nn::yb::main::EFFECT_ANIM_FADE {

public:
	EFFECT_LOOP();
	EFFECT_LOOP( ::TASK_SAVE * parent, nn::yb::main::EFFECT_LOOP * effectAnimArg);
	virtual ~EFFECT_LOOP();
	void task();
	void disp();
	

	float m_fLoopSpeedX;//���[�v���鑬�x
	float m_fLoopSpeedY;
	float m_fMoveSpeedX;//���̃I�u�W�F�N�g�S�̂��ړ����鑬�x
	float m_fMoveSpeedY;

protected:
	float m_fLoopStartPosX;//m_cLoobObj�̓��A�摜��`�悷��J�n���W
	float m_fLoopStartPosY;//m_cLoobObj�̓��A�摜��`�悷��J�n���W
	int m_iLoopNumX;
	int m_iLoopNumY;//x,�������ɕ`�悷���

	
};

//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_LOOP * effect_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_LOOP * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_LOOP * effect_loop_fadein_out( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int z, 
													 int blend, 
													 int kind, 
													 //loop speed
													 int speed_x, int speed_y,
													 //fade in
													 int intime,
													 int isx, int iex, 
													 int isy, int iey,
													 int isw, int iew, 
													 int ish, int ieh, 
													 int isr, int ier, 
													 int isa, int iea, 
													 //animation
													 int animtime,
													 int asx, int aex, 
													 int asy, int aey,
													 int asw, int aew, 
													 int ash, int aeh, 
													 int asr, int aer, 
													 int asa, int aea, 
													 //fade out
													 int outtime,
													 int osx, int oex, 
													 int osy, int oey,
													 int osw, int oew, 
													 int osh, int oeh, 
													 int osr, int oer, 
													 int osa, int oea, 
													 nn::yb::main::CEffectCtrl * ctrlArg
													 );//�����l�E�I���l�E�S�̒l�ݒ�
//-----------------------------------------------------------------------------
//��ʐU���G�t�F�N�g�N���X
class EFFECT_QUAKE : public ::TASK_SAVE {

public:
	EFFECT_QUAKE();
	EFFECT_QUAKE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_QUAKE * effectArg);
	virtual ~EFFECT_QUAKE();
	void task();
	void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	
	int kind;	//�ʏ�U��/�����U��/�����U��
	int frame;	//�t���[��
	float w;	//�c
	float h;	//��
	
private:
	int cnt;//�J�E���^
	
};
//-----------------------------------------------------------------------------
//�h�炷�t���[���A���A�������w��
nn::yb::main::EFFECT_QUAKE * effect_quake( TASK_SAVE * parent, int kind, int frame, int w, int h, nn::yb::main::CEffectCtrl * ctrlArg );

//-----------------------------------------------------------------------------

class EFFECT_MUSOFUINSAN : public ::TASK_SAVE {

public:
	EFFECT_MUSOFUINSAN();
	EFFECT_MUSOFUINSAN( ::TASK_SAVE * parent, nn::yb::main::EFFECT_MUSOFUINSAN * effectMusofuinArg);
	virtual ~EFFECT_MUSOFUINSAN();
	void task();
	void disp();
	static void call_back( void * effectMusoufuinArg, void * dummy);
	
	nn::yb::main::CEffectCtrl * ctrl;
	
private:
	int cnt;
	int ballCnt;
	int cardCnt;
	int frameCnt;
	
};

//-----------------------------------------------------------------------------

//�����̉摜�ɂ��A�j���[�V�����G�t�F�N�g
class EFFECT_FRAME_ANIMATION : public nn::yb::main::EFFECT_SLIDE {

public:
	EFFECT_FRAME_ANIMATION();
	EFFECT_FRAME_ANIMATION( ::TASK_SAVE * parent, nn::yb::main::EFFECT_FRAME_ANIMATION * effectSlideArg);
	virtual ~EFFECT_FRAME_ANIMATION();
	void task();
	void disp();

	int objIdMax;//�A�j���[�V�����̑S����
	
};
nn::yb::main::EFFECT_FRAME_ANIMATION * effect_frame_anime( TASK_SAVE * parent, int objId, int objIdMax, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg, int frame = 60);
nn::yb::main::EFFECT_FRAME_ANIMATION * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_FRAME_ANIMATION * effectArg) ;


//-----------------------------------------------------------------------------
//�ړ��E�g��k���E���ω��Ȃǂ̃A�j���[�V�������s���G�t�F�N�g(�z�[���ł̃G�t�F�N�g�p)
class EFFECT_ANIM_HOLE : public nn::yb::main::EFFECT_ANIM {

public:
	EFFECT_ANIM_HOLE();
	EFFECT_ANIM_HOLE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_HOLE * effectAnimArg);
	virtual ~EFFECT_ANIM_HOLE();
	void task();
	void disp();

public:
	void (* pop_exec_cb)(void *, void *);//pop�����֐��̃|�C���^
};
//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_HOLE * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_HOLE * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_ANIM_HOLE * effect_start_end_total_hole( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int sx, int ex, int tx,
													 int sy, int ey, int ty,
													 int z, 
													 int sw, int ew, int tw,
													 int sh, int eh, int th,
													 int sr, int er, int tr,
													 int sa, int ea, int ta,
													 int blend, 
													 int kind, 
													 GXRgb color,
													 nn::yb::main::CEffectCtrl * ctrlArg,
													 void (*pop_exec_cb)(void *, void *)
													 );//�����l�E�I���l�E�S�̒l�ݒ�
//-----------------------------------------------------------------------------
} //namespace main

} //namespace yb
} //namespece nn

#endif __EFFECT_H__