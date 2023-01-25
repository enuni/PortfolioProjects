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

#include "flan.h"
#include "bit.h"
#include "ball.h"
#include "animation.h"
#include "animation_data.h"
#include "effect\\effect_star.h"
#include "sound_nns.h"
#include "sound_data.sadl"

#include "yb_common.h"

#include "camera.h"
#include "ctrl.h"
#include "hole_ctrl.h"

#include "enum.h"

const int nn::yb::main::EFFECT_FLAN::FPS_FLAN = 60;
const float	nn::yb::main::EFFECT_FLAN::SCALE_FPS_FLAN = (float)FPS_FLAN / 30.0f;	//FPS���Z
const int nn::yb::main::EFFECT_FLAN::YUKKURI_2_FLAN_FRAME = 60;

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FLAN::EFFECT_FLAN()
: nn::yb::main::EFFECT_SPELL() {

	::nn_commonLog(1,  "EFFECT_FLAN CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_FLAN));

	this->iStarCt = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FLAN::EFFECT_FLAN( TASK_SAVE * parent, nn::yb::main::EFFECT_FLAN * effectFLANArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectFLANArg, (int)(1000 * SCALE_FPS_FLAN), (int)(1000 * SCALE_FPS_FLAN))
{
	::nn_commonLog(1,  "EFFECT_FLAN START\n");


	this->shadowX = effectFLANArg->shadowX;
	this->shadowY = effectFLANArg->shadowY;
	this->srcX = effectFLANArg->srcX;
	this->srcY = effectFLANArg->srcY;
	this->shadowSpeed = effectFLANArg->shadowSpeed;
	this->z = effectFLANArg->z;
	this->angle = effectFLANArg->angle;
	this->m_pBall = effectFLANArg->m_pBall;

	this->m_vec3DStartPos.x = effectFLANArg->m_vec3DStartPos.x;
	this->m_vec3DStartPos.y = effectFLANArg->m_vec3DStartPos.y;
	this->m_vec3DStartPos.z = effectFLANArg->m_vec3DStartPos.z;

	this->iStarCt = 0;

	//�t�����X�v���C�g
	this->m_sprFlan.m_vec3DPos = D3DXVECTOR3(camera_offset_x + 400.0f, (float)camera_offset_y, (float)(nn::yb::e::global::DISP_Z_SPELL_BALL + 1));//�����ʒu�͉E��, ��������艜
	this->m_sprFlan.m_vec3DSize =  D3DXVECTOR3(128.0f, 112.0f, 0.0f);
	this->m_sprFlan.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprFlan.m_iAlpha = 31;//
	this->m_sprFlan.m_iFileTexture = OBJ_ID_GAME_CHAR_FLAN_1;//Obj ID
	
	//�t�����V���G�b�g�X�v���C�g
	this->m_sprFlanShadow.m_vec3DPos = D3DXVECTOR3(camera_offset_x + 400.0f, (float)camera_offset_y, (float)(nn::yb::e::global::DISP_Z_SPELL_BALL - 1));//�����ʒu�͉E��, ����������O
	this->m_sprFlanShadow.m_vec3DSize =  D3DXVECTOR3(128.0f, 112.0f, 0.0f);
	this->m_sprFlanShadow.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprFlanShadow.m_iAlpha = 31;//
	this->m_sprFlanShadow.m_iFileTexture = OBJ_ID_GAME_CHAR_FLAN_0;//Obj ID

	this->ctrl->m_effect3D_z = static_cast<int>(this->m_sprFlan.m_vec3DPos.z - 1);//�G�t�F�N�g��Z�l���w��

}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_FLAN::~EFFECT_FLAN() {

	::nn_commonLog(1,  "EFFECT_FLAN END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_FLAN::task() {
	nn::yb::main::EFFECT_SPELL::task();

	static int iCt = 0;//�A�j���[�V�����p�J�E���^
	static unsigned int iCurrShoadowNum = 0;//���݉��Ԗڂ̉e��\�����Ă��邩�̃J�E���^

	//�X�L�b�v����
    ::KEY_WORK * key = &this->m_pBall-> master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		//����{�^����������Ă�����
		if(this->m_eState >= 0
			&& this->m_eState <= 1)
		{
			this->m_sprFlanShadow.m_iAlpha = 0;//�V���G�b�g�͓�����

			this->m_uiAnimeCounter = 0;
			this->iStarCt = 0;//�J�E���^�����Z�b�g
			this->m_eState = 2;
		}
		else if (this->m_eState >= 2
			&& this->m_eState <= 6)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 7;
			this->m_uiAnimeCounter = 0;
			this->iStarCt = 0;//�J�E���^�����Z�b�g

			iCt = 0;
			iCurrShoadowNum = 0;

			this->m_sprFlan.m_iFileTexture = OBJ_ID_GAME_CHAR_FLAN_3;//��ы���
			this->m_sprFlan.m_vec3DPos.y = (float)(this->m_pBall->y - this->m_pBall->z  - 300.0);
				
			this->m_pBall->setDispState(nn::yb::e::ball::DISP_STATE_DEAD, 0);

			if (this->m_taskCutIn != NULL)//�J�b�g�C�����\������Ă���ꍇ�A��~
			{ 
				::animationEnd( (ANIMATION *)this->m_taskCutIn->work );
				::animationHide( (ANIMATION *)this->m_taskCutIn->work );
				this->m_taskCutIn = NULL;
			}
		}
	}

	static int old_state = 0;
	if(old_state != this->m_eState) {
		::nn_commonLog(1, "this->m_eState = %d\n", this->m_eState);
		old_state = this->m_eState;
	}

	this->disp();

	this->frameCnt++;
	bool end_flag = false;
	
	if(this->m_taskCutIn != NULL) {
		((ANIMATION *)this->m_taskCutIn->work)->x++;
	}

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{

	case 0://�X�y�J�J�n�G�t�F�N�g�I���҂�
		{
			if (
				this->m_uiAnimeCounter >= 30//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

		}
		break;


	case 1://�t�����̃V���G�b�g�����y�ɍ��킹�ďo�����ăt�F�[�h�A�E�g
		{

			const int n = 7;
			const int alpha_n = 80;

			//�S����n��A�w��t���[�����ɏo���E�t�F�[�h�A�E�g
			//�ʒu
			const int c_posX[n] = { -240,  240, -180, 140, -100,  70,   0};
			const int c_posY[n] = {  160, -160, -130, 100,   70,  30, -40};
			//�o���^�C�~���O
			const unsigned  int c_shadowFrame[n + 1] = {1, 120, 245, 370, 520, 645, 795, 940};

			if (this->m_uiAnimeCounter >= c_shadowFrame[iCurrShoadowNum])
			{
				if (iCurrShoadowNum >= n)
				{
					this->m_sprFlanShadow.m_iAlpha = 0;//�V���G�b�g�͓�����
					iCt = 0;
					iCurrShoadowNum = 0;

					this->m_uiAnimeCounter = 0;
					this->m_eState++;
				}
				else
				{
					iCt = 0;//�J�E���^�����Z�b�g
					this->m_sprFlanShadow.m_vec3DPos.x = (float)(c_posX[iCurrShoadowNum] + this->m_pBall->x);
					this->m_sprFlanShadow.m_vec3DPos.y = (float)(c_posY[iCurrShoadowNum] + this->m_pBall->y - this->m_pBall->z );

					iCurrShoadowNum++;//���J�E���^���C���N�������g
				}
			}

			int iAlpha = LunaAnimation::Sub2(32, 0, alpha_n, iCt);
			this->m_sprFlanShadow.m_iAlpha = iAlpha;
			iCt++;//�J�E���^���C���N�������g
			if (iCt >= alpha_n)
				iCt = alpha_n;
		}
		break;

	case 2://�t�����t�F�[�h�C��
		{
			if (
				this->m_uiAnimeCounter >= 80//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_taskCutIn = animationExec( NULL, ANIMATION_ID_SPELL_FLAN, 320, 240, 100 + 513, FALSE, nn::yb::main::EFFECT_SPELL::cutin_cb, this);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
			const int iFrame = 40;

			//�L�����t�F�[�h�C��
			float fPosY = 0.0;
			int cnt = this->m_uiAnimeCounter;
			if(cnt > iFrame) {
				cnt = iFrame;
			}
			fPosY = (float)LunaAnimation::Add<double, int>(this->m_pBall->y - this->m_pBall->z -72.0f, this->m_pBall->y - this->m_pBall->z  - 24.0f, iFrame, cnt);
			this->m_sprFlan.m_vec3DPos.y = fPosY;
			this->m_sprFlan.m_vec3DPos.x = this->m_vec3DStartPos.x + 34.0f;

			int iAlpha = LunaAnimation::Add<int, int>(0, 31, iFrame, this->m_uiAnimeCounter);
			this->m_sprFlan.m_iAlpha = iAlpha;
		}
		break;

	case 3://�t�������݂�
		{
			if (
				this->m_uiAnimeCounter >= 30//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_uiAnimeCounter = 0;
				sound_nnsPlaySe( SE_GROUND);//SE
				sound_nnsPlaySe( SE_GYA);//SE

				 //�t�����摜���X�V
				this->m_sprFlan.m_iFileTexture = ::OBJ_ID_GAME_CHAR_FLAN_2;//���݂�
				this->m_sprFlan.m_vec3DPos.y = (float)(this->m_pBall->y - this->m_pBall->z  - 13.0);
				this->m_sprFlan.m_vec3DPos.z = (float)(nn::yb::e::global::DISP_Z_SPELL_BALL - 1);
				
				this->m_pBall->setDispState(nn::yb::e::ball::DISP_STATE_DEAD, 0);

				this->m_eState++;
				break;
			}
		}
		break;
	case 4://�X�y�J���\��
		{
			if (
				this->m_uiAnimeCounter >= 30//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_uiAnimeCounter = 0;
				/*
				//�Ռ��g�̐���
				this->ctrl->push_event( nn::yb::main::EFFECT_FLAN::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_FLASH_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					15,
					(int) m_pBall->x, (int)m_pBall->x, 10,
					(int)( m_pBall->y - m_pBall->z ), (int)( m_pBall->y - m_pBall->z ), 10,
					127,
					32, 1024, 15,
					32, 1024, 15,
					0, 0, 10,
					31, 0, 15,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, this->ctrl);//�g��
				this->cnt++;

				//�q�b�g�G�t�F�N�g�̐���
				this->ctrl->push_event( nn::yb::main::EFFECT_FLAN::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					30,
					(int) m_pBall->x,  (int)m_pBall->x, 30,
					(int) ( m_pBall->y - m_pBall->z ),  (int)( m_pBall->y - m_pBall->z ), 30,
					127,
					32, 512, 30,
					32, 512, 30,
					0, 0, 30,
					24, 0, 30,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, this->ctrl);//�g��
				this->cnt++;
				*/
				
				//�X�y�J���\��
				this->ctrl->push_event( nn::yb::main::EFFECT_FLAN::call_back, this, NULL);
				nn::yb::main::effect_fadein_out(this, OBJ_ID_SPELLNAME_SPELLNAME_8, //
					nn::yb::enum_effect::SLIDE_MODE_FRAME,
					(int)(this->totalFrame * 0.5f), 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
					(int)(20 * SCALE_FPS_FLAN),
					camera_offset_x + 180, camera_offset_x + 180,
					camera_offset_y + 140, camera_offset_y + 140,
					1024, 256,
					128, 32,
					0, 0,
					0, 31,
					(int)(15 * SCALE_FPS_FLAN),
					camera_offset_x + 180, camera_offset_x + 180,
					camera_offset_y + 140, camera_offset_y - 210,
					256, 256,
					32, 32,
					0, 0,
					31, 31,
					(int)((this->totalFrame * 0.5f - (20 + 15)) * SCALE_FPS_FLAN),
					camera_offset_x + 180, camera_offset_x + 180,
					camera_offset_y - 210, camera_offset_y - 210,
					256, 256,
					32, 32,
					0, 0,
					31, 0,
					this->ctrl);//�X�y�����̕���(�w��t���[���I����ɏ���: �X�y���������ƕ\��)
				this->cnt++;

				this->m_eState++;
				break;
			}
		}
		break;
	case 5://�X�y�J���\���҂�
		{
			if (
				this->m_uiAnimeCounter >= 120//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_uiAnimeCounter = 0;
				
				this->m_sprFlan.m_iFileTexture = ::OBJ_ID_GAME_CHAR_FLAN_3;//��ы���
			
				this->m_eState++;
				break;
			}
			if(
				this->m_uiAnimeCounter == 20
				)
			{
				//�Ռ��g�̐���
				//this->ctrl->push_event( nn::yb::main::EFFECT_FLAN::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_FLASH_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					15,
					(int) m_pBall->x, (int)m_pBall->x, 10,
					(int)( m_pBall->y - m_pBall->z ), (int)( m_pBall->y - m_pBall->z ), 10,
					127,
					32, 1024, 15,
					32, 1024, 15,
					0, 0, 10,
					31, 0, 15,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, NULL);//�g��
				//this->cnt++;
			}
		}
		break;
		/*
	case 5://�J�b�g�C��
		{
			if (
				this->m_uiAnimeCounter >= 30 + 15 ///�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_taskCutIn = animationExec( NULL, ANIMATION_ID_SPELL_CHAR_3, 320, 240, 100 + 513, FALSE, nn::yb::main::EFFECT_SPELL::cutin_cb, this);
				animationChangeObjId( (ANIMATION *)this->m_taskCutIn->work, ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID, OBJ_ID_SPELL_CHAR_REIMU_0, OBJ_ID_SPELL_CHAR_FLAN_0);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;

	case 6://�J�b�g�C���I���҂�
		{
			if (
				this->m_uiAnimeCounter >= 150//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_sprFlan.m_iFileTexture = ::OBJ_ID_GAME_CHAR_FLAN_3;//��ы���

				this->m_pBall->setDispState(nn::yb::e::ball::DISP_SATTE_PERA, 120);//�������z���S��Ԃ�

				sound_nnsPlaySe( SE_LEVELUP);//�ړ� SE
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;
		*/
	case 6://�t��������ʊO�ֈړ�
		{
			if (
				this->m_uiAnimeCounter >= 70//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
			const int iFrame = 70;
		
			//�t��������ʊO��
			float fCurrY = (float)LunaAnimation::Add<double, int>(this->m_pBall->y - this->m_pBall->z - 16.0 , this->m_pBall->y - this->m_pBall->z  - 300.0, iFrame, this->m_uiAnimeCounter);//

			if(this->m_uiAnimeCounter / 10 % 2 == 0) {	
				this->m_sprFlan.m_iFileTexture = ::OBJ_ID_GAME_CHAR_FLAN_3;//��ы���
			}
			else {
				this->m_sprFlan.m_iFileTexture = ::OBJ_ID_GAME_CHAR_FLAN_4;//��ы���
			}
			this->m_sprFlan.m_vec3DPos.y = fCurrY;
		}
		break;
	case 7://�t������
		{
			if (
				this->m_uiAnimeCounter >= nn::yb::main::EFFECT_FLAN::YUKKURI_2_FLAN_FRAME//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_pBall->ballForm = nn::yb::e::ball::BALL_FORM_VAMPIRE;//�������z���S��Ԃ�
				this->m_pBall->setDispState(nn::yb::e::ball::DISP_STATE_DEFAULT, 0);
				this->m_pBall->flyVampireRemainingFrame = 60 * 10 + 15;
 
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
			if(
				this->m_uiAnimeCounter == nn::yb::main::EFFECT_FLAN::YUKKURI_2_FLAN_FRAME / 2
				)
			{
				sound_nnsPlaySe( SE_AAAAA);
			}
		}
		break;
	case 8://�I������
		{
			if (
				this->m_uiAnimeCounter >= 10//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->ctrl->stopEffect();//stop 3D effect

				nn::yb::main::EFFECT_FLAN::call_back( this, 0);
				
				this->m_pBall->spellEffectEndFlan();
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
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_FLAN::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

	this->m_sprFlan.disp();

	const int ciWidthScale = 20;
	switch ( this->m_eState )
	{
	case 1://
		{
			this->m_sprFlanShadow.disp();//�t�����V���G�b�g
		}
		break;
	case 7:
		{
			int alpha = (int)LunaAnimation::Add<int, int>(0, 31,
				nn::yb::main::EFFECT_FLAN::YUKKURI_2_FLAN_FRAME, this->m_uiAnimeCounter
			);
			disp_commonObjCenterColor( OBJ_ID_MAIN_YUKKURI_FLAN_2,
				(int)this->m_pBall->x, (int)this->m_pBall->y - 16 - (int)this->m_pBall->z,
				nn::yb::e::global::DISP_Z_SPELL_BALL - 1,
				RGB_WHITE, alpha
			);
		}
		break;
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_FLAN::call_back( void * effectFLANArg, void * dummy) {

	nn::yb::main::EFFECT_FLAN * effect = (nn::yb::main::EFFECT_FLAN *)effectFLANArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

