#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"

#include "effect\\effect_ctrl.h"
#include "control.h"
////#include "my.h"
////#include "dungeon.h"

#include "obj_main_3d_data.h"

#include "bit.h"
#include "musofuin.h"

#include "common\\LunaAnimation.h"

#include "sound_nns.h"
#include "sound_data.sadl"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

nn::yb::main::BIT::BIT()
: m_bTargetHitEffect(false)
, spellEffectMusoCb(NULL)
, ball(NULL)
, m_iID(0)
{

	::nn_commonLog(1,  "BIT CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::BIT));

}

//---------------------------------------------------------------------------

nn::yb::main::BIT::BIT( TASK_SAVE * parent, nn::yb::main::BIT * effectArg, int frame, int  x, int y) 
: ::TASK_SAVE( parent) 
, totalFrame(frame)//�f�t�H���g�l
{
	::nn_commonLog(1,  "BIT START\n");

	const float cfFpsSpeedScale = nn::yb::main::EFFECT_MUSOFUIN::SCALE_FPS_MUSOFUIN;

	this->ctrl = effectArg->ctrl;
	this->cnt = 1;//�q�^�X�N�����ꍇ�A�S�Ďq�^�X�N���I�������u�Ԃɐe�^�X�N���I�����Ă��܂����߁A�������g���J�E���g���Ă���
	this->frameCnt = 0;
	
	this->objId = effectArg->objId;
	this->a = 31;
	this->r = 0;
	this->w = effectArg->w;
	this->h = effectArg->h;
	this->blend = DISP_COMMON_BLEND_MODE_ADD;
	this->frame = static_cast<int>(360 * cfFpsSpeedScale);//�\������ő�t���[��

	this->m_bTargetHitEffect = effectArg->m_bTargetHitEffect;
	this->spellEffectMusoCb = effectArg->spellEffectMusoCb;//���e���R�[���o�b�N
	this->ball = effectArg->ball;//
	this->m_iID = effectArg->m_iID;
 
	this->m_vec3DPos = D3DXVECTOR3((float)x, (float)y, 170.0f);//�G�t�F�N�g�I�u�W�F�N�g��z�l���w�� 
	this->m_vec3DTarget = effectArg->m_vec3DTarget;
	this->m_vec3DSpeed = D3DXVECTOR3(15.0f, 15.0f, 0.0f);
	this->m_vec3DAccel= D3DXVECTOR3(-0.25f, -0.25f, 0.0f);
	this->m_eState = 0;
	
	
	int angle = (::n_commonRandN(36) - 18) * 2;
	if (angle == 0)
		angle = 1;

	this->m_fTargetAngle = static_cast<float>(PI / (float)angle);//�ڕW�ւ̊p�x

	this->m_fHomingTargetAngle = static_cast<float>(PI / 36.0f);//�ڕW�ւ̊p�x��1��̃z�[�~���O�����ŕω������鎞�̊p�x
	this->m_uiScaleCounter = 0;//�g��k���p�J�E���^�[
	this->m_fScaleLimit = 3.0f;
	this->m_vec3DScale= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	this->m_uiAnimeCounter = 0;
	this->m_uiAnimeCounterLimit = 240;
 
}

//---------------------------------------------------------------------------

nn::yb::main::BIT::~BIT() {

	::nn_commonLog(1,  "BIT END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::BIT::task() {

	this->disp();
	this->frameCnt++;
	bool end_flag = false;

	const float cfFpsSpeedScale = nn::yb::main::EFFECT_MUSOFUIN::SCALE_FPS_MUSOFUIN;

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{
	case 0://�����ړ�(�����蔻�薳��)
		{
			const unsigned int ciTotalFrame = static_cast<unsigned int>(30 * cfFpsSpeedScale);
			if (
					this->m_uiAnimeCounter >= ciTotalFrame//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_vec3DAccel = D3DXVECTOR3(0.5f, 0.5f, 0.0f);//�����x�ύX
				this->m_vec3DScale = D3DXVECTOR3(1.0f, 1.0f, 0.0f);//�X�P�[��������

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
			}

			this->updateHoming();//�z�[�~���O���W����
			
			//�����x����
			if (this->m_vec3DAccel.x < 0.0f)
				this->m_vec3DAccel.x = 0.0f;
			if (this->m_vec3DAccel.y < 0.0f)
				this->m_vec3DAccel.y = 0.0f;

			//�X�P�[��
			this->m_vec3DScale.x = ::LunaAnimation::FlatSpeed(0.5f, 1.0f, (float)ciTotalFrame , (float)this->m_uiAnimeCounter );
			this->m_vec3DScale.y = ::LunaAnimation::FlatSpeed(0.5f, 1.0f, (float)ciTotalFrame , (float)this->m_uiAnimeCounter );
		}
		break;
	case 1://�z�[�~���O�`���e�E�q�b�g�G�t�F�N�g����
		{
			const int ciTotalFrame = (int)(15 * cfFpsSpeedScale);

			D3DXVECTOR3 vec3DSub;
			vec3DSub = this->m_vec3DPos;
			vec3DSub.Sub(m_vec3DTarget);
			float fLength = vec3DSub.Get2DLength();
			if (
				fLength < 32.0f ||
				this->m_uiAnimeCounter >= this->m_uiAnimeCounterLimit * cfFpsSpeedScale//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				sound_nnsPlaySe( SE_GROUND);//�r�b�g���eSE

				if(this->spellEffectMusoCb != NULL)
				{
					//���z����r�b�g���q�b�g���ɂ�callback��Ԃ��A�����𑱂���(hit�シ����ball�͈ړ�)
					this->spellEffectMusoCb(this->ball, this->m_iID);
				}


				//�Ռ��g�̐���
				if (this->m_bTargetHitEffect)//���e�G�t�F�N�g��ڕW���W�ɏo���ꍇ
				{
					this->m_vec3DPos.x = m_vec3DTarget.x;
					this->m_vec3DPos.y = m_vec3DTarget.y;
				}
				//this->ctrl->push_event( nn::yb::main::BIT::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_FLASH_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					ciTotalFrame,
					(int)this->m_vec3DPos.x, (int)this->m_vec3DPos.x, (int)(10 * cfFpsSpeedScale),
					(int)this->m_vec3DPos.y, (int)this->m_vec3DPos.y, (int)(10 * cfFpsSpeedScale),
					127,
					32, 1024, ciTotalFrame,
					32, 1024, ciTotalFrame,
					0, 0, (int)(10 * cfFpsSpeedScale),
					31, 0, ciTotalFrame,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, NULL);//�g��
				//this->cnt++;
				
				//�q�b�g�G�t�F�N�g�̐���
				const int ciHitTotalFrame = (int)(30 * cfFpsSpeedScale);
				this->ctrl->push_event( nn::yb::main::BIT::call_back, this, NULL);
				nn::yb::main::effect_start_end_total(this, OBJ_ID_DUN_SPELL_HIT_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					ciHitTotalFrame,
					(int)this->m_vec3DPos.x, (int)this->m_vec3DPos.x, ciHitTotalFrame,
					(int)this->m_vec3DPos.y, (int)this->m_vec3DPos.y, ciHitTotalFrame,
					178,
					32, 512, ciHitTotalFrame,
					32, 512, ciHitTotalFrame,
					0, 0, ciHitTotalFrame,
					24, 0, ciHitTotalFrame,
					DISP_COMMON_BLEND_MODE_ADD, enum_effect::EFFECT_ANIME_SUB, RGB_WHITE, this->ctrl);//�g��
				this->cnt++;

				this->ctrl->push_event( nn::yb::main::BIT::call_back, this, NULL);
				nn::yb::main::effect_quake(this, nn::yb::enum_effect::EFFECT_ANIME_SUB, ciTotalFrame, 10, 14, this->ctrl);//��ʐU��
				this->cnt++;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
			}

			this->updateHoming();//�z�[�~���O���W����

			this->m_uiScaleCounter++;
			float fScale = this->m_fScaleLimit * (this->m_uiScaleCounter) / (float)this->m_uiAnimeCounterLimit;

			//�t���[�����o�߂���ɂ�A�ǔ����\���グ��
			this->m_fHomingTargetAngle = ::LunaAnimation::FlatSpeed((float)PI / 36.0f, (float)PI / 2.0f, (float)this->m_uiAnimeCounterLimit ,(float)this->m_uiAnimeCounter );
		}
		break;
	case 2://���e���g��
		{
			const unsigned int ciTotalFrame = static_cast<unsigned int>(8 * cfFpsSpeedScale);

 			////if (this->m_uiAnimeCounter >= this->m_uiAnimeCounterLimit)
 			if (this->m_uiAnimeCounter >= ciTotalFrame)
			{
				this->m_eState++;
				this->m_uiAnimeCounter = 0;
				break;
			}
				 		
			
			this->m_uiScaleCounter++;
			float fScale = LunaAnimation::SubSpeed(1.0f, this->m_fScaleLimit, (float)ciTotalFrame, (float)this->m_uiAnimeCounter);

			this->m_vec3DScale = D3DXVECTOR3(fScale, fScale, 1.0f);

			////D3DCOLOR dwColor = m_dwColor;
			////long lAlpha = 31 * (this->m_uiAnimeCounterLimit - this->m_uiScaleCounter) / (float)30.0f/* + 16*/;
			long lAlpha = static_cast<long>(LunaAnimation::SubSpeed(31.0f, 24.0f, (float)ciTotalFrame, (float)this->m_uiAnimeCounter) );
			////m_dwColor = D3DCOLOR_ARGB(lAlpha, 0xff&(dwColor>>16) , 0xff&(dwColor>>8), 0xff&(dwColor));
			this->a = (float)lAlpha;

		}
		break;
	case 3://�g���k���E����
		{
			const unsigned int ciTotalFrame = static_cast<unsigned int>(10 * cfFpsSpeedScale);
 			if (this->m_uiAnimeCounter >= ciTotalFrame)
			{
				this->m_vec3DScale  = D3DXVECTOR3(0.0f,0.0f,1.0f);//�傫��0�ɂ���
				this->m_eState++;
				end_flag = true;
				break;
			}	
			
			float fScale = LunaAnimation::AddSpeed(this->m_fScaleLimit, 0.0f, (float)ciTotalFrame, (float)this->m_uiAnimeCounter);

			this->m_vec3DScale = D3DXVECTOR3(fScale, fScale, 1.0f);

			////long lAlpha = 31 * (this->m_uiScaleCounter) / (float)30.0f/* + 16*/;
			float Alpha = LunaAnimation::AddSpeed(24.0f, 0.0f, (float)ciTotalFrame, (float)this->m_uiAnimeCounter);
			this->a = Alpha;

		}
		break;
	}

	////if(this->mode == ::yb::enum_effect::SLIDE_MODE_FRAME) {
		if(this->frameCnt >= this->frame) {
			end_flag = true;
		}
	////}
	////else if(this->mode == ::yb::enum_effect::SLIDE_MODE_DISP) {
		if(
			////(int)(this->m_vec3DPos.x + this->w / 2) < 0 || 640 < (int)(this->m_vec3DPos.x - this->w / 2)
			////|| (int)(this->m_vec3DPos.y + this->h / 2) < 0 || 480 < (int)(this->m_vec3DPos.y - this->h / 2)|| //��ʓ�����͂ݏo�����ꍇ

			(int)(this->h ) <= 0 || 0 >= (int)(this->h)//�k�����đ傫����0�ɂȂ����ꍇ
			) {
				end_flag = true;//�^�X�N�����t���O�𗧂Ă�
		}

		//�����ŏ��ɋK�肵���ő�t���[���܂Ői�񂾂�A�J�E���g���Ȃ�
		if (this->frame < cnt)
		{
			cnt = this->frame;
		}
	////}

	if(	end_flag && this->cnt == 0 ) {
		this->ctrl->pop_event();
		this->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::BIT::disp() {

	////int x = (int)this->m_vec3DPos.x;
	////int y = (int)this->m_vec3DPos.y;
	////int z = (int)this->m_vec3DPos.z;
	////int w = /*(int)this->m_vec3DScale.x **/ this->w;
	////int h = /*(int)this->m_vec3DScale.y **/ this->h;
	////u16 a = (int)this->a;
	////u16 r = (int)this->r;

	////int b = (int)this->blend;

	int ct = 0;
	for(std::vector< D3DXVECTOR3 >::iterator it = this->m_vecAnimPos.begin();
	it != this->m_vecAnimPos.end(); it++)
	{
		int x = (int)(*it).x;
		int y = (int)(*it).y;
		int z = (int)(*it).z;
		int w = (int)(this->m_vec3DScale.x * this->w * (ct + 1) / (float)m_vecAnimPos.size());
		int h = (int)(this->m_vec3DScale.y * this->h * (ct + 1) / (float)m_vecAnimPos.size());
		u16 a = (u16)(this->a * (ct + 1) / (float)m_vecAnimPos.size());
		u16 r = (u16)this->r;

		int b = (int)this->blend;
		::disp_commonObjDxCenterFull( this->objId, x, y, z, w, h, r, FALSE, FALSE, RGB_WHITE, a, b);
		
		ct++;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::BIT::call_back( void * effectArg, void * dummy) {

	nn::yb::main::BIT * effect = (nn::yb::main::BIT *)effectArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::BIT::updateHoming() {


	float fAngle = (float)::atan2(
								(float)(m_vec3DTarget.y) -this->m_vec3DPos.y,
								 (float)(m_vec3DTarget.x)- this->m_vec3DPos.x); 
	this->m_fTargetAngle = (float)fmod(this->m_fTargetAngle + PI * 2, PI * 2); // 0�`2�΂̒l�Ɋۂ߂�
	fAngle=(float)fmod(fAngle + PI * 2 , PI * 2); // 0�`2�΂̒l�Ɋۂ߂�
	
	if ( fabs(fAngle-this->m_fTargetAngle) < this->m_fHomingTargetAngle )	//�p�x�̍���this->m_fHomingTargetAngle�ȓ��Ȃ�
	{
		fAngle = this->m_fTargetAngle;	//�p�x����v������
	}else{
		if (this->m_fTargetAngle < PI)
		{
			if (fAngle > this->m_fTargetAngle && fAngle < this->m_fTargetAngle + PI)	//�p�x���߂Â���Ȃ��
			{
				this->m_fTargetAngle+=this->m_fHomingTargetAngle;	//���v����
			}else{
				this->m_fTargetAngle-=this->m_fHomingTargetAngle;	//�����v�����
			}
		}else{
			if (fAngle < this->m_fTargetAngle && fAngle > this->m_fTargetAngle - PI)	//�p�x���߂Â���Ȃ��
			{
				this->m_fTargetAngle-=this->m_fHomingTargetAngle;	//�����v�����
			}else{
				this->m_fTargetAngle+=this->m_fHomingTargetAngle;	//���v����
			}
		}
	}
	

	//////�ŏ��̃t���[���̏ꍇ
	////if (this->m_uiAnimeCounter == 0)
	////	this->m_fTargetAngle = fAngle + PI;

	//���݂̍��W��ۑ�
	this->m_vecAnimPos.push_back(this->m_vec3DPos);
	if (this->m_vecAnimPos.size() > 10)
	{
		std::vector< D3DXVECTOR3 >::iterator it = this->m_vecAnimPos.begin();
		this->m_vecAnimPos.erase(it);
	}

	this->m_vec3DPos.x += this->m_vec3DSpeed.x * ::cos( (float)this->m_fTargetAngle );
	this->m_vec3DPos.y += this->m_vec3DSpeed.y * ::sin( (float)this->m_fTargetAngle );
	if (this->m_vec3DSpeed.x < 15.0f && this->m_vec3DSpeed.y < 15.0f)//�ō����x����
		this->m_vec3DSpeed.Add(this->m_vec3DAccel);
}

//---------------------------------------------------------------------------

nn::yb::main::BIT * nn::yb::main::effect_bit_arg( TASK_SAVE * parent, nn::yb::main::BIT * effectArg, int frame, int  x, int y) {

	nn::yb::main::BIT * effect_bit = new nn::yb::main::BIT( parent, effectArg, frame, x, y);

	return effect_bit;

}

//---------------------------------------------------------------------------
//
nn::yb::main::BIT * nn::yb::main::effect_bit2( 
		 TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, nn::yb::main::CBall* pBall, int ballCnt, void  (*pSpellEffectMusoCb)( void *, int), nn::yb::main::CEffectCtrl * ctrlArg, bool m_bTargetHitEffect
	){
		
	nn::yb::main::BIT effect_anim_arg;
	
	effect_anim_arg.ctrl = ctrlArg;

	////angle %= 360;
	effect_anim_arg.m_vec3DPos.x = (float)x;
	effect_anim_arg.m_vec3DPos.y = (float)y;
	effect_anim_arg.m_vec3DTarget.x = (float)tx;
	effect_anim_arg.m_vec3DTarget.y = (float)ty;
	effect_anim_arg.w = (float)size;
	effect_anim_arg.h = (float)size;
	effect_anim_arg.objId = OBJ_ID_DUN_SPELL_LIGHT_SPHERE_0 + color;

	effect_anim_arg.m_bTargetHitEffect = m_bTargetHitEffect; 

	effect_anim_arg.ball = pBall;
	effect_anim_arg.m_iID = ballCnt;
	effect_anim_arg.spellEffectMusoCb= pSpellEffectMusoCb;

	return nn::yb::main::effect_bit_arg( parent, &effect_anim_arg, frame, x, y);
}

//---------------------------------------------------------------------------
//
nn::yb::main::BIT * nn::yb::main::effect_bit( 
		 TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, nn::yb::main::CBall* pBall, int ballCnt, void  (*pSpellEffectMusoCb)( void *, int), nn::yb::main::CEffectCtrl * ctrlArg
	){
		
	nn::yb::main::BIT effect_anim_arg;
	
	effect_anim_arg.ctrl = ctrlArg;

	////angle %= 360;
	effect_anim_arg.m_vec3DPos.x = (float)x;
	effect_anim_arg.m_vec3DPos.y = (float)y;
	effect_anim_arg.m_vec3DTarget.x = (float)tx;
	effect_anim_arg.m_vec3DTarget.y = (float)ty;
	effect_anim_arg.w = (float)size;
	effect_anim_arg.h = (float)size;
	effect_anim_arg.objId = OBJ_ID_DUN_SPELL_LIGHT_SPHERE_0 + color;

	effect_anim_arg.ball = pBall;
	effect_anim_arg.m_iID = ballCnt;
	effect_anim_arg.spellEffectMusoCb= pSpellEffectMusoCb;

	return nn::yb::main::effect_bit_arg( parent, &effect_anim_arg, frame, x, y);
}

//---------------------------------------------------------------------------

