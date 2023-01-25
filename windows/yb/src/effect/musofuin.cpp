#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"

////#include "control.h"
////#include "my.h"
////#include "dungeon.h"

#include "obj_main_3d_data.h"

#include "effect\\effect_ctrl.h"
#include "effect\\effect_spell.h"

#include "common\\LunaAnimation.h"

#include "musofuin.h"
#include "bit.h"
#include "animation.h"
#include "animation_data.h"

#include "sound_nns.h"
#include "sound_data.sadl"

const int nn::yb::main::EFFECT_MUSOFUIN::FPS_MUSOFUIN = 60;
const float	nn::yb::main::EFFECT_MUSOFUIN::SCALE_FPS_MUSOFUIN = (float)FPS_MUSOFUIN / 30.0f;	//FPS���Z

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUIN::EFFECT_MUSOFUIN()
: nn::yb::main::EFFECT_SPELL() 
, m_bTargetHitEffect(false)
, spellEffectMusoCb(NULL)
, ball(NULL)
{

	::nn_commonLog(1,  "EFFECT_MUSOFUIN CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_MUSOFUIN));

	
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUIN::EFFECT_MUSOFUIN( TASK_SAVE * parent, nn::yb::main::EFFECT_MUSOFUIN * effectMUSOFUINArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectMUSOFUINArg, (int)(170 * SCALE_FPS_MUSOFUIN), (int)(170 * SCALE_FPS_MUSOFUIN))
{
	::nn_commonLog(1,  "EFFECT_MUSOFUIN START\n");

	//090208���ʂ�
	this->dstN = effectMUSOFUINArg->dstN;
	memcpy( this->dstLayX, effectMUSOFUINArg->dstLayX, sizeof( this->dstLayX));
	memcpy( this->dstLayY, effectMUSOFUINArg->dstLayY, sizeof( this->dstLayY));
	this->bitCnt = effectMUSOFUINArg->bitCnt;

	this->ctrl->push_event( nn::yb::main::EFFECT_MUSOFUIN::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_DUN_SPELL_NAME_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
		this->totalFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		(int)(20 * SCALE_FPS_MUSOFUIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		(int)(15 * SCALE_FPS_MUSOFUIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		(int)((this->totalFrame - (20 + 15)) * SCALE_FPS_MUSOFUIN),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y - 210, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 0,
		this->ctrl);//�X�y�����̕���(�w��t���[���I����ɏ���: �X�y���������ƕ\��)
	this->cnt++;

	//���˂���x, y���W
	this->m_iStartX = effectMUSOFUINArg->m_iStartX;
	this->m_iStartY = effectMUSOFUINArg->m_iStartY;

	this->m_bTargetHitEffect = effectMUSOFUINArg->m_bTargetHitEffect;

	this->spellEffectMusoCb = effectMUSOFUINArg->spellEffectMusoCb;
	this->ball = effectMUSOFUINArg->ball;

	this->bitRoot = new TASK_SAVE( this);
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MUSOFUIN::~EFFECT_MUSOFUIN() {

	::nn_commonLog(1,  "EFFECT_MUSOFUIN END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUIN::task() {
	nn::yb::main::EFFECT_SPELL::task();

	//�X�L�b�v����
    ::KEY_WORK * key = &this->ball->master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		//����{�^����������Ă�����
		if (this->m_eState >= 0 
			&& this->m_eState <= 2)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 99;
			this->m_uiAnimeCounter = 0;

			//��������\��������
			this->ball->setDispState( nn::yb::e::ball::DISP_STATE_DEFAULT);

			if (this->m_taskCutIn != NULL)//�J�b�g�C�����\������Ă���ꍇ�A��~
			{ 
				::animationHide( (ANIMATION *)this->m_taskCutIn->work );
				::animationEnd( (ANIMATION *)this->m_taskCutIn->work );
				this->m_taskCutIn = NULL;
			}

			//�r�b�g���폜
			this->bitRoot->taskDelete();
			
			//�Ó]�G�t�F�N�g�̍폜
			if (this->m_pBlackEffect != NULL )
			{
				//���ɋN������Ă���G�t�F�N�g������ꍇ�^�X�N������
				if (this->isLive(this->m_pBlackEffect))
				{
					this->m_pBlackEffect->taskDelete();
				}
				this->m_pBlackEffect = NULL;
			}
			if (this->ball->m_pMusoReimu != NULL)
			{
				this->ball->m_pMusoReimu->taskDelete();
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
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}

		}
		break;

	case 1://�J�b�g�C���I���҂�
		{
			if (
					this->m_uiAnimeCounter >= 120//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

		}
		break;
	case 2://�e����
		{
			if (
				this->m_uiAnimeCounter >= 25 * SCALE_FPS_MUSOFUIN + this->dstN * 10 + 60 ///�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
				//090208���ʂ�
				const int ciStartFrame = static_cast<int>(25 * SCALE_FPS_MUSOFUIN);
				if(this->bitCnt < this->dstN) {

					////if(this->frameCnt == ciStartFrame + this->bitCnt * 10) {
					if(this->m_uiAnimeCounter == ciStartFrame + this->bitCnt * 10) {
					
						sound_nnsPlaySe( SE_BITSTART);//�r�b�g����SE

						int iColor = (this->bitCnt) % 5;//�����̐F
						nn::yb::main::effect_bit2(this->bitRoot, 120, m_iStartX, m_iStartY, this->dstLayX[this->bitCnt], this->dstLayY[this->bitCnt], 128, iColor,
							this->ball, this->bitCnt, this->spellEffectMusoCb, this->ctrl, this->m_bTargetHitEffect);//
						this->bitCnt++;
					}
				}
			/*
				const int ciStartFrame = 15;
				if (this->frameCnt == ciStartFrame + 10)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 100, 100, 128, 0,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 20)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 500, 100, 128, 1,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 30)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 500, 400, 128, 2,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 40)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 100, 400, 128, 3,
						this->ctrl);//
				}
				else if (this->frameCnt == ciStartFrame + 50)
				{
					nn::yb::main::effect_bit(this, 120, 320, 240, 100, 300, 128, 4,
						this->ctrl);//
				}
			*/

		}
		break;
	case 3://�I������
		{
			if (
					this->m_uiAnimeCounter >= 10//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->ball->m_pCurrSpellEffect = NULL;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

		}
		break;
	case 99://�L�����Z���p�I������
		{
			if (
					this->m_uiAnimeCounter >= 20//
				)
			{
				nn::yb::main::EFFECT_MUSOFUIN::call_back( this, 0);
				
				this->ball->spellEffectMusoCb( this->ball, 4 );
				this->ball->spellEffectMusoReimuCb( this->ball );
				this->ball->spellEffectEndDummyCb( this->ball, NULL );
				////this->initialize();

				this->ctrl->clear_cb();// clear callback
				this->taskDelete();// 
				this->ball->m_pCurrSpellEffect = NULL;

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

void nn::yb::main::EFFECT_MUSOFUIN::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MUSOFUIN::call_back( void * effectMUSOFUINArg, void * dummy) {

	nn::yb::main::EFFECT_MUSOFUIN * effect = (nn::yb::main::EFFECT_MUSOFUIN *)effectMUSOFUINArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}
}

//---------------------------------------------------------------------------

