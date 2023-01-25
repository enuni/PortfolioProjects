#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"

#include "obj_main_3d_data.h"

#include "effect\\effect_ctrl.h"
#include "effect\\effect_spell.h"

#include "common\\LunaAnimation.h"

#include "gungnir.h"
#include "bit.h"

#include "ball.h"

#include "hole.h"
#include "hole_ctrl.h"
#include "ctrl.h"
#include "camera.h"
#include "animation.h"
#include "animation_data.h"
#include "edit_hole_obj.h"

#include "sound_nns.h"
#include "sound_data.sadl"

#include "musofuin.h"
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SPRITE::EFFECT_SPRITE()
: m_iFileTexture(0)
, m_iBlendMode(DISP_COMMON_BLEND_MODE_ALPHA)
, m_iAlpha(31)
{
	::nn_commonLog(1,  "EFFECT_SPRITE CREATE\n");

	
	m_vec3DScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vec3DRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_SPRITE::~EFFECT_SPRITE() {

	::nn_commonLog(1,  "EFFECT_SPRITE END\n");

}
//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPRITE::update() {


}
//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPRITE::disp() {

	int x = (int)this->m_vec3DPos.x;
	int y = (int)this->m_vec3DPos.y;
	int z = (int)this->m_vec3DPos.z;
	int w = (int)(this->m_vec3DScale.x * this->m_vec3DSize.x );
	int h = (int)(this->m_vec3DScale.y * this->m_vec3DSize.y );
	u16 r = (u16)(this->m_vec3DRotation.z);

	int b = (int)this->m_iBlendMode;
	::disp_commonObjDxCenterFull( this->m_iFileTexture
		, x, y, z, w, h, r, FALSE, FALSE, RGB_WHITE, m_iAlpha, b);

}
//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_SPRITE::dispMain() {

	int x = (int)this->m_vec3DPos.x;
	int y = (int)this->m_vec3DPos.y;
	int z = (int)this->m_vec3DPos.z;
	int w = (int)(this->m_vec3DScale.x * this->m_vec3DSize.x );
	int h = (int)(this->m_vec3DScale.y * this->m_vec3DSize.y );
	u16 r = (u16)(this->m_vec3DRotation.z);

	int b = (int)this->m_iBlendMode;
	////::disp_commonMain3dObj( this->m_iFileTexture
	////	, x, y, z, w, h, r, FALSE, FALSE, RGB_WHITE, m_iAlpha, b);

	disp_commonMain3dObj( this->m_iFileTexture, x, y, z);
}




//---------------------------------------------------------------------------

