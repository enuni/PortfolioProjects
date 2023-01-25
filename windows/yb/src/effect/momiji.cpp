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
const float	nn::yb::main::EFFECT_MOMIJI::SCALE_FPS_MOMIJI = (float)FPS_MOMIJI / 30.0f;	//FPS換算

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
 
	//移動方向は固定: 左やや下→右やや上と右やや下→左やや上
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

	this->m_vec3DStartPos.x = effectMOMIJIArg->m_vec3DStartPos.x;//初期はCBallクラスのx, y, zが格納されている
	this->m_vec3DStartPos.y = effectMOMIJIArg->m_vec3DStartPos.y;
	this->m_vec3DStartPos.z = effectMOMIJIArg->m_vec3DStartPos.z;

	const int darkenFrame = (int)(160 * SCALE_FPS_MOMIJI);//画面暗転している間のフレーム数

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
		this->ctrl);//スペル名称文字(指定フレーム終了後に消去: スペル中ずっと表示)
	this->cnt++;

	this->ctrl->stopEffect();
	this->ctrl->unloadEffect();//unload 3D effect
	//loading 3D effect
	this->ctrl->loadEffect(L"graphics\\image\\bmp_main\\effect\\tornado_delete_test6.efk" );
 
	this->m_uiAnimeCounterLimit = 240;

	this->iStarCt = 0;

	this->m_fTornadoScale = 0.0f;

	
	//文スプライト
	this->m_sprAya.m_vec3DPos = D3DXVECTOR3((float)camera_offset_x, camera_offset_y + 300.0f, nn::yb::e::global::DISP_Z_SPELL_BALL - 1);//初期位置は下部
	this->m_sprAya.m_vec3DSize =  D3DXVECTOR3(104.0f, 104.0f, 0.0f);
	this->m_sprAya.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprAya.m_iAlpha = 31;//
	this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_1;//Obj ID
 
	//dummy effect(透明エフェクト)
	this->ctrl->push_event( nn::yb::main::EFFECT_MOMIJI::call_back, this, NULL);
	this->dummyEffect = nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_FLASH_0,
		camera_offset_x, camera_offset_y, 1,
		0, 0, 32, 10.0f, 32, 10.0f, 0, 0.0f, 0, 0.0f, DISP_COMMON_BLEND_MODE_ADD, NULL,
		(int)(290 * SCALE_FPS_MOMIJI));//フレーム数を設定
	this->cnt++;

	this->ctrl->m_effect3D_z = nn::yb::e::global::DISP_Z_SPELL_BALL - 1;//3DエフェクトのZ値を指定

}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_MOMIJI::~EFFECT_MOMIJI() {

	::nn_commonLog(1,  "EFFECT_MOMIJI END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_MOMIJI::task() {
	nn::yb::main::EFFECT_SPELL::task();

	//スキップ処理
    ::KEY_WORK * key = &this->m_pBall-> master->keyWork;
	if (
		this->isLive( this->m_pBall ) &&
		key != NULL &&
		key->trigger & nn::common::e::PAD_WIN_BUTTON_A
		) 
	{
		//決定ボタンが押されていたら、竜巻移動状態までスキップ
		if (
			this->m_eState >= 0 && 
			this->m_eState <= 6)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 9;
			this->m_uiAnimeCounter = 0;

			//位置初期化
			this->m_sprAya.m_vec3DPos.x = this->m_vec3DPos.x - 160;
			this->m_sprAya.m_vec3DPos.y = this->m_vec3DPos.y - 120 - 32;
			this->shadowX = (int)this->m_vec3DPos.x - 160;
			this->shadowY = (int)this->m_vec3DPos.y - 120;
			this->shadowX2 = (int)this->m_vec3DPos.x - 640;//画面外
			this->shadowY2 = (int)this->m_vec3DPos.y - 480;

			//文の座標も設定
			this->m_sprAya.m_vec3DPos.x = this->m_vec3DStartPos.x - 180 ;
			this->m_sprAya.m_vec3DPos.y = this->m_vec3DStartPos.y - 120 - (float)this->m_pBall->z - 32;

			this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_6;//扇振り上げ３


			this->ctrl->stopAllEffects();//一旦停止

			//カメラも竜巻生成位置へ移動(竜巻は画面中心位置のまま固定)
			////this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_vec3DStartPos.x - 180, (int)(this->m_vec3DStartPos.y - 120  - this->m_pBall->z), 10, NULL, NULL);

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x, (float)this->m_pBall->y - (float)this->m_pBall->z);//エフェクトの座標設定
			//カメラも移動(竜巻は画面中心位置のまま固定)
			this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_pBall->x, (int)(this->m_pBall->y - this->m_pBall->z));

			
			if (m_taskCutIn != NULL)//カットインが表示されている場合、削除
			{ 
				::animationEnd( (ANIMATION *)m_taskCutIn->work );
				::animationHide( (ANIMATION *)m_taskCutIn->work );
				m_taskCutIn = NULL;
			}

			//暗転エフェクトの削除
			if (this->m_pBlackEffect != NULL )
			{
				//既に起動されているエフェクトがある場合タスクを消去
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

			this->iStarCt = 0;//カウンタをリセット
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


	//Ballの落下判定
	if(this->m_pBall->z + this->m_pBall->getZSpeed() <= nn::yb::e::global::FLY_FALL_Z ) {
		//エフェクトをアンロードして終了

		this->ctrl->stopAllEffects();
		this->ctrl->unloadEffect();//unload 3D effect

		this->m_eState = 12;

		//古いエフェクトは終了するため、変数初期化
		this->initialize();
		this->taskDelete();

		this->ctrl->clear_cb();// clear callback

		this->m_pBall->m_pCurrSpellEffect = NULL;
	}


	switch ( this->m_eState )
	{
	case 0://カットイン
		{
			if (
				this->m_uiAnimeCounter >= 30 + 15 ///指定フレーム経過した場合、次の状態へ
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

	case 1://カットイン終了待ち
		{
			if (
				this->m_uiAnimeCounter >= 75//指定フレーム経過した場合、次の状態へ
				)
			{
				//影移動準備
				nn::common::graphics2d::initStart2GollDDA( this->srcX, this->srcY,
					this->shadowX, this->shadowY, this->vx, this->vy,
					this->dx, this->dy, this->diff
				);
				nn::common::graphics2d::initStart2GollDDA( this->srcX2, this->srcY2,
					this->shadowX2, this->shadowY2, this->vx2, this->vy2,
					this->dx2, this->dy2, this->diff
				);
				sound_nnsPlaySe( SE_LEVELUP);//影移動 SE

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;

			}

		}
		break;

	case 2://射命丸移動1
		{

			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->shadowX, this->shadowY, this->srcX, this->srcY, this->vx, this->vy,
				this->dx, this->dy, this->diff, this->shadowSpeed
			);

			//文の座標も設定
			this->m_sprAya.m_vec3DPos.x = (float)this->shadowX;
			this->m_sprAya.m_vec3DPos.y = (float)this->shadowY - 32.0f;

			if(end_flag) {
				this->iStarCt = 0;//カウンタをリセット

				sound_nnsPlaySe( SE_LEVELUP);//影移動 SE

				if (this->m_sprAya.m_iFileTexture == ::OBJ_ID_GAME_CHAR_AYA_0)
					this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_1;//文の画像を切り替え
				else
					this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_0;//文の画像を切り替え


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;

	case 3://射命丸移動2
		{
			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->shadowX2, this->shadowY2, this->srcX2, this->srcY2, this->vx2, this->vy2,
				this->dx2, this->dy2, this->diff, this->shadowSpeed
			);

			//文の座標も設定
			this->m_sprAya.m_vec3DPos.x = (float)this->shadowX2 ;
			this->m_sprAya.m_vec3DPos.y = (float)this->shadowY2 - 32.0f;

			if(end_flag) {
				this->iStarCt = 0;//カウンタをリセット

				double temp_angle = this->angle + 90.0;
				if(temp_angle > 360.0) {
					temp_angle -= 360.0;
				}
				int front_x = (int)nn::yb::common::yb_commonAngleSpeed2MoveX( temp_angle, 500.0);//角度を９０度プラスして直交させる
				int front_y = (int)nn::yb::common::yb_commonAngleSpeed2MoveY( temp_angle, 500.0);
				shadowX = (int)this->m_vec3DStartPos.x - this->m_pBall->holeCtrl->ctrl->drawGlobalW / 2; //移動始点
				shadowY =  (int)this->m_vec3DStartPos.y - this->m_pBall->holeCtrl->ctrl->drawGlobalH / 2;
				srcX = (int)this->m_vec3DStartPos.x - 195;//移動終点
				srcY = (int)this->m_vec3DStartPos.y - 120 - (int)this->m_pBall->z - 12;

				//影登場準備
				nn::common::graphics2d::initStart2GollDDA( this->srcX, this->srcY,
					this->shadowX, this->shadowY, this->vx, this->vy,
					this->dx, this->dy, this->diff
				);

				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_2;//ジャンプ中

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;

	case 4://射命丸＋影登場
		{
			end_flag = nn::common::graphics2d::moveStart2GollDDA(
				this->shadowX, this->shadowY, this->srcX, this->srcY, this->vx, this->vy,
				this->dx, this->dy, this->diff, 5
			);

			//文の座標も設定
			this->m_sprAya.m_vec3DPos.x = (float)this->shadowX ;
			this->m_sprAya.m_vec3DPos.y = (float)this->shadowY - 32.0f;


			if(end_flag) {
				this->iStarCt = 0;//カウンタをリセット

				//文の座標も設定
				this->m_sprAya.m_vec3DPos.x = this->m_vec3DStartPos.x - 180 ;
				this->m_sprAya.m_vec3DPos.y = this->m_vec3DStartPos.y - 120 - (float)this->m_pBall->z - 32;

				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_3;//着地

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;
	case 5://文着地後一時タメを入れる
		{
			if (
				this->m_uiAnimeCounter >= 20//指定フレーム経過した場合、次の状態へ
				)
			{
				this->iStarCt = 0;//カウンタをリセット

				//カメラも竜巻生成位置へ移動(竜巻は画面中心位置のまま固定)
				this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_vec3DStartPos.x - 180, (int)(this->m_vec3DStartPos.y - 120  - this->m_pBall->z), 10, NULL, NULL);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		break;
		}
	case 6://カメラワークが終わるまでタメを入れる
		{
			if (
				this->m_uiAnimeCounter >= 20//指定フレーム経過した場合、次の状態へ
				)
			{
				this->iStarCt = 0;//カウンタをリセット

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		break;
		}
	case 7://竜巻生成
		{
			if (
				this->m_uiAnimeCounter >= 40//指定フレーム経過した場合、次の状態へ
				)
			{
				////nn::yb::main::CEffectCtrl::setScreenPos(this->m_vec3DPos.x, this->m_vec3DPos.y - this->m_pBall->z);//エフェクトの座標設定
				nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x - 180, (float)this->m_pBall->y - 120.0f - (float)this->m_pBall->z);//エフェクトの座標設定

				sound_nnsPlaySe( SE_MOMIJI);// SE
				this->ctrl->playEffect();//play 3D effect
				this->ctrl->setScale(0.5f, 0.5f, 0.5f);//最初は縮小した状態で生成

				//ホーミング用変数を初期化
				this->m_vec3DPos = nn::yb::main::D3DXVECTOR3(this->m_vec3DStartPos.x - 180, this->m_vec3DStartPos.y - 120, this->m_vec3DStartPos.z);//スタート座標
				m_vec3DTarget = nn::yb::main::D3DXVECTOR3(this->m_vec3DStartPos.x, this->m_vec3DStartPos.y, this->m_vec3DStartPos.z);//ゴール座標
				m_vec3DSpeed = nn::yb::main::D3DXVECTOR3(15.0f, 5.0f, 0.0f);
				m_vec3DAccel = nn::yb::main::D3DXVECTOR3(0.8f, 0.8f, 0.0f);//加速度変更

				if (angle == 0)
					angle = 1;//0割り防止
				m_fTargetAngle = (float)PI / (float)angle;//目標への角度

				m_fHomingTargetAngle = (float)PI / 18.0f;//目標への角度を1回のホーミング処理で変化させる時の角度

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
				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_4;//扇振り上げ１
			}
			else if (
				this->m_uiAnimeCounter >= 19 &&
				this->m_uiAnimeCounter <= 26
				)
			{
				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_5;//扇振り上げ２
			}
			else if (
				this->m_uiAnimeCounter >= 27 &&
				this->m_uiAnimeCounter <= 40
				)
			{
				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_6;//扇振り上げ３
			}
		}
		break;
	case 8://竜巻が回転してゆっくりへ近づく
		{
			D3DXVECTOR3 vec3DSub;
			vec3DSub = this->m_vec3DPos;
			vec3DSub.Sub(m_vec3DTarget);
			float fLength = vec3DSub.Get2DLength();
			if (
				fLength < 20.0f || //竜巻がヒットするか、
				this->m_uiAnimeCounter >= 75//指定フレーム経過した場合、次の状態へ
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
			m_fTargetAngle = (float)fmod(m_fTargetAngle + PI * 2, PI * 2); // 0～2πの値に丸める
			fAngle=(float)fmod(fAngle + PI * 2 , PI * 2); // 0～2πの値に丸める
			
			if ( fabs(fAngle-m_fTargetAngle) < m_fHomingTargetAngle )	//角度の差がm_fHomingTargetAngle以内なら
			{
				fAngle = m_fTargetAngle;	//角度を一致させる
			}else{
				if (m_fTargetAngle < PI)
				{
					if (fAngle > m_fTargetAngle && fAngle < m_fTargetAngle + PI)	//角度を近づけるならば
					{
						m_fTargetAngle+=m_fHomingTargetAngle;	//時計回りに
					}else{
						m_fTargetAngle-=m_fHomingTargetAngle;	//反時計周りに
					}
				}else{
					if (fAngle < m_fTargetAngle && fAngle > m_fTargetAngle - PI)	//角度を近づけるならば
					{
						m_fTargetAngle-=m_fHomingTargetAngle;	//反時計周りに
					}else{
						m_fTargetAngle+=m_fHomingTargetAngle;	//時計回りに
					}
				}
			}

			this->m_vec3DPos.x += m_vec3DSpeed.x * ::cos( (float)m_fTargetAngle );
			this->m_vec3DPos.y += m_vec3DSpeed.y * ::sin( (float)m_fTargetAngle );
			if (m_vec3DSpeed.x < 15.0f && m_vec3DSpeed.y < 15.0f)//最高速度制限
				m_vec3DSpeed.Add(m_vec3DAccel);

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_vec3DPos.x, (float)this->m_vec3DPos.y - (float)this->m_pBall->z);//エフェクトの座標設定

			//カメラも移動(竜巻は画面中心位置のまま固定)
			this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos((int)this->m_vec3DPos.x, (int)(this->m_vec3DPos.y - this->m_pBall->z));
			
			//フレームが経過するにつれ、追尾性能を上げる
			m_fHomingTargetAngle = ::LunaAnimation::FlatSpeed((float)PI / 18.0f, (float)PI / 2.0f, (float)this->m_uiAnimeCounterLimit ,(float)this->m_uiAnimeCounter );
		}
		break;
	case 9://その場で竜巻拡大
		{
			if (
				this->m_uiAnimeCounter >= 45//指定フレーム経過した場合、次の状態へ
				)
			{
				this->ctrl->stopAllEffects();//一旦停止

				this->ctrl->playEffect();//play 3D effect
				this->ctrl->setScale(1.5f, 1.5f, 1.5f);//

				this->m_sprAya.m_iFileTexture = ::OBJ_ID_GAME_CHAR_AYA_2;//文の画像を切り替え
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			const int iMoveFrame = 45;
			const float fMoveDistance = 200.0f;


			//ジャンプ中の場合、Z座標を合わせる
			if (0)
			{
				float fTornadoZ = 0.0f;
				this->ctrl->setLocation(0.0f, 0.0f, fTornadoZ);
			}

			//竜巻拡大
			float fScale = LunaAnimation::Sub<float, int>(
				0.5f, 1.5f,
				iMoveFrame, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, fScale, fScale);


			//////ゆっくりも回転
			////double dRotate = LunaAnimation::Sub<double, int>(
			////	15.0, 45.0,
			////	iMoveFrame, this->m_uiAnimeCounter);//
			////this->m_pBall->add_angle(dRotate);
		}
		break;
	case 10://射命丸は画面外へ移動
		{
			if (
				this->m_uiAnimeCounter >= 30//指定フレーム経過した場合、次の状態へ
				)
			{

				//この時点で、Ballクラスに処理を返す
				this->m_pBall->spellEffectEndMomiji();
				////this->m_pBall->spellEffectEndCb(this->m_pBall);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			const int iMoveFrame = 30;
			const float fMoveDistance = 200.0f;


			//竜巻が元のサイズに戻る(最初の20フレーム固定)
			float fScale = LunaAnimation::Sub<float, int>(
				1.5f, 0.8f,
				20, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, fScale, fScale);


			//////ゆっくりも回転
			////double dRotate = LunaAnimation::Sub<double, int>(
			////	15.0, 45.0,
			////	iMoveFrame, this->m_uiAnimeCounter);//
			////this->m_pBall->add_angle(dRotate);

			//射命丸は画面外へ移動
			float fCurrY = LunaAnimation::Sub2<float, int>(this->m_sprAya.m_vec3DPos.y, this->m_sprAya.m_vec3DPos.y - 340, iMoveFrame, this->m_uiAnimeCounter);//
			float fCurrX = LunaAnimation::Sub2<float, int>(this->m_sprAya.m_vec3DPos.x, this->m_sprAya.m_vec3DPos.x - 340, iMoveFrame, this->m_uiAnimeCounter);//

			this->m_sprAya.m_vec3DPos.y = fCurrY;
			this->m_sprAya.m_vec3DPos.x = fCurrX;
		}
		break;
	case 11://竜巻移動
		{
			if (
				this->m_uiAnimeCounter >= 360//ゆっくりが停止次第次の状態へ遷移するが、最大フレーム数は設定しておく
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

			//キー入力で竜巻を若干操作可能
			const float cfKeyMoveLength = nn::yb::e::global::SPELL_MOMIJI_CURVE;
			float fVecX = 0.0f;
			float fVecY = 0.0f;
			if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
				// 左キー
				fVecX = cfKeyMoveLength * ::cos( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
				fVecY = cfKeyMoveLength * ::sin( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
				::nn_commonLog(1,  "LeftKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
				//進行方向に対して左側にカーブ

				//angleを変更？
				this->m_pBall->angle += nn::yb::e::global::SPELL_MOMIJI_CURVE;
			}
			else if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
				// 右キー
				fVecX = cfKeyMoveLength * ::cos( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
				fVecY = cfKeyMoveLength * ::sin( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
				::nn_commonLog(1,  "RightKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
				//進行方向に対して右側にカーブ

				//angleを変更？
				this->m_pBall->angle -= nn::yb::e::global::SPELL_MOMIJI_CURVE;
			}*/

			//////ゆっくりの座標を補正
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
			this->m_vec3DPos.x += fDiffX;//進行方向へ移動
			this->m_vec3DPos.y += fDiffY;

			//キー入力を受け付けるのは現状態のフレーム数の前半2/3のみ、一定フレームたったらキー入力を受け付けない
			if (this->m_uiAnimeCounter < iMoveFrame * 2 / 3.0)
			{
				this->m_vec3DPos.x += fVecX;//キー入力による左右への補正
				this->m_vec3DPos.y += fVecY;
			}

			float fCurrX = this->m_vec3DPos.x ;
			float fCurrY = this->m_vec3DPos.y ;*/


			//竜巻縮小(x, z軸方向)
			float fScale = LunaAnimation::Sub2<float, int>(
				0.8f, 0.4f,
				iMoveFrame, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, 1.0f, fScale);
			this->m_fTornadoScale = fScale;//現在のスケールを保存

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x, (float)this->m_pBall->y - (float)this->m_pBall->z);//エフェクトの座標設定
			////nn::yb::main::CEffectCtrl::setScreenPos(this->m_pBall->holeCtrl->ctrl->camera->x, this->m_pBall->holeCtrl->ctrl->camera->y);//エフェクトの座標設定


			//カメラも移動(竜巻は画面中心位置のまま固定)
			////this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos(fCurrX, fCurrY - this->m_pBall->z);
			
			int z = nn::yb::e::global::DISP_Z_SPELL_BALL - 1;
			if((int)this->m_pBall->z < -this->m_pBall->r) {
				z = nn::yb::e::global::DISP_Z_FALL_BALL - 1;
			}
			this->ctrl->m_effect3D_z = z;//3DエフェクトのZ値を指定

		}
		break;
	case 12://消去処理
		{
			if (
				this->m_uiAnimeCounter >= 30//指定フレーム経過した場合、次の状態へ
				)
			{
				this->ctrl->stopEffect();//stop 3D effect

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}

			const int iMoveFrame = 30;
			//竜巻縮小(x, z軸方向)
			float fScale = LunaAnimation::Sub2<float, int>(
				this->m_fTornadoScale, 0.0f,
				iMoveFrame, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, 1.0f, fScale);

			nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x, (float)this->m_pBall->y - (float)this->m_pBall->z);//エフェクトの座標設定


			int z = nn::yb::e::global::DISP_Z_SPELL_BALL - 1;
			if((int)this->m_pBall->z < -this->m_pBall->r) {
				z = nn::yb::e::global::DISP_Z_FALL_BALL - 1;
			}
			this->ctrl->m_effect3D_z = z;//3DエフェクトのZ値を指定
		}
		break;
	case 13://終了処理
		{
			if (
				this->m_uiAnimeCounter >= 10//指定フレーム経過した場合、次の状態へ
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
	case 2://影移動
		{
			disp_commonObjCenterColor( ::OBJ_ID_MAIN_SHADOW_0, this->shadowX, this->shadowY, this->z, RGB_WHITE, 32);
		}
		break;
	case 3://影移動
		{
			disp_commonObjCenterColor( ::OBJ_ID_MAIN_SHADOW_0, this->shadowX2, this->shadowY2, this->z, RGB_WHITE, 32);
		}
		break;
	case 4://射命丸+影登場
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

