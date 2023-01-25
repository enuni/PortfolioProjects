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

#include "nitori.h"
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

const int nn::yb::main::EFFECT_NITORI::FPS_NITORI = 60;
const float	nn::yb::main::EFFECT_NITORI::SCALE_FPS_NITORI = (float)FPS_NITORI / 30.0f;	//FPS換算

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_NITORI::EFFECT_NITORI()
: nn::yb::main::EFFECT_SPELL() {

	::nn_commonLog(1,  "EFFECT_NITORI CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_NITORI));

	this->iStarCt = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_NITORI::EFFECT_NITORI( TASK_SAVE * parent, nn::yb::main::EFFECT_NITORI * effectNITORIArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectNITORIArg, (int)(280 * SCALE_FPS_NITORI), (int)(280 * SCALE_FPS_NITORI))
{
	::nn_commonLog(1,  "EFFECT_NITORI START\n");


	this->shadowX = effectNITORIArg->shadowX;
	this->shadowY = effectNITORIArg->shadowY;
	this->srcX = effectNITORIArg->srcX;
	this->srcY = effectNITORIArg->srcY;
	this->shadowSpeed = effectNITORIArg->shadowSpeed;
	this->z = effectNITORIArg->z;
	this->angle = effectNITORIArg->angle;
	this->m_pBall = effectNITORIArg->m_pBall;

	this->m_vec3DStartPos.x = effectNITORIArg->m_vec3DStartPos.x;
	this->m_vec3DStartPos.y = effectNITORIArg->m_vec3DStartPos.y;
	this->m_vec3DStartPos.z = effectNITORIArg->m_vec3DStartPos.z;

	this->ctrl->push_event( nn::yb::main::EFFECT_NITORI::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_SPELLNAME_SPELLNAME_7, 
		nn::yb::enum_effect::SLIDE_MODE_FRAME,
		this->totalFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		(int)(20 * SCALE_FPS_NITORI),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		(int)(15 * SCALE_FPS_NITORI),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		(int)((this->totalFrame - (20 + 15)) * SCALE_FPS_NITORI),
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
	////this->ctrl->loadEffect(L"graphics\\image\\bmp_main\\effect\\Nitori_Flash_Left.efk" );//火花エフェクト
	this->ctrl->loadEffect(L"graphics\\image\\bmp_main\\effect\\Nitori_Flash2.efk" );//火花エフェクト

	this->iStarCt = 0;

	//にとりスプライト
	this->m_sprNitori.m_vec3DPos = D3DXVECTOR3(camera_offset_x + 400.0f, (float)camera_offset_y - 32.0f, (float)(nn::yb::e::global::DISP_Z_SPELL_BALL - 1));//初期位置は右部, ゆっくりより手前
	this->m_sprNitori.m_vec3DSize =  D3DXVECTOR3(104.0f, 104.0f, 0.0f);
	this->m_sprNitori.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprNitori.m_iAlpha = 31;//
	this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_0;//Obj ID
	
	this->ctrl->m_effect3D_z = static_cast<int>(this->m_sprNitori.m_vec3DPos.z - 1);//3DエフェクトのZ値を指定
}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_NITORI::~EFFECT_NITORI() {

	::nn_commonLog(1,  "EFFECT_NITORI END\n");

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_NITORI::task() {
	nn::yb::main::EFFECT_SPELL::task();

	//スキップ処理
    ::KEY_WORK * key = &this->m_pBall-> master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		//決定ボタンが押されていたら、アームが伸びきった状態までスキップ
		if (this->m_eState >= 0 
			&& this->m_eState <= 5)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_eState = 6;
			this->m_uiAnimeCounter = 0;
			this->iStarCt = 0;//カウンタをリセット

			this->ctrl->stopAllEffects();
			this->ctrl->unloadEffect();

			//位置の初期化
			this->m_sprNitori.m_vec3DPos.x = this->m_vec3DStartPos.x;

			this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_6;//にとりスプライトを切り替え

			this->m_pBall->ballForm = nn::yb::e::ball::BALL_FORM_MECHA;//ゆっくりをメカゆっくり状態に


			if (this->m_taskCutIn != NULL)//カットインが表示されている場合、停止
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

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{
	case 0://カットイン
		{
			if (
				this->m_uiAnimeCounter >= 30 + 15 ///指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_taskCutIn = animationExec( NULL, ANIMATION_ID_SPELL_CHAR_3, 320, 240, 100 + 513, FALSE, nn::yb::main::EFFECT_SPELL::cutin_cb, this);
				animationChangeObjId( (ANIMATION *)this->m_taskCutIn->work, ANIMATION_CHANGE_OBJ_ID_TYPE_OBJ_ID, OBJ_ID_SPELL_CHAR_REIMU_0, OBJ_ID_SPELL_CHAR_NITORI_0);

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
				sound_nnsPlaySe( SE_LEVELUP);//移動 SE
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;
			}
		}
		break;


	case 2://にとりが右側から歩いてくる
		{
			if (
				this->m_uiAnimeCounter >= 120//指定フレーム経過した場合、次の状態へ
				)
			{
				//にとりスプライトを切り替え
				this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_5;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			
			const int iFrame = 120;

			//キャラ移動
			float fPosX = 0.0;
			fPosX = LunaAnimation::Sub<float, int>(this->m_vec3DStartPos.x + 400.0f, this->m_vec3DStartPos.x, iFrame, this->m_uiAnimeCounter);
			this->m_sprNitori.m_vec3DPos.x = fPosX;

			//歩行アニメ
			const int c_animeID[8] = {0, 1, 2, 3, 4, 3, 2, 1};//ID
			const int c_animeFrame = sizeof c_animeID /sizeof c_animeID[0];

            int iAnimeCt = this->m_uiAnimeCounter % (c_animeFrame * 8) / 8;
			int iSpr = OBJ_ID_GAME_CHAR_NITORI_0 + c_animeID[iAnimeCt];
			this->m_sprNitori.m_iFileTexture = iSpr;
		}
		break;
	case 3://にとりが後ろを向く
		{
			if (
				this->m_uiAnimeCounter >= 15//指定フレーム経過した場合、次の状態へ
				)
			{
				//にとりスプライトを切り替え
				this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_6;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			
			const int iFrame = 15;
			
			if (this->m_uiAnimeCounter >= 0 && this->m_uiAnimeCounter < 7)
			{
				this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_6;
			}
			else if (this->m_uiAnimeCounter >= 8 && this->m_uiAnimeCounter < 15)
			{
				this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_7;
			}
		}
		break;
	case 4://改造エフェクト
		{
			if (
				this->m_uiAnimeCounter >= 180//指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_6;//にとりスプライトを切り替え

				this->m_pBall->ballForm = nn::yb::e::ball::BALL_FORM_MECHA;//ゆっくりをメカゆっくり状態に

				sound_nnsPlaySe( SE_GROUND);//着弾SE


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			
			const int iFrame = 180;

			//改造アニメ
			int iSpr = OBJ_ID_GAME_CHAR_NITORI_8 + this->m_uiAnimeCounter % (2 * 10) / 10;
			this->m_sprNitori.m_iFileTexture = iSpr;

			//指定フレームに、エフェクトを表示
			if (
				this->m_uiAnimeCounter == 1
				)
			{
				sound_nnsPlaySe( SE_HIBANA1 );// SE
				//火花(右側)
				this->ctrl->playEffect();//play 3D effect
				this->ctrl->setScale(1.1f, 1.1f, 1.1f);
				////this->ctrl->setRotation(0.0f, 0.0f, 90.0f * (float)PI / 180.0f);
				nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x + 24, (float)(this->m_pBall->y - this->m_pBall->z - 32));//エフェクトの座標設定

			}
			else if (
				this->m_uiAnimeCounter == 90
				)
			{
				sound_nnsPlaySe( SE_HIBANA2 );// SE
				//火花(左側)
				this->ctrl->playEffect();//play 3D effect
				//this->ctrl->setRotation(0.0f, 0.0f, 90.0f * (float)PI / 180.0f);
				nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x - 24, (float)(this->m_pBall->y - this->m_pBall->z - 32));//エフェクトの座標設定
			}
			
			if (this->m_uiAnimeCounter == 1)
			{
				//中央やや右側から閃光
				//this->ctrl->push_event( nn::yb::main::EFFECT_NITORI::call_back, this, NULL);
				nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_FLASH_0, camera_offset_x + 10, camera_offset_y, 1, 0, 0, 32, 70.0f, 32, 70.0f, 0, 0.0f, 31, -0.1f, DISP_COMMON_BLEND_MODE_ADD, NULL, 20);//
				//this->cnt++;
			}
			else if (this->m_uiAnimeCounter == 90)
			{
				//中央やや左側から閃光2
				this->ctrl->push_event( nn::yb::main::EFFECT_NITORI::call_back, this, NULL);
				nn::yb::main::effect_out_ex(parent, OBJ_ID_DUN_SPELL_SONIC_0, camera_offset_x - 10, camera_offset_y, 1, 0, 0, 32, 80.0f, 32, 80.0f, 0, 0.0f, 31, -0.1f, DISP_COMMON_BLEND_MODE_ADD, this->ctrl, 15);//
				this->cnt++;
			}

			if (
				this->m_uiAnimeCounter == 1
				|| this->m_uiAnimeCounter == 10
				|| this->m_uiAnimeCounter == 20
				
				|| this->m_uiAnimeCounter == 70
				|| this->m_uiAnimeCounter == 80
				|| this->m_uiAnimeCounter == 90
				)
			{
				this->ctrl->push_event( nn::yb::main::EFFECT_NITORI::call_back, this, NULL);
				nn::yb::main::effect_quake(this, nn::yb::enum_effect::EFFECT_ANIME_SUB, 10, 15, 20, this->ctrl);//画面振動
				this->cnt++;
			}
			else if (
				   this->m_uiAnimeCounter == 60
				|| this->m_uiAnimeCounter == 120
				|| this->m_uiAnimeCounter == 160
				)
			{
				this->ctrl->push_event( nn::yb::main::EFFECT_NITORI::call_back, this, NULL);
				nn::yb::main::effect_quake(this, nn::yb::enum_effect::EFFECT_ANIME_SUB, 20, 25, 30, this->ctrl);//大きく画面振動
				this->cnt++;
			}

			//毎フレーム必要な更新処理
			if (
				this->m_uiAnimeCounter >= 1 && this->m_uiAnimeCounter < 90
				)
			{
				this->ctrl->setScale(0.98f, 0.98f, 0.98f);
				this->ctrl->setRotation(0.0f, 0.0f, 90.0f * (float)PI / 180.0f);
			}
			else if (this->m_uiAnimeCounter >= 90)
			{
				this->ctrl->setScale(0.97f, 0.97f, 0.97f);
				this->ctrl->setRotation(0.0f, 0.0f, 270.0f * (float)PI / 180.0f);

			}

			//アニメーション一時停止
			if (
				this->m_uiAnimeCounter >= 1 && this->m_uiAnimeCounter < 30
				)
			{
				this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_8;
			}
			else if (
				this->m_uiAnimeCounter >= 90 && this->m_uiAnimeCounter < 120
				)
			{
				this->m_sprNitori.m_iFileTexture = OBJ_ID_GAME_CHAR_NITORI_9;
			}
			
		}
		break;
	case 5://のび～るアームが画面外に伸びる
		{
			if (
				this->m_uiAnimeCounter >= 60//指定フレーム経過した場合、次の状態へ
				)
			{

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			const int iMoveFrame = 60;

		}
		break;
	case 6://にとりが画面外へ移動処理
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
			const int iFrame = 30;
			//フェードアウト
			int iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Sub2<float, int>(31, 0, iFrame, this->m_uiAnimeCounter));
		

			//にとり画面外へ
			float fCurrY = (float)LunaAnimation::Sub2<double, int>(this->m_pBall->y - this->m_pBall->z, this->m_pBall->y - this->m_pBall->z - 340, iFrame, this->m_uiAnimeCounter);//

			this->m_sprNitori.m_vec3DPos.y = fCurrY;

		}
		break;
	case 7://移動処理
		{
			if (
				this->m_uiAnimeCounter >= 10//指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			const int iFrame = 90;

			//十字キーでゆっくり移動

			//////キー入力で若干操作可能
			////const float cfKeyMoveLength = 10.0f;
			////float fVecX = 0.0f;
			////float fVecY = 0.0f;
			////if(key->trigger & nn::common::e::PAD_WIN_KEY_LEFT) {
			////	// 左キー
			////	fVecX = cfKeyMoveLength * (float)::cos( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
			////	fVecY = cfKeyMoveLength * (float)::sin( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
			////	::nn_commonLog(1,  "LeftKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
			////	//進行方向に対して左側にカーブ
			////}
			////else if(key->trigger & nn::common::e::PAD_WIN_KEY_RIGHT) {
			////	// 右キー
			////	fVecX = cfKeyMoveLength * (float)::cos( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
			////	fVecY = cfKeyMoveLength * (float)::sin( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
			////	::nn_commonLog(1,  "RightKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
			////	//進行方向に対して右側にカーブ
			////}
		}
		break;
	case 8://終了処理
		{
			if (
				this->m_uiAnimeCounter >= 10//指定フレーム経過した場合、次の状態へ
				)
			{

				this->ctrl->unloadEffect();//unload 3D effect


				nn::yb::main::EFFECT_NITORI::call_back( this, 0);
				
				this->m_pBall->spellEffectEndNitori();
				this->initialize();
				this->ctrl->clear_cb();// clear callback
				this->taskDelete();// 
				this->m_pBall->m_pCurrSpellEffect = NULL;

				this->m_taskCutIn = NULL;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				nn::yb::main::EFFECT_NITORI::call_back( this, 0);
				break;
			}

		}
		break;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_NITORI::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

	this->m_sprNitori.disp();

	const int ciWidthScale = 20;
	switch ( this->m_eState )
	{
	case 2://
		{
		}
		break;
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_NITORI::call_back( void * effectNITORIArg, void * dummy) {

	nn::yb::main::EFFECT_NITORI * effect = (nn::yb::main::EFFECT_NITORI *)effectNITORIArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