nn::yb::main::EFFECT_GUNGNIR::EFFECT_GUNGNIR()
: nn::yb::main::EFFECT_SPELL() 
////, this->m_eState(0)
////, this->m_uiAnimeCounter(0)
////, m_uiAnimeCounterLimit(30)
, m_bIsGround(true)
, m_fGroundZ(0.0f)
{

	::nn_commonLog(1,  "EFFECT_GUNGNIR CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_GUNGNIR));
	
	this->m_pBall = NULL;

}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_GUNGNIR::EFFECT_GUNGNIR( TASK_SAVE * parent, nn::yb::main::EFFECT_GUNGNIR * effectGUNGNIRArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectGUNGNIRArg, (int)(350 * nn::yb::main::EFFECT_MUSOFUIN::SCALE_FPS_MUSOFUIN), (int)(350 * nn::yb::main::EFFECT_MUSOFUIN::SCALE_FPS_MUSOFUIN) )
, m_bIsGround(true)
, m_fGroundZ(0.0f)
{
	
	::nn_commonLog(1,  "EFFECT_GUNGNIR START\n");

	this->m_bIsGround = effectGUNGNIRArg->m_bIsGround;
	this->m_pBall = effectGUNGNIRArg->m_pBall;

	////this->cnt = 1;//�q�^�X�N�����ꍇ�A�S�Ďq�^�X�N���I�������u�Ԃɐe�^�X�N���I�����Ă��܂����߁A�������g���J�E���g���Ă���

	//���e����^�[�Q�b�g�̍��W 
	this->m_vec3DTargetPos.x = effectGUNGNIRArg->m_vec3DTargetPos.x;
	this->m_vec3DTargetPos.y = effectGUNGNIRArg->m_vec3DTargetPos.y;
	this->m_vec3DTargetPos.z = effectGUNGNIRArg->m_vec3DTargetPos.z;

	
	//�Ռ��g�̍��W
	this->m_vec3DWavePos.x = effectGUNGNIRArg->m_vec3DWavePos.x;
	this->m_vec3DWavePos.y = effectGUNGNIRArg->m_vec3DWavePos.y;
	this->m_vec3DWavePos.z = effectGUNGNIRArg->m_vec3DWavePos.z;

	
	//���~���A�̈ړ���̍��W�Ǝ��@���W�̒����ɃJ�������ړ����邽�߁A�G�t�F�N�g�̈ʒu�����炷
	int iCameraX = (int)((this->m_vec3DTargetPos.x + this->camera_offset_x) / 2.0f);
	int iCameraY = (int)((this->m_vec3DTargetPos.y + this->camera_offset_y - 220.0f) / 2.0f);
	this->camera_offset_x = /*this->m_pBall->holeCtrl->ctrl->camera->x + */ iCameraX;
	this->camera_offset_y = /*this->m_pBall->holeCtrl->ctrl->camera->y + */ iCameraY;


	this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_SPELLNAME_SPELLNAME_6, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		this->totalFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		20,
		this->camera_offset_x + 200, this->camera_offset_x + 200,
		this->camera_offset_y + 140, this->camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		15,
		this->camera_offset_x + 200, this->camera_offset_x + 200,
		this->camera_offset_y + 140, this->camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		this->totalFrame - (20 + 15),
		this->camera_offset_x + 200, this->camera_offset_x + 200,
		this->camera_offset_y - 210, this->camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 0,
		this->ctrl);//�X�y�����̕���(�w��t���[���I����ɏ���: �X�y���������ƕ\��)
	this->cnt++;



	//���~���A�X�v���C�g
	this->m_sprRemiria.m_vec3DPos = D3DXVECTOR3(this->camera_offset_x - 330.0f, this->camera_offset_y - 220.0f, 169.0f);//�����ʒu�͍��㕔
	this->m_sprRemiria.m_vec3DSize = D3DXVECTOR3(104.0f, 104.0f, 0.0f);
	this->m_sprRemiria.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprRemiria.m_iAlpha = 31;//
	this->m_sprRemiria.m_iFileTexture = ::OBJ_ID_GAME_CHAR_REMILIA_1;//Obj ID
	
	//�O���j�O���X�v���C�g
	this->m_sprGungnir.m_vec3DPos = D3DXVECTOR3(320.0f, 240.0f - 126.0f, 512.0f + 170.0f);
	////this->m_sprGungnir.m_vec3DSize = D3DXVECTOR3(512.0f, 256.0f, 0.0f);
	this->m_sprGungnir.m_vec3DSize = D3DXVECTOR3(256.0f, 128.0f, 0.0f);
	this->m_sprGungnir.m_vec3DScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	this->m_sprGungnir.m_iBlendMode = DISP_COMMON_BLEND_MODE_ADD;//
	this->m_sprGungnir.m_iAlpha = 0;//�ŏ��͓���
	this->m_sprGungnir.m_iFileTexture = OBJ_ID_GUNGNIR_GUNGNIR_0;//Obj ID
	
	this->m_sprGungnir.m_vec3DRotation.z = 0.0f;//�����p�x

	this->m_taskBigbulletParent = NULL;
	
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_GUNGNIR::~EFFECT_GUNGNIR() {

	::nn_commonLog(1,  "EFFECT_GUNGNIR END\n");

}

//---------------------------------------------------------------------------

float gXShift = 16.0;
void nn::yb::main::EFFECT_GUNGNIR::task() {
	nn::yb::main::EFFECT_SPELL::task();

	//�X�L�b�v����
    ::KEY_WORK * key = &this->m_pBall-> master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		//����{�^����������Ă�����
		if (this->m_eState >= 0 
			&& this->m_eState <= 5)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 6;
			this->m_uiAnimeCounter = 0;

			//���~���A�̈ړ���̍��W�Ǝ��@���W�̒����ɃJ���������킹��
			int iCameraX = (int)((this->m_vec3DTargetPos.x + this->camera_offset_x) / 2.0f);
			int iCameraY = (int)(this->m_sprRemiria.m_vec3DPos.y + 98.0f);

			//�J�����ړ�
			this->m_pBall->holeCtrl->ctrl->camera->setCameraPos(iCameraX,iCameraY );
			this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_pBall->x, (int)(this->m_pBall->y - this->m_pBall->z), 1, NULL, NULL );

			this->m_sprRemiria.m_vec3DPos.x = this->m_vec3DTargetPos.x + gXShift;//Obj ID
			this->m_sprRemiria.m_iFileTexture = ::OBJ_ID_GAME_CHAR_REMILIA_0;//Obj ID
			this->m_sprRemiria.m_vec3DPos.z = 176.0f;

			this->m_sprGungnir.m_vec3DPos.x = 320.0f;
			this->m_sprGungnir.m_vec3DPos.y = 240.0f - 126.0f;
			this->m_sprGungnir.m_iAlpha = 31;

			//�^�[�Q�b�g�Ƃ̊p�x
			/*
			// �^���Œ�
			float fTargetAngle = (float)::atan2(
				(float)(this->m_vec3DTargetPos.y) - this->m_sprRemiria.m_vec3DPos.y,
				(float)(this->m_vec3DTargetPos.x) - (this->m_sprRemiria.m_vec3DPos.x - gXShift)); 
			fTargetAngle = (float)fmod(fTargetAngle + PI * 2 , PI * 2); // 0�`2�΂̒l�Ɋۂ߂�
			fTargetAngle *= (float)(360.0f / (2.0f * PI));//�x���ɕϊ�
			////fTargetAngle -= 180.0f;
			fTargetAngle += 180.0f;
			*/
			float fTargetAngle = 270.0f;
			this->m_sprGungnir.m_vec3DRotation.z = fTargetAngle;

			if(this->m_taskBigbulletParent != NULL) {
				this->m_taskBigbulletParent->taskDelete();
			}

			if (this->m_taskCutIn != NULL)//�J�b�g�C�����\������Ă���ꍇ�A��~
			{ 
				::animationEnd( (ANIMATION *)this->m_taskCutIn->work );
				::animationHide( (ANIMATION *)this->m_taskCutIn->work );
				this->m_taskCutIn = NULL;
			}
		}
	}

	this->disp();

	this->frameCnt++;
	bool end_flag = false;

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{
	case 0://�J�b�g�C��
		{
			if (
					this->m_uiAnimeCounter >= 30 + 15 ///�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_taskCutIn = animationExec( NULL, ANIMATION_ID_SPELL_CHAR_3, 320, 240, 100 + 513, FALSE, nn::yb::main::EFFECT_SPELL::cutin_cb, this);
				animationChangeObjId( (ANIMATION *)this->m_taskCutIn->work, ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID, OBJ_ID_SPELL_CHAR_REIMU_0, OBJ_ID_SPELL_CHAR_REMIRIA_0);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

		}
		break;

	case 1://�J�����ړ�
		{
			if (
					this->m_uiAnimeCounter >= 70 + 45 /** nn::yb::main::EFFECT_MUSOFUIN.SCALE_FPS_MUSOFUIN*///�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				//���~���A�̈ړ���̍��W�Ǝ��@���W�̒����ɃJ���������킹��
				int iCameraX = (int)((this->m_vec3DTargetPos.x + this->camera_offset_x) / 2.0f);
				int iCameraY = (int)(this->m_sprRemiria.m_vec3DPos.y + 98.0f);
				//�J�����ړ�
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos(iCameraX,iCameraY );

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

		}
		break;
	case 2://�L�����ړ�
		{
			if (
					this->m_uiAnimeCounter >= 60//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_sprRemiria.m_vec3DPos.x = (float)this->camera_offset_x + gXShift;

				::OBJ * obj = ::disp_commonGetObj( OBJ_ID_DUN_SPELL_BIGBULLET_0);

				this->m_taskBigbulletParent =  nn::yb::main::effect_out_ex( this, OBJ_ID_DUN_SPELL_BIGBULLET_0, 
					(int)(this->m_sprRemiria.m_vec3DPos.x),  
					(int)(this->m_sprRemiria.m_vec3DPos.y), 
					170, 0, 0,
					(int)(obj->w / 2.0f), 0.0f,(int)(obj->h / 2.0f), 0.0f,
					0, 0.0f, 0, 0.0f, DISP_COMMON_BLEND_MODE_ADD,
					this->ctrl, 60 + 70);

				// �G�t�F�N�g�̐���
				for( int i = 0; i < 30; i++) {

					////if(this->cardCnt + this->ballCnt < 300) {
						////if((this->cardCnt + this->ballCnt) % 5 <= 2) {
						////	int angle = ::n_commonRandN( 360);
						////	int speed = 15 + ::n_commonRandN( 5) * 2;
						////	int obj_id = obj_id_tbl[this->cardCnt % 2];
						////	::OBJ * obj = ::disp_commonGetObj( obj_id);
						////	this->ctrl->push_event(  nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
						////	nn::yb::main::effect_out_ex_a_spd( this,obj_id, this->m_sprRemiria.m_vec3DPos.x, this->m_sprRemiria.m_vec3DPos.y , 0, angle, speed, obj->w * 2, 0.0f, obj->h * 2, 0.0f, angle, 0.0f, 31, 0.0f, DISP_COMMON_BLEND_MODE_ADD, this->ctrl, -0.1f);
						////	this->cnt++;
						////	this->cardCnt++;
						////}
						////else if((this->cardCnt + this->ballCnt) % 5 <= 3) {
						////	int angle = ::n_commonRandN( 360);
						////	int speed = 15 + ::n_commonRandN( 5) * 1.5;
						////	::OBJ * obj = ::disp_commonGetObj( OBJ_ID_DUN_EFFECT_REIFUMUSOFUINSAN_11);
						////	this->ctrl->push_event(  nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
						////	nn::yb::main::effect_out_ex_a_spd( this, OBJ_ID_DUN_EFFECT_REIFUMUSOFUINSAN_11, this->m_sprRemiria.m_vec3DPos.x, this->m_sprRemiria.m_vec3DPos.y , 0, angle, speed, obj->w, 0.0f, obj->h, 0.0f, angle, 16.0f, 31, 0.0f, DISP_COMMON_BLEND_MODE_ADD, this->ctrl, -0.1f);
						////	this->cnt++;
						////	this->ballCnt++;
						////}
						////else {
							int angle = ::n_commonRandN( 360);
							int speed = 20 + ::n_commonRandN( 5) * 1;
							::OBJ * obj = ::disp_commonGetObj( OBJ_ID_DUN_SPELL_BIGBULLET_0);
							this->ctrl->push_event(  nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
							nn::yb::main::effect_out_ex_a_spd( this->m_taskBigbulletParent, OBJ_ID_DUN_SPELL_BIGBULLET_0, 
								(int)this->m_sprRemiria.m_vec3DPos.x,
								(int)this->m_sprRemiria.m_vec3DPos.y ,
								(int)170.0f, angle, speed,
								(int)(obj->w / 2.0f), 0.0f, (int)(obj->h / 2.0f), 0.0f,
								angle, 32.0f,
								31, 0.1f, DISP_COMMON_BLEND_MODE_ADD, this->ctrl,
								-0.1f,
								0.0f, 
								0.0f,
								15.0f, 
								0.0f,
								0.0f, 0.0f,
								this->m_pBall->holeCtrl->ctrl->camera->x - 320, this->m_pBall->holeCtrl->ctrl->camera->y - 240);
							this->cnt++;
							this->ballCnt++;
						////}
					////}

				}
				sound_nnsPlaySe( SE_LEVELUP);//SE

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}


			//�L�����ړ�
			float fPosX = 0.0;
			fPosX = LunaAnimation::Add2<float, int>(this->camera_offset_x - 330.0f, (float)this->camera_offset_x + gXShift, 60, this->m_uiAnimeCounter);
			this->m_sprRemiria.m_vec3DPos.x = fPosX;
			////this->m_sprRemiria.m_vec3DPos.y = fPosX;

		}
		break;
	case 3://���e�z������
		{
			if (
					this->m_uiAnimeCounter >= 70 //�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{

				printf( "next\n");
				sound_nnsPlaySe( SE_LEVELUP);//�O���j�O��������SE
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;

			}
			//�z�����݃G�t�F�N�g
			int angle = ::n_commonRandN( 360);
			int speed = 5 + ::n_commonRandN( 3) * 2;
			////int obj_id = OBJ_ID_DUN_SPELL_FOG_0;
			int obj_id = OBJ_ID_DUN_SPELL_BIGBULLET_0;
			
			int r = 600 + ::n_commonRandN(20);
			int x = (int)(::gSin[angle] * r);
			int y = (int)(-::gCos[angle] * r);

			angle += 180;
			angle %= 360;

			::OBJ * obj = ::disp_commonGetObj( obj_id);
			ctrl->push_event(  nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
			nn::yb::main::effect_out_ex( this->m_taskBigbulletParent,obj_id, 
				(int)(this->m_sprRemiria.m_vec3DPos.x /*+ this->m_sprRemiria.m_vec3DSize.x / 2.0f*/ + x),  
				(int)(this->m_sprRemiria.m_vec3DPos.y /*+ this->m_sprRemiria.m_vec3DSize.y / 2.0f*/ + y), 
				170, angle, speed, (int)(obj->w / 2.0f), -0.1f, (int)(obj->h / 2.0f), -0.1f, angle, -1.0f, 31, -0.2f, DISP_COMMON_BLEND_MODE_ADD, ctrl, 70);
			this->cnt++;
			this->cardCnt++;

			if (this->m_uiAnimeCounter % 20 + 0 == 0)
			{
				sound_nnsPlaySe( SE_BITSTART);//�r�b�g����SE
			}

		}
		break;
	case 4://�O���j�O���\���E�k��
		{
			if (
					this->m_uiAnimeCounter >= 60//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				sound_nnsPlaySe( SE_BITSTART);//�r�b�g����SE


				//�O���O�j�������w
				this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_MAGIC_CIRCLE_MAGIC_CIRCLE_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					30,
					this->camera_offset_x, this->camera_offset_x, 30,
					this->camera_offset_y - 252, this->camera_offset_y - 252, 30,
					127,
					32, 512, 30,
					32, 512, 30,
					0, 0, 30,
					31, 0, 30,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->ctrl);//�g��
				this->cnt++;


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

			//�k��
			float fScale = 0.0f;
			fScale = LunaAnimation::Add2<float, int>(1.5f, 1.0f, 60, this->m_uiAnimeCounter);
			this->m_sprGungnir.m_vec3DScale.x = fScale;
			this->m_sprGungnir.m_vec3DScale.y = fScale;

			//�t�F�[�h�C��
			int iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Add2<float, int>(0, 31, 60, this->m_uiAnimeCounter));
			this->m_sprGungnir.m_iAlpha = iAlpha;

		}
		break;
	case 5://�O���j�O������
		{
			//�^�[�Q�b�g�Ƃ̊p�x
			/*
			// �^���Œ�
			float fTargetAngle = (float)::atan2(
				(float)(this->m_vec3DTargetPos.y) - this->m_sprRemiria.m_vec3DPos.y,
				(float)(this->m_vec3DTargetPos.x) - (this->m_sprRemiria.m_vec3DPos.x - gXShift)); 
			fTargetAngle = (float)fmod(fTargetAngle + PI * 2 , PI * 2); // 0�`2�΂̒l�Ɋۂ߂�
			fTargetAngle *= (float)(360.0f / (2.0f * PI));//�x���ɕϊ�
			////fTargetAngle -= 180.0f;
			fTargetAngle += 180.0f;
			*/
			float fTargetAngle = 270.0f;
			
			if (
					this->m_uiAnimeCounter >= 45//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				sound_nnsPlaySe( SE_LEVELUP);//�O���j�O��������SE

				this->m_sprRemiria.m_iFileTexture = ::OBJ_ID_GAME_CHAR_REMILIA_0;//Obj ID
				this->m_sprRemiria.m_vec3DPos.z = 176.0f;
				this->m_sprGungnir.m_vec3DRotation.z = fTargetAngle;
				
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_pBall->x, (int)(this->m_pBall->y - this->m_pBall->z), 1, NULL, NULL );

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

			//��]
			float fRotate = 360.0f;
			////fRotate = LunaAnimation::Add2<float, int>(0.0f, -90.0f, 20, this->m_uiAnimeCounter);
			int end_frame = 10;
			int now_frame = this->m_uiAnimeCounter;
			if(now_frame > 31) {
				now_frame = 31;
			}
			fRotate = LunaAnimation::AddSpeed(360.0f, fTargetAngle + 45.0f, (float)end_frame, (float)now_frame);
			////::nn_commonLog(1,  "fRotate = %f\n", fRotate);
			this->m_sprGungnir.m_vec3DRotation.z = fRotate;

			this->m_sprGungnir.m_vec3DPos.x = 320.0f;
			this->m_sprGungnir.m_vec3DPos.y = 240.0f - 126.0f;

		}
		break;
	case 6://���i�E�����蔻��҂�
		{
			if (
					this->m_uiAnimeCounter == 35//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_pBall->x, (int)(this->m_pBall->y - this->m_pBall->z), 20, NULL, NULL );
			}
			if(
				this->m_uiAnimeCounter == 35 + 15
				)
			{
				sound_nnsPlaySe( SE_GROUND);//�r�b�g���eSE
				this->m_eState = 10;
				this->m_uiAnimeCounter = 120;
			}
////////////// �ȍ~�� this->m_eState == 10 �̏I�������܂Ŕ�΂� //////////////
			/*
			if (
					this->m_uiAnimeCounter >= 10//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				sound_nnsPlaySe( SE_GROUND);//�r�b�g���eSE

				// enuni 110506
				//this->m_pBall->setDispState( nn::yb::e::ball::DISP_STATE_PRESS, 210);

				//�J�����ړ�
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_pBall->x, (int)(this->m_pBall->y - this->m_pBall->z) );

				//�Ռ��g�̐���
				this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_FLASH_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					15,
					(int) this->m_vec3DTargetPos.x,  (int)this->m_vec3DTargetPos.x, 10,
					(int) this->m_vec3DTargetPos.y,  (int)this->m_vec3DTargetPos.y, 10,
					127,
					32, 1024, 15,
					32, 1024, 15,
					0, 0, 10,
					31, 0, 15,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->ctrl);//�g��
				this->cnt++;

				//�q�b�g�G�t�F�N�g�̐���
				this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					20,
					 (int)this->m_vec3DTargetPos.x,  (int)this->m_vec3DTargetPos.x, 30,
					 (int)this->m_vec3DTargetPos.y,  (int)this->m_vec3DTargetPos.y, 30,
					127,
					32, 512, 20,
					32, 512, 20,
					0, 0, 20,
					24, 0, 20,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->ctrl);//�g��
				this->cnt++;


				//�����n�_�ɒn�ʂ����邩�ǂ���
				if (m_bIsGround)
				{
					m_fGroundZ = 0.0f;//�n�ʂ������ꍇ
				}
				else
				{
					m_fGroundZ = -1000.0f;//�n�ʂ������ꍇ
				}

				//CBall��x, y�������x��0�ɂ���
				this->m_pBall->setSpeed( 0.0);
 
				//���̎��_�ŁABall�N���X�ɏ�����Ԃ�
				this->m_pBall->spellEffectEndGungnir();

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
			//���i
			D3DXVECTOR3 vecTarget;
			vecTarget.x = this->m_vec3DTargetPos.x;
			vecTarget.y = this->m_vec3DTargetPos.y;
			float fCurrX, fCurrY = 0.0f;
			fCurrX = LunaAnimation::Add2<float, int>((float)this->camera_offset_x, vecTarget.x, 10, this->m_uiAnimeCounter);
			fCurrY = LunaAnimation::Add2<float, int>((float)this->camera_offset_y - 220.0f, vecTarget.y, 10, this->m_uiAnimeCounter);
			this->m_sprGungnir.m_vec3DPos.x = fCurrX;
			this->m_sprGungnir.m_vec3DPos.y = fCurrY;
			*/
		}
		break;
		
	case 7://����
		{
			////bool bIsSlopeOnFlag = false;//���n�n�_��Slope���ǂ����̃t���O
			////nn::yb::stage::CHole * hole =  this->m_pBall->holeCtrl->hole;

			////if( !this->m_pBall->flyProcFlag ) {
			////	for( int i = 0; i < hole->data.holeObjN; i++) {

			////		nn::yb::def::HOLE_OBJ * obj = &hole->data.holeObj[i];
			////		if (
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_UP ||
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_UP ||
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT ||
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_RIGHT_DOWN ||
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_DOWN ||
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_DOWN ||
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT ||
			////			obj->category ==  nn::yb::e::edit_hole_obj::CATEGORY_SLOPE_LEFT_UP
			////			) 
			////		{
			////			if(/*(!this->m_pBall->flySlopeOnFlag) && */
			////				(!this->m_pBall->flyCupOnFlag) && this->m_pBall->flyGroundOnFlag &&
			////				obj->x1 <= (int)this->m_pBall->x && (int)this->m_pBall->x <= obj->x2 &&
			////				obj->y1 <= (int)this->m_pBall->y && (int)this->m_pBall->y <= obj->y2
			////			){
			////				bIsSlopeOnFlag = true;
			////				break;
			////			}
			////		}
			////	}
			////}

			if (
				////this->m_uiAnimeCounter >= 30 || //�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				////::abs(this->m_vec3DTargetPos.z) < 0.5f //z���W���\���������ꍇ
				 this->m_pBall->flyGroundOnFlag
				////&& (!bIsSlopeOnFlag) //���n�n�_��Slope�łȂ��ꍇ
				&& !this->m_pBall->flySlopeOnFlag
				)
			{
				this->m_sprGungnir.m_iAlpha = 0;//�X�s�A�𓧖���
				
				//�����n�_�ɒn�ʂ����邩�ǂ���
				if (!m_bIsGround)
				{
					//�n�ʂ������ꍇ�̓��~���A���t�F�[�h�A�E�g���ďI��
					this->m_eState = 9;
					this->m_sprRemiria.m_iFileTexture = ::OBJ_ID_GAME_CHAR_REMILIA_2;//���~���A�摜���t�F�[�h�A�E�g�ɐ؂�ւ�
					this->m_uiAnimeCounter = 0;
					break;
				}
				
				this->m_pBall->z = static_cast<int>(-0.0f);

				//���������X�y�J��Ԃɂ��Ĉꎞ��~������	
				this->m_pBall->state = nn::yb::e::ball::STATE_SPELL;

				//�J�����ړ�
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_pBall->x, (int)(this->m_pBall->y - this->m_pBall->z) );

				//CBall��x, y�������x��0�ɂ���
				this->m_pBall->setSpeed( 0.0);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
			//����
			int iFallFrame = 30;
			if (m_bIsGround)
			{
				iFallFrame = 30;//�n�ʂȂ鎞�͑�������
			}
			else
			{
				iFallFrame = 40;//�n�ʖ����ꍇ�͂�����藎��
			}
			////float fCurrZ = LunaAnimation::Add<float, int>(this->m_vec3DTargetPos.z, m_fGroundZ, iFallFrame, this->m_uiAnimeCounter);//���n�܂�

			////this->m_pBall->z = static_cast<int>(fCurrZ);

			this->m_sprGungnir.m_vec3DPos.y = (float)(this->m_pBall->y - this->m_pBall->z);

			//�t�F�[�h�A�E�g
			int iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Add2<float, int>(31, 0, 10, this->m_uiAnimeCounter));
			if (iAlpha < 0)
				iAlpha = 0;
			this->m_sprGungnir.m_iAlpha = iAlpha;

		}
		break;
	case 8://�q�b�g�G�t�F�N�g
		{
			if (
					this->m_uiAnimeCounter >= 5//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				sound_nnsPlaySe( SE_GROUND);//�r�b�g���eSE
				//�Ռ��g�̐���
				//this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_FLASH_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					15,
					(int)this->m_pBall->x, (int)this->m_pBall->x, 10,
					(int)(this->m_pBall->y - this->m_pBall->z), (int)(this->m_pBall->y - this->m_pBall->z), 10,
					127,
					32, 1024, 15,
					32, 1024, 15,
					0, 0, 10,
					31, 0, 15,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, NULL);//�g��
				//this->cnt++;

				//�q�b�g�G�t�F�N�g�̐���
				this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					30,
					(int) this->m_pBall->x,  (int)this->m_pBall->x, 30,
					(int)(this->m_pBall->y - this->m_pBall->z),  (int)(this->m_pBall->y - this->m_pBall->z), 30,
					127,
					32, 512, 30,
					32, 512, 30,
					0, 0, 30,
					24, 0, 30,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->ctrl);//�g��
				this->cnt++;


				//�G�t�F�N�g�̐���
				this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
				nn::yb::main::effect_fadein_out(this, OBJ_ID_DUN_FUMAZIN_FUMAZIN_CENTER_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					120, 1, DISP_COMMON_BLEND_MODE_ADD ,enum_effect::EFFECT_ANIME_ADD , 
					20,
					(int)this->m_pBall->x,  (int)this->m_pBall->x,
					(int)(this->m_pBall->y- this->m_pBall->z) - 208,  (int)(this->m_pBall->y- this->m_pBall->z) - 208,
					128, 256,
					512, 512,
					0, 0,
					0, 31,
					80,
					(int)this->m_pBall->x,  (int)this->m_pBall->x,
					(int)(this->m_pBall->y - this->m_pBall->z) - 208,  (int)(this->m_pBall->y - this->m_pBall->z) - 208,
					256, 256,
					512, 512,
					0, 0,
					31, 12,
					20,
					(int)this->m_pBall->x,  (int)this->m_pBall->x,
					(int)(this->m_pBall->y - this->m_pBall->z) - 208,  (int)(this->m_pBall->y - this->m_pBall->z) - 208,
					256, 128,
					512, 512,
					0, 0,
					12, 0,
					this->ctrl);//�G�t�F�N�g(�w��t���[���I����ɏ���)
				this->cnt++;
				


				this->ctrl->push_event( nn::yb::main::EFFECT_GUNGNIR::call_back, this, NULL);
				nn::yb::main::effect_loop_fadein_out(this, OBJ_ID_DUN_FUMAZIN_FUMAZIN_LOOP_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					100, 1,DISP_COMMON_BLEND_MODE_ADD ,enum_effect::EFFECT_ANIME_FLAT, 
					0, 20,
					20,
					(int)this->m_pBall->x,  (int)this->m_pBall->x,
					(int)(this->m_pBall->y - this->m_pBall->z) - 40,  (int)(this->m_pBall->y - this->m_pBall->z) - 40,
					0, 90,
					900, 900,
					0, 0,
					0, 20,
					60,
					(int)this->m_pBall->x,  (int)this->m_pBall->x,
					(int)(this->m_pBall->y - this->m_pBall->z),  (int)(this->m_pBall->y - this->m_pBall->z),
					90, 90,
					900, 900,
					0, 0,
					20, 20,
					20,
					(int)this->m_pBall->x,  (int)this->m_pBall->x,
					(int)(this->m_pBall->y - this->m_pBall->z),  (int)(this->m_pBall->y - this->m_pBall->z),
					90, 0,
					900, 900,
					0, 0,
					20, 0,
					this->ctrl);//�c�������[�v�G�t�F�N�g(�w��t���[���I����ɏ���)
				this->cnt++;
				

				this->m_sprRemiria.m_iFileTexture = ::OBJ_ID_GAME_CHAR_REMILIA_2;//���~���A�摜���t�F�[�h�A�E�g�ɐ؂�ւ�

				this->m_sprGungnir.m_iAlpha = 0;//�O���j�O���𓧖���

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}


		}
		break;
	case 9://���~���A�t�F�[�h�A�E�g
		{
			if (
					this->m_uiAnimeCounter >= 30//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				this->m_pBall->ballForm = nn::yb::e::ball::BALL_FORM_DEFAULT;
				break;
			}
			//�L�����ړ�
			float fPosX = 0.0;
			float fPosY = 0.0;
			fPosX = LunaAnimation::Add2<float, int>((float)this->camera_offset_x, (float)this->camera_offset_x + 320.0f, 30, this->m_uiAnimeCounter);
			fPosY = LunaAnimation::Add2<float, int>((float)this->camera_offset_y - 220.0f, (float)this->camera_offset_y - 460.0f, 30, this->m_uiAnimeCounter);
			this->m_sprRemiria.m_vec3DPos.x = fPosX;
			this->m_sprRemiria.m_vec3DPos.y = fPosY;

		}

		break;
	case 10://�I������
		{
			if (
					this->m_uiAnimeCounter >= 120//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_pBall->spellEffectEndGungnir();
				//�J�����𒆉��ɖ߂�
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)(this->m_pBall->x), (int)(this->m_pBall->y) );


				nn::yb::main::EFFECT_GUNGNIR::call_back( this, 0);
				
				this->m_pBall->spellEffectEnd();
				this->initialize();
				this->ctrl->clear_cb();// clear callback
				this->taskDelete();// 
				this->m_pBall->m_pCurrSpellEffect = NULL;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

		}
		break;
	}

	////if(this->frameCnt >= this->frame) {
	////	end_flag = true;
	////}
	////if(
	////	(int)(this->m_vec3DPos.x + this->w / 2) < 0 || 640 < (int)(this->m_vec3DPos.x - this->w / 2)
	////	|| (int)(this->m_vec3DPos.y + this->h / 2) < 0 || 480 < (int)(this->m_vec3DPos.y - this->h / 2)//��ʓ�����͂ݏo�����ꍇ

	////	|| (int)(this->h ) <= 0 || 0 >= (int)(this->h)//�k�����đ傫����0�ɂȂ����ꍇ
	////	) {
	////		end_flag = true;//�^�X�N�����t���O�𗧂Ă�
	////}

	//////�����ŏ��ɋK�肵���ő�t���[���܂Ői�񂾂�A�J�E���g���Ȃ�
	////if (this->frame < this->cnt)
	////{
	////	this->cnt = this->frame;
	////}

	if(	end_flag && this->cnt == 0 ) {
		this->ctrl->pop_event();
		this->taskDelete();

		//�G�t�F�N�g�̍폜
		
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_GUNGNIR::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

	////this->m_sprRemiria.dispMain();
	this->m_sprRemiria.disp();
	this->m_sprGungnir.disp();
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_GUNGNIR::call_back( void * effectGUNGNIRArg, void * dummy) {

	nn::yb::main::EFFECT_GUNGNIR * effect = (nn::yb::main::EFFECT_GUNGNIR *)effectGUNGNIRArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_GUNGNIR::anime_cb( void * effectGUNGNIRArg) {

	nn::yb::main::EFFECT_GUNGNIR * effect = (nn::yb::main::EFFECT_GUNGNIR *)effectGUNGNIRArg;

	effect->m_uiAnimeCounter = 0;
	effect->m_eState = 7;

	////effect->m_sprKanameLower.this->m_vec3DPos = D3DXVECTOR3(320, 264, 180 + 513);
	////OBJ * obj_kanameishi = disp_commonGetObj( OBJ_ID_KANAMEISHI_ITEM0_0);
	////effect->m_sprKanameLower.m_vec3DSize = D3DXVECTOR3((float)obj_kanameishi->w, (float)obj_kanameishi->h, 0.0f);

}

//---------------------------------------------------------------------------

