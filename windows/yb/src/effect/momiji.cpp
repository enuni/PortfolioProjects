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

#include "momiji.h"
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

const int nn::yb::main::EFFECT_MOMIJI::FPS_MOMIJI = 60;
const float	nn::yb::main::EFFECT_MOMIJI::SCALE_FPS_MOMIJI = (float)FPS_MOMIJI / 30.0f;	//FPS���Z

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MOMIJI::EFFECT_MOMIJI()
: nn::yb::main::EFFECT_SPELL() {

	::nn_commonLog(1,  "EFFECT_MOMIJI CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_MOMIJI));

	this->iStarCt = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MOMIJI::EFFECT_MOMIJI( TASK_SAVE * parent, nn::yb::main::EFFECT_MOMIJI * effectMOMIJIArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectMOMIJIArg, (int)(290 * SCALE_FPS_MOMIJI), (int)(160 * SCALE_FPS_MOMIJI) )
{
	::nn_commonLog(1,  "EFFECT_MOMIJI START\n");

	this->m_pBall = effectMOMIJIArg->m_pBall;
 
	//�ړ������͌Œ�: ����≺���E����ƉE��≺��������
	this->shadowX = (int)this->m_pBall->x - 640;
	this->shadowY = (int)this->m_pBall->y - (int)this->m_pBall->z + 480;
	this->srcX = (int)this->m_pBall->x + 640;
	this->srcY = (int)this->m_pBall->y - (int)this->m_pBall->z - 480;

	this->shadowX2 = (int)this->m_pBall->x + 640;
	this->shadowY2 = (int)this->m_pBall->y - (int)this->m_pBall->z + 480;
	this->srcX2 = (int)this->m_pBall->x - 640;
	this->srcY2 = (int)this->m_pBall->y - (int)this->m_pBall->z - 480;

	this->shadowSpeed = effectMOMIJIArg->shadowSpeed;
	this->z = effectMOMIJIArg->z;
	this->angle = effectMOMIJIArg->angle;
	this->targetX = effectMOMIJIArg->targetX;
	this->targetY = effectMOMIJIArg->targetY;
	this->m_pBall = effectMOMIJIArg->m_pBall;

	this->m_vec3DStartPos.x = effectMOMIJIArg->m_vec3DStartPos.x;//������CBall�N���X��x, y, z���i�[����Ă���
	this->m_vec3DStartPos.y = effectMOMIJIArg->m_vec3DStartPos.y;
	this->m_vec3DStartPos.z = effectMOMIJIArg->m_vec3DStartPos.z;

	const int darkenFrame = (int)(160 * SCALE_FPS_MOMIJI);//��ʈÓ]���Ă���Ԃ̃t���[����

	this->dx = 1;
	this->dy = 1;
	this->dx2 = 1;
	this->dy2 = 1;

	this->ctrl->push_event( nn::yb::main::EFFECT_SPELL::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_SPELLNAME_SPELLNAME_3, 
		nn::yb::enum_effect::SLIDE_MODE_FRAME,
		darkenFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		(int)(20 * SCALE_FPS_MOMIJI),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		(int)(15 * SCALE_FPS_MOMIJI),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		(int)(darkenFrame - (20 + 15) * SCALE_FPS_MOMIJI),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y - 210, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 0,
		this->ctrl);//�X�y�����̕���(�w��t���[���I����ɏ���: �X�y���������ƕ\��)
	this->cnt++;

	this->ctrl->stopEffect();
	this->ctrl->unloadEffect();//unload 3D effect
	//loading 3D effect
	this->ctrl->loadEffect(L"graphics\\image\\bmp_main\\effect\\tornado_delete_test6.efk" );
 
	this->m_uiAnimeCounterLimit = 240;

	this->iStarCt = 0;

	this->m_fTornadoScale = 0.0f;

	
	//���X�v���C�g
	this->m_sprAya.m_vec3DPos = D3DXVECTOR3((float)camera_offset_x, camera_offset_y + 300.0f, nn::yb::e::global::DISP_Z_SPELL_BALL - 1);//�����ʒu�͉���
	this->m_sprAya.m_vec3DSize =  D3DXVECTOR3(104.0f, 104.0f, 0.0f);
	this->m_sprAya.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprAya.m_iAlpha = 31;//
	this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_1;//Obj ID
 
	//dummy effect(�����G�t�F�N�g)
	this->ctrl->push_event( nn::yb::main::EFFECT_MOMIJI::call_back, this, NULL);
	this->dummyEffect = nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_FLASH_0,
		camera_offset_x, camera_offset_y, 1,
		0, 0, 32, 10.0f, 32, 10.0f, 0, 0.0f, 0, 0.0f, DISP_COMMON_BLEND_MODE_ADD, NULL,
		(int)(290 * SCALE_FPS_MOMIJI));//�t���[������ݒ�
	this->cnt++;

	this->ctrl->m_effect3D_z = nn::yb::e::global::DISP_Z_SPELL_BALL - 1;//3D�G�t�F�N�g��Z�l���w��

}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MOMIJI::~EFFECT_MOMIJI() {

	::nn_commonLog(1,  "EFFECT_MOMIJI END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MOMIJI::task() {
	nn::yb::main::EFFECT_SPELL::task();

	//�X�L�b�v����
    ::KEY_WORK * key = &this->m_pBall-> master->keyWork;
	if (
		this->isLive( this->m_pBall ) &&
		key != NULL &&
		key->trigger & nn::common::e::PAD_WIN_BUTTON_A
		) 
	{
		//����{�^����������Ă�����A�����ړ���Ԃ܂ŃX�L�b�v
		if (
			this->m_eState >= 0 && 
			this->m_eState <= 6)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 9;
			this->m_uiAnimeCounter = 0;

			//�ʒu������
			this->m_sprAya.m_vec3DPos.x = this->m_vec3DPos.x - 160;
			this->m_sprAya.m_vec3DPos.y = this->m_vec3DPos.y - 120 - 32;
			this->shadowX = (int)this->m_vec3DPos.x - 160;
			this->shadowY = (int)this->m_vec3DPos.y - 120;
			this->shadowX2 = (int)this->m_vec3DPos.x - 640;//��ʊO
			this->shadowY2 = (int)this->m_vec3DPos.y - 480;

			//���̍��W���ݒ�
			this->m_sprAya.m_vec3DPos.x = this->m_vec3DStartPos.x - 180 ;
			this->m_sprAya.m_vec3DPos.y = this->m_vec3DStartPos.y - 120 - (float)this->m_pBall->z - 32;

			this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_6;//��U��グ�R


			this->ctrl->stopAllEffects();//��U��~

			//�J���������������ʒu�ֈړ�(�����͉�ʒ��S�ʒu�̂܂܌Œ�)
			////this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_vec3DStartPos.x - 180, (int)(this->m_vec3DStartPos.y - 120  - this->m_pBall->z), 10, NULL, NULL);

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x, (float)this->m_pBall->y - (float)this->m_pBall->z);//�G�t�F�N�g�̍��W�ݒ�
			//�J�������ړ�(�����͉�ʒ��S�ʒu�̂܂܌Œ�)
			this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_pBall->x, (int)(this->m_pBall->y - this->m_pBall->z));

			
			if (m_taskCutIn != NULL)//�J�b�g�C�����\������Ă���ꍇ�A�폜
			{ 
				::animationEnd( (ANIMATION *)m_taskCutIn->work );
				::animationHide( (ANIMATION *)m_taskCutIn->work );
				m_taskCutIn = NULL;
			}

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

			if(this->dummyEffect != NULL)
			{
				this->dummyEffect->taskDelete();
				this->dummyEffect = NULL;
			}

			this->iStarCt = 0;//�J�E���^�����Z�b�g
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

	this->m_uiAnimeCounter++;


	//Ball�̗�������
	if(this->m_pBall->z + this->m_pBall->getZSpeed() <= nn::yb::e::global::FLY_FALL_Z ) {
		//�G�t�F�N�g���A�����[�h���ďI��

		this->ctrl->stopAllEffects();
		this->ctrl->unloadEffect();//unload 3D effect

		this->m_eState = 12;

		//�Â��G�t�F�N�g�͏I�����邽�߁A�ϐ�������
		this->initialize();
		this->taskDelete();

		this->ctrl->clear_cb();// clear callback

		this->m_pBall->m_pCurrSpellEffect = NULL;
	}


	switch ( this->m_eState )
	{
	case 0://�J�b�g�C��
		{
			if (
				this->m_uiAnimeCounter >= 30 + 15 ///�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				////TASK * t = animationExec( NULL, ANIMATION_ID_SPELL_CHAR_3_2, 320, 240, 100 + 513, FALSE, NULL, NULL);
				m_taskCutIn = animationExec( NULL, ANIMATION_ID_SPELL_CHAR_3_2, 320, 240, 100 + 513, FALSE, nn::yb::main::EFFECT_SPELL::cutin_cb, this);
				animationChangeObjId( (ANIMATION *)m_taskCutIn->work, ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID, OBJ_ID_SPELL_CHAR_REIMU_0, OBJ_ID_SPELL_CHAR_AYA_0);
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;

	case 1://�J�b�g�C���I���҂�
		{
			if (
				this->m_uiAnimeCounter >= 75//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				//�e�ړ�����
				nn::common::graphics2d::initStart2GollDDA( this->srcX, this->srcY,
					this->shadowX, this->shadowY, this->vx, this->vy,
					this->dx, this->dy, this->diff
				);
				nn::common::graphics2d::initStart2GollDDA( this->srcX2, this->srcY2,
					this->shadowX2, this->shadowY2, this->vx2, this->vy2,
					this->dx2, this->dy2, this->diff
				);
				sound_nnsPlaySe( SE_LEVELUP);//�e�ړ� SE

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;

			}

		}
		break;

	case 2://�˖��ۈړ�1
		{

			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->shadowX, this->shadowY, this->srcX, this->srcY, this->vx, this->vy,
				this->dx, this->dy, this->diff, this->shadowSpeed
			);

			//���̍��W���ݒ�
			this->m_sprAya.m_vec3DPos.x = (float)this->shadowX;
			this->m_sprAya.m_vec3DPos.y = (float)this->shadowY - 32.0f;

			if(end_flag) {
				this->iStarCt = 0;//�J�E���^�����Z�b�g

				sound_nnsPlaySe( SE_LEVELUP);//�e�ړ� SE

				if (this->m_sprAya.m_iFileTexture == ::OBJ_ID_GAME_CHAR_AYA_0)
					this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_1;//���̉摜��؂�ւ�
				else
					this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_0;//���̉摜��؂�ւ�


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;

	case 3://�˖��ۈړ�2
		{
			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->shadowX2, this->shadowY2, this->srcX2, this->srcY2, this->vx2, this->vy2,
				this->dx2, this->dy2, this->diff, this->shadowSpeed
			);

			//���̍��W���ݒ�
			this->m_sprAya.m_vec3DPos.x = (float)this->shadowX2 ;
			this->m_sprAya.m_vec3DPos.y = (float)this->shadowY2 - 32.0f;

			if(end_flag) {
				this->iStarCt = 0;//�J�E���^�����Z�b�g

				double temp_angle = this->angle + 90.0;
				if(temp_angle > 360.0) {
					temp_angle -= 360.0;
				}
				int front_x = (int)nn::yb::common::yb_commonAngleSpeed2MoveX( temp_angle, 500.0);//�p�x���X�O�x�v���X���Ē���������
				int front_y = (int)nn::yb::common::yb_commonAngleSpeed2MoveY( temp_angle, 500.0);
				shadowX = (int)this->m_vec3DStartPos.x - this->m_pBall->holeCtrl->ctrl->drawGlobalW / 2; //�ړ��n�_
				shadowY =  (int)this->m_vec3DStartPos.y - this->m_pBall->holeCtrl->ctrl->drawGlobalH / 2;
				srcX = (int)this->m_vec3DStartPos.x - 195;//�ړ��I�_
				srcY = (int)this->m_vec3DStartPos.y - 120 - (int)this->m_pBall->z - 12;

				//�e�o�ꏀ��
				nn::common::graphics2d::initStart2GollDDA( this->srcX, this->srcY,
					this->shadowX, this->shadowY, this->vx, this->vy,
					this->dx, this->dy, this->diff
				);

				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_2;//�W�����v��

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;

	case 4://�˖��ہ{�e�o��
		{
			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->shadowX, this->shadowY, this->srcX, this->srcY, this->vx, this->vy,
				this->dx, this->dy, this->diff, 5
			);

			//���̍��W���ݒ�
			this->m_sprAya.m_vec3DPos.x = (float)this->shadowX ;
			this->m_sprAya.m_vec3DPos.y = (float)this->shadowY - 32.0f;


			if(end_flag) {
				this->iStarCt = 0;//�J�E���^�����Z�b�g

				//���̍��W���ݒ�
				this->m_sprAya.m_vec3DPos.x = this->m_vec3DStartPos.x - 180 ;
				this->m_sprAya.m_vec3DPos.y = this->m_vec3DStartPos.y - 120 - (float)this->m_pBall->z - 32;

				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_3;//���n

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;
	case 5://�����n��ꎞ�^��������
		{
			if (
				this->m_uiAnimeCounter >= 20//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->iStarCt = 0;//�J�E���^�����Z�b�g

				//�J���������������ʒu�ֈړ�(�����͉�ʒ��S�ʒu�̂܂܌Œ�)
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_vec3DStartPos.x - 180, (int)(this->m_vec3DStartPos.y - 120  - this->m_pBall->z), 10, NULL, NULL);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		break;
		}
	case 6://�J�������[�N���I���܂Ń^��������
		{
			if (
				this->m_uiAnimeCounter >= 20//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->iStarCt = 0;//�J�E���^�����Z�b�g

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		break;
		}
	case 7://��������
		{
			if (
				this->m_uiAnimeCounter >= 40//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				////nn::yb::main::CEffectCtrl::setScreenPos(this->m_vec3DPos.x, this->m_vec3DPos.y - this->m_pBall->z);//�G�t�F�N�g�̍��W�ݒ�
				nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x - 180, (float)this->m_pBall->y - 120.0f - (float)this->m_pBall->z);//�G�t�F�N�g�̍��W�ݒ�

				sound_nnsPlaySe( SE_MOMIJI);// SE
				this->ctrl->playEffect();//play 3D effect
				this->ctrl->setScale(0.5f, 0.5f, 0.5f);//�ŏ��͏k��������ԂŐ���

				//�z�[�~���O�p�ϐ���������
				this->m_vec3DPos = nn::yb::main::D3DXVECTOR3(this->m_vec3DStartPos.x - 180, this->m_vec3DStartPos.y - 120, this->m_vec3DStartPos.z);//�X�^�[�g���W
				m_vec3DTarget = nn::yb::main::D3DXVECTOR3(this->m_vec3DStartPos.x, this->m_vec3DStartPos.y, this->m_vec3DStartPos.z);//�S�[�����W
				m_vec3DSpeed = nn::yb::main::D3DXVECTOR3(15.0f, 5.0f, 0.0f);
				m_vec3DAccel = nn::yb::main::D3DXVECTOR3(0.8f, 0.8f, 0.0f);//�����x�ύX

				if (angle == 0)
					angle = 1;//0����h�~
				m_fTargetAngle = (float)PI / (float)angle;//�ڕW�ւ̊p�x

				m_fHomingTargetAngle = (float)PI / 18.0f;//�ڕW�ւ̊p�x��1��̃z�[�~���O�����ŕω������鎞�̊p�x

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			if (
				this->m_uiAnimeCounter >= 5 &&
				this->m_uiAnimeCounter <= 18
				)
			{
				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_4;//��U��グ�P
			}
			else if (
				this->m_uiAnimeCounter >= 19 &&
				this->m_uiAnimeCounter <= 26
				)
			{
				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_5;//��U��グ�Q
			}
			else if (
				this->m_uiAnimeCounter >= 27 &&
				this->m_uiAnimeCounter <= 40
				)
			{
				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_6;//��U��グ�R
			}
		}
		break;
	case 8://��������]���Ă������֋߂Â�
		{
			D3DXVECTOR3 vec3DSub;
			vec3DSub = this->m_vec3DPos;
			vec3DSub.Sub(m_vec3DTarget);
			float fLength = vec3DSub.Get2DLength();
			if (
				fLength < 20.0f || //�������q�b�g���邩�A
				this->m_uiAnimeCounter >= 75//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			
			float fAngle = (float)::atan2(
										(float)(m_vec3DTarget.y) -this->m_vec3DPos.y,
										 (float)(m_vec3DTarget.x)- this->m_vec3DPos.x); 
			m_fTargetAngle = (float)fmod(m_fTargetAngle + PI * 2, PI * 2); // 0�`2�΂̒l�Ɋۂ߂�
			fAngle=(float)fmod(fAngle + PI * 2 , PI * 2); // 0�`2�΂̒l�Ɋۂ߂�
			
			if ( fabs(fAngle-m_fTargetAngle) < m_fHomingTargetAngle )	//�p�x�̍���m_fHomingTargetAngle�ȓ��Ȃ�
			{
				fAngle = m_fTargetAngle;	//�p�x����v������
			}else{
				if (m_fTargetAngle < PI)
				{
					if (fAngle > m_fTargetAngle && fAngle < m_fTargetAngle + PI)	//�p�x���߂Â���Ȃ��
					{
						m_fTargetAngle+=m_fHomingTargetAngle;	//���v����
					}else{
						m_fTargetAngle-=m_fHomingTargetAngle;	//�����v�����
					}
				}else{
					if (fAngle < m_fTargetAngle && fAngle > m_fTargetAngle - PI)	//�p�x���߂Â���Ȃ��
					{
						m_fTargetAngle-=m_fHomingTargetAngle;	//�����v�����
					}else{
						m_fTargetAngle+=m_fHomingTargetAngle;	//���v����
					}
				}
			}

			this->m_vec3DPos.x += m_vec3DSpeed.x * ::cos( (float)m_fTargetAngle );
			this->m_vec3DPos.y += m_vec3DSpeed.y * ::sin( (float)m_fTargetAngle );
			if (m_vec3DSpeed.x < 15.0f && m_vec3DSpeed.y < 15.0f)//�ō����x����
				m_vec3DSpeed.Add(m_vec3DAccel);

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_vec3DPos.x, (float)this->m_vec3DPos.y - (float)this->m_pBall->z);//�G�t�F�N�g�̍��W�ݒ�

			//�J�������ړ�(�����͉�ʒ��S�ʒu�̂܂܌Œ�)
			this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_vec3DPos.x, (int)(this->m_vec3DPos.y - this->m_pBall->z));
			
			//�t���[�����o�߂���ɂ�A�ǔ����\���グ��
			m_fHomingTargetAngle = ::LunaAnimation::FlatSpeed((float)PI / 18.0f, (float)PI / 2.0f, (float)this->m_uiAnimeCounterLimit ,(float)this->m_uiAnimeCounter );
		}
		break;
	case 9://���̏�ŗ����g��
		{
			if (
				this->m_uiAnimeCounter >= 45//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->ctrl->stopAllEffects();//��U��~

				this->ctrl->playEffect();//play 3D effect
				this->ctrl->setScale(1.5f, 1.5f, 1.5f);//

				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_2;//���̉摜��؂�ւ�
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			const int iMoveFrame = 45;
			const float fMoveDistance = 200.0f;


			//�W�����v���̏ꍇ�AZ���W�����킹��
			if (0)
			{
				float fTornadoZ = 0.0f;
				this->ctrl->setLocation(0.0f, 0.0f, fTornadoZ);
			}

			//�����g��
			float fScale = LunaAnimation::Sub<float, int>(
				0.5f, 1.5f,
				iMoveFrame, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, fScale, fScale);


			//////����������]
			////double dRotate = LunaAnimation::Sub<double, int>(
			////	15.0, 45.0,
			////	iMoveFrame, this->m_uiAnimeCounter);//
			////this->m_pBall->add_angle(dRotate);
		}
		break;
	case 10://�˖��ۂ͉�ʊO�ֈړ�
		{
			if (
				this->m_uiAnimeCounter >= 30//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{

				//���̎��_�ŁABall�N���X�ɏ�����Ԃ�
				this->m_pBall->spellEffectEndMomiji();
				////this->m_pBall->spellEffectEndCb(this->m_pBall);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			const int iMoveFrame = 30;
			const float fMoveDistance = 200.0f;


			//���������̃T�C�Y�ɖ߂�(�ŏ���20�t���[���Œ�)
			float fScale = LunaAnimation::Sub<float, int>(
				1.5f, 0.8f,
				20, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, fScale, fScale);


			//////����������]
			////double dRotate = LunaAnimation::Sub<double, int>(
			////	15.0, 45.0,
			////	iMoveFrame, this->m_uiAnimeCounter);//
			////this->m_pBall->add_angle(dRotate);

			//�˖��ۂ͉�ʊO�ֈړ�
			float fCurrY = LunaAnimation::Sub2<float, int>(this->m_sprAya.m_vec3DPos.y, this->m_sprAya.m_vec3DPos.y - 340, iMoveFrame, this->m_uiAnimeCounter);//
			float fCurrX = LunaAnimation::Sub2<float, int>(this->m_sprAya.m_vec3DPos.x, this->m_sprAya.m_vec3DPos.x - 340, iMoveFrame, this->m_uiAnimeCounter);//

			this->m_sprAya.m_vec3DPos.y = fCurrY;
			this->m_sprAya.m_vec3DPos.x = fCurrX;
		}
		break;
	case 11://�����ړ�
		{
			if (
				this->m_uiAnimeCounter >= 360//������肪��~���掟�̏�Ԃ֑J�ڂ��邪�A�ő�t���[�����͐ݒ肵�Ă���
				|| ( ::abs( this->m_pBall->getSpeed() ) <= 0.1f
					&& ::abs( this->m_pBall->getZSpeed() ) <= 0.1f 
					/*&& this->m_pBall->state == nn::yb::e::ball::STATE_STANDBY */)
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			const int iMoveFrame = 360;
			/*const float fMoveDistance = 200.0f;

			//�L�[���͂ŗ������኱����\
			const float cfKeyMoveLength = nn::yb::e::global::SPELL_MOMIJI_CURVE;
			float fVecX = 0.0f;
			float fVecY = 0.0f;
			if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
				// ���L�[
				fVecX = cfKeyMoveLength * ::cos( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
				fVecY = cfKeyMoveLength * ::sin( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
				::nn_commonLog(1,  "LeftKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
				//�i�s�����ɑ΂��č����ɃJ�[�u

				//angle��ύX�H
				this->m_pBall->angle += nn::yb::e::global::SPELL_MOMIJI_CURVE;
			}
			else if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
				// �E�L�[
				fVecX = cfKeyMoveLength * ::cos( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
				fVecY = cfKeyMoveLength * ::sin( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
				::nn_commonLog(1,  "RightKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
				//�i�s�����ɑ΂��ĉE���ɃJ�[�u

				//angle��ύX�H
				this->m_pBall->angle -= nn::yb::e::global::SPELL_MOMIJI_CURVE;
			}*/

			//////�������̍��W��␳
			////this->m_pBall->x += fVecX;
			////this->m_pBall->y += fVecY;


			////float fCurrX = LunaAnimation::Sub2<float, int>(
			////	this->m_vec3DStartPos.x,
			////	this->m_vec3DStartPos.x + fMoveDistance * ::cos( ( ( (360 - this->angle) ) % 360) * PI / 180.0),
			////	iMoveFrame, this->m_uiAnimeCounter);//
			////float fCurrY = LunaAnimation::Sub2<float, int>(
			////	this->m_vec3DStartPos.y,
			////	this->m_vec3DStartPos.y + fMoveDistance * ::sin( ( ( (360 - this->angle) ) % 360) * PI / 180.0),
			////	iMoveFrame,
			////	this->m_uiAnimeCounter);//

			/*float fDiffX = fMoveDistance * ::cos( ( ( (360 - this->angle) ) % 360) * PI / 180.0) / iMoveFrame;
			float fDiffY = fMoveDistance * ::sin( ( ( (360 - this->angle) ) % 360) * PI / 180.0) / iMoveFrame;
			this->m_vec3DPos.x += fDiffX;//�i�s�����ֈړ�
			this->m_vec3DPos.y += fDiffY;

			//�L�[���͂��󂯕t����̂͌���Ԃ̃t���[�����̑O��2/3�̂݁A���t���[����������L�[���͂��󂯕t���Ȃ�
			if (this->m_uiAnimeCounter < iMoveFrame * 2 / 3.0)
			{
				this->m_vec3DPos.x += fVecX;//�L�[���͂ɂ�鍶�E�ւ̕␳
				this->m_vec3DPos.y += fVecY;
			}

			float fCurrX = this->m_vec3DPos.x ;
			float fCurrY = this->m_vec3DPos.y ;*/


			//�����k��(x, z������)
			float fScale = LunaAnimation::Sub2<float, int>(
				0.8f, 0.4f,
				iMoveFrame, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, 1.0f, fScale);
			this->m_fTornadoScale = fScale;//���݂̃X�P�[����ۑ�

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x, (float)this->m_pBall->y - (float)this->m_pBall->z);//�G�t�F�N�g�̍��W�ݒ�
			////nn::yb::main::CEffectCtrl::setScreenPos(this->m_pBall->holeCtrl->ctrl->camera->x, this->m_pBall->holeCtrl->ctrl->camera->y);//�G�t�F�N�g�̍��W�ݒ�


			//�J�������ړ�(�����͉�ʒ��S�ʒu�̂܂܌Œ�)
			////this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos(fCurrX, fCurrY - this->m_pBall->z);
			
			int z = nn::yb::e::global::DISP_Z_SPELL_BALL - 1;
			if((int)this->m_pBall->z < -this->m_pBall->r) {
				z = nn::yb::e::global::DISP_Z_FALL_BALL - 1;
			}
			this->ctrl->m_effect3D_z = z;//3D�G�t�F�N�g��Z�l���w��

		}
		break;
	case 12://��������
		{
			if (
				this->m_uiAnimeCounter >= 30//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->ctrl->stopEffect();//stop 3D effect

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			const int iMoveFrame = 30;
			//�����k��(x, z������)
			float fScale = LunaAnimation::Sub2<float, int>(
				this->m_fTornadoScale, 0.0f,
				iMoveFrame, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, 1.0f, fScale);

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x, (float)this->m_pBall->y - (float)this->m_pBall->z);//�G�t�F�N�g�̍��W�ݒ�


			int z = nn::yb::e::global::DISP_Z_SPELL_BALL - 1;
			if((int)this->m_pBall->z < -this->m_pBall->r) {
				z = nn::yb::e::global::DISP_Z_FALL_BALL - 1;
			}
			this->ctrl->m_effect3D_z = z;//3D�G�t�F�N�g��Z�l���w��
		}
		break;
	case 13://�I������
		{
			if (
				this->m_uiAnimeCounter >= 10//�w��t���[���o�߂����ꍇ�A���̏�Ԃ�
				)
			{
				this->ctrl->unloadEffect();//unload 3D effect

				////this->m_pBall->spellEffectEnd();
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

void nn::yb::main::EFFECT_MOMIJI::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

	const int ciWidthScale = 20;
	switch ( this->m_eState )
	{
	case 2://�e�ړ�
		{
			disp_commonObjCenterColor( ::OBJ_ID_MAIN_SHADOW_0, this->shadowX, this->shadowY, this->z, RGB_WHITE, 32);
		}
		break;
	case 3://�e�ړ�
		{
			disp_commonObjCenterColor( ::OBJ_ID_MAIN_SHADOW_0, this->shadowX2, this->shadowY2, this->z, RGB_WHITE, 32);
		}
		break;
	case 4://�˖���+�e�o��
		{
			disp_commonObjCenterColor( ::OBJ_ID_MAIN_SHADOW_0, this->shadowX, this->shadowY, this->z, RGB_WHITE, 32);
		}
		break;
	}

	this->m_sprAya.disp();
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MOMIJI::call_back( void * effectMOMIJIArg, void * dummy) {

	nn::yb::main::EFFECT_MOMIJI * effect = (nn::yb::main::EFFECT_MOMIJI *)effectMOMIJIArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

