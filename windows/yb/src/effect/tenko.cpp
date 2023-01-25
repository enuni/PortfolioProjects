#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nn_common.h"

#include "disp_common.h"
#include "sin_cos.h"
#include "anime.h"

#include "obj_main_3d_data.h"

#include "effect\\effect_ctrl.h"
#include "effect\\effect_spell.h"

#include "common\\LunaAnimation.h"

#include "tenko.h"
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
#include "animation_data.h"
#include "enum.h"

const int nn::yb::main::EFFECT_TENKO::FPS_TENKO = 60;
const float	nn::yb::main::EFFECT_TENKO::SCALE_FPS_TENKO = (float)FPS_TENKO / 30.0f;	//FPS換算

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_TENKO::EFFECT_TENKO()
: nn::yb::main::EFFECT_SPELL() {

	::nn_commonLog(1,  "EFFECT_TENKO CREATE\n");

	memset( this, 0, sizeof( nn::yb::main::EFFECT_TENKO));

	this->iStarCt = 0;
}

//---------------------------------------------------------------------------

nn::yb::main::EFFECT_TENKO::EFFECT_TENKO( TASK_SAVE * parent, nn::yb::main::EFFECT_TENKO * effectTENKOArg) 
: nn::yb::main::EFFECT_SPELL( parent, effectTENKOArg, (int)(230 * SCALE_FPS_TENKO), (int)(230 * SCALE_FPS_TENKO))
{
	::nn_commonLog(1,  "EFFECT_TENKO START\n");

	this->m_pBall = effectTENKOArg->m_pBall;

	this->shadowX = effectTENKOArg->shadowX;
	this->shadowY = effectTENKOArg->shadowY;
	this->srcX = effectTENKOArg->srcX;
	this->srcY = effectTENKOArg->srcY;
	this->shadowSpeed = effectTENKOArg->shadowSpeed;
	this->z = effectTENKOArg->z;
	this->angle = effectTENKOArg->angle;
	this->targetX = effectTENKOArg->targetX;
	this->targetY = effectTENKOArg->targetY;
	this->m_pBall = effectTENKOArg->m_pBall;

	this->m_vec3DStartPos.x = effectTENKOArg->m_vec3DStartPos.x;
	this->m_vec3DStartPos.y = effectTENKOArg->m_vec3DStartPos.y;
	this->m_vec3DStartPos.z = effectTENKOArg->m_vec3DStartPos.z;

	int darkenFrame = static_cast<int>(230 * SCALE_FPS_TENKO);

	this->ctrl->push_event( nn::yb::main::EFFECT_TENKO::call_back, this, NULL);
	nn::yb::main::effect_fadein_out(this, OBJ_ID_SPELLNAME_SPELLNAME_4, 
		nn::yb::enum_effect::SLIDE_MODE_FRAME,
		darkenFrame, 160, DISP_COMMON_BLEND_MODE_ALPHA , nn::yb::enum_effect::EFFECT_ANIME_ADD , 
		(int)(20 * SCALE_FPS_TENKO),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y + 140,
		1024, 256,
		128, 32,
		0, 0,
		0, 31,
		(int)(15 * SCALE_FPS_TENKO),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y + 140, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 31,
		(int)((darkenFrame - (20 + 15)) * SCALE_FPS_TENKO),
		camera_offset_x + 200, camera_offset_x + 200,
		camera_offset_y - 210, camera_offset_y - 210,
		256, 256,
		32, 32,
		0, 0,
		31, 0,
		this->ctrl);//スペル名称文字(指定フレーム終了後に消去: スペル中ずっと表示)
	this->cnt++;

	this->ctrl->stopEffect();
	//loading 3D effect
	this->ctrl->loadEffect(L"graphics\\image\\bmp_main\\effect\\SonicBoom_Ground.efk" );

	this->iStarCt = 0;
	
	//要石スプライト(上半分): ゆっくりより手前側のレイヤー
	this->m_sprKaname.m_vec3DPos = D3DXVECTOR3((float)camera_offset_x, camera_offset_y - 400.0f, 170.0f);//初期位置は上部
	OBJ * obj_kanameishi = disp_commonGetObj( OBJ_ID_KANAMEISHI_ITEM0_1);
	this->m_sprKaname.m_vec3DSize = D3DXVECTOR3((float)obj_kanameishi->w, (float)obj_kanameishi->h, 0.0f);
	this->m_sprKaname.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprKaname.m_iAlpha = 31;//
	this->m_sprKaname.m_iFileTexture = OBJ_ID_KANAMEISHI_ITEM0_1;//Obj ID
	
	//要石スプライト(下半分)
	this->m_sprKanameLower.m_vec3DPos = D3DXVECTOR3((float)camera_offset_x, camera_offset_y + 300.0f, 176.0f);//初期位置は下部
	this->m_sprKanameLower.m_vec3DSize = D3DXVECTOR3(232.0f, 119.0f, 0.0f);
	this->m_sprKanameLower.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprKanameLower.m_iAlpha = 31;//
	this->m_sprKanameLower.m_iFileTexture = OBJ_ID_KANAMEISHI_ITEM0_0;//Obj ID


	//天子スプライト
	this->m_sprTenko.m_vec3DPos = D3DXVECTOR3((float)camera_offset_x, camera_offset_y + 300.0f, 170.0f);//初期位置は下部
	this->m_sprTenko.m_vec3DSize =  D3DXVECTOR3(104.0f, 104.0f, 0.0f);
	this->m_sprTenko.m_iBlendMode = DISP_COMMON_BLEND_MODE_ALPHA;//
	this->m_sprTenko.m_iAlpha = 31;//
	this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_0;//Obj ID

	this->m_taskTenkoAnime = NULL;

	this->cnt++;
	
	this->ctrl->m_effect3D_z = static_cast<int>(this->m_sprKaname.m_vec3DPos.z - 1);//3DエフェクトのZ値を指定
}
//---------------------------------------------------------------------------

nn::yb::main::EFFECT_TENKO::~EFFECT_TENKO() {

	::nn_commonLog(1,  "EFFECT_TENKO END\n");

}

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_TENKO::task() {
	nn::yb::main::EFFECT_SPELL::task();

	bool end_flag = false;

	//スキップ処理
    ::KEY_WORK * key = &this->m_pBall-> master->keyWork;
	if (key->trigger & nn::common::e::PAD_WIN_BUTTON_A) 
	{
		//決定ボタンが押されていたら、要石激突エフェクト状態までスキップ
		if (this->m_eState >= 0 
			&& this->m_eState <= 6)
		{
			::nn_commonLog(1, "spell effect skip: this->m_eState = %d\n", this->m_eState);
			this->m_uiAnimeCounter = 0;
			this->iStarCt = 0;//カウンタをリセット

			if (this->m_taskCutIn != NULL)//カットインが表示されている場合、削除
			{ 
				((nn::common::dxlib::CAnime *)this->m_taskCutIn)->end();
				this->m_taskCutIn = NULL;
			}

			anime_cb(this);//アニメ終了コールバック処理

			//要石の初期化
			this->m_sprKanameLower.m_vec3DPos = D3DXVECTOR3(320, 264, 180 + 513);
			OBJ * obj_kanameishi = disp_commonGetObj( OBJ_ID_KANAMEISHI_ITEM0_0);
			this->m_sprKanameLower.m_vec3DSize = D3DXVECTOR3((float)obj_kanameishi->w, (float)obj_kanameishi->h, 0.0f);
			this->m_sprKanameLower.m_iAlpha = 31;
			
			this->ctrl->stopEffect();
			this->m_eState = 7;
			this->m_sprKaname.m_vec3DPos.y = (float)this->m_pBall->y - (float)this->m_pBall->z - 10.0f;
			this->m_sprTenko.m_vec3DPos.y = (float)this->m_pBall->y - (float)this->m_pBall->z - 130.0f;
			
			this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_4;//天子スプライトを切り替え
			this->m_pBall->setDispState(nn::yb::e::ball::DISP_SATTE_PERA, 120);//ゆっくりをぺらぺら状態に

			
  			if (this->m_taskTenkoAnime != NULL)
			{
				end_flag = true;//エフェクト終了フラグを立てる
				////::animationEnd( (ANIMATION *)(this->m_taskTenkoAnime->work) );

				s16 sTotalFrames = this->m_taskTenkoAnime->getFrame(::ANIMATION_ID_SPELL_GAME_CHAR_TENKO);
				if (
					this->m_taskTenkoAnime->frameCnt > 0  &&
					this->m_taskTenkoAnime->frameCnt < sTotalFrames //フレーム途中でキャンセルする場合
					)
				{
					this->m_taskTenkoAnime->end();
					////this->m_taskTenkoAnime->hide();
					////this->m_taskTenkoAnime->back();
					if (this->m_taskTenkoAnime->endCallBackFunc != NULL)
					{
						this->m_taskTenkoAnime->endCallBackFunc = NULL;
					}
					if (this->m_taskTenkoAnime->endCallBackFuncArg != NULL)
					{
						this->m_taskTenkoAnime->endCallBackFuncArg = NULL;
					}
				}
				this->m_taskTenkoAnime = NULL;
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

	this->m_uiAnimeCounter++;
	switch ( this->m_eState )
	{
	case 0://カットイン
		{
			if (
				this->m_uiAnimeCounter >= 30 + 15 ///指定フレーム経過した場合、次の状態へ
				)
			{
				nn::common::dxlib::CAnime * anime = nn::common::dxlib::CAnime::exec(
					this, ANIMATION_ID_SPELL_CHAR_3, 320, 240, 100 + 513, 0,
					nn::yb::main::EFFECT_SPELL::cutin_cb, this
				);
				anime->changeObjId( ANIME_CHANGE_OBJ_ID_TYPE_OBJ_ID, OBJ_ID_SPELL_CHAR_REIMU_0, OBJ_ID_SPELL_CHAR_TENKO_0);
				this->m_taskCutIn = (TASK *)anime;

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
				this->m_taskTenkoAnime = nn::common::dxlib::CAnime::exec(
					this, ::ANIMATION_ID_SPELL_GAME_CHAR_TENKO, 320, 240, 179 + 513, 0,
					nn::yb::main::EFFECT_TENKO::anime_cb, this
				);
				////this->m_taskTenkoAnime = animationExec( NULL, ANIMATION_ID_SPELL_GAME_CHAR_TENKO, 
				////	0, 0, 179 + 513, 0, nn::yb::main::EFFECT_TENKO::anime_cb, this);

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				break;

			}

		}
		break;


	case 2://背景消去
		{
			if (
				this->m_uiAnimeCounter >= 10//指定フレーム経過した場合、次の状態へ
				)
			{
				int darkenFrame = static_cast<int>(230 * SCALE_FPS_TENKO);

				//画面暗転(黒テクスチャ)を、不透明度32(MAX)にしてフェードインする
				this->ctrl->push_event( nn::yb::main::EFFECT_TENKO::call_back, this, NULL);
				nn::yb::main::effect_fadein_out(this, OBJ_ID_DUN_SPELL_BG_BLACK_0, nn::yb::enum_effect::SLIDE_MODE_FRAME,
					darkenFrame - 150, 191 + 513   ,DISP_COMMON_BLEND_MODE_ALPHA ,enum_effect::EFFECT_ANIME_ADD , 
					30,
					320, 320,
					240, 240,
					1024, 1024,
					512, 512,
					0, 0,
					0, 32,
					darkenFrame - 120 - (30 + 30),
					320, 320,
					240, 240,
					1024, 1024,
					512, 512,
					0, 0,
					32, 32,
					30,
					320, 320,
					240, 240,
					1024, 1024,
					512, 512,
					0, 0,
					32, 0,
					this->ctrl);//画面暗転
				this->cnt++;

				//登場時振動
				/*
				this->ctrl->push_event( nn::yb::main::EFFECT_TENKO::call_back, this, NULL);
				nn::yb::main::effect_quake(this, nn::yb::enum_effect::EFFECT_ANIME_SUB, 90, 35, 15, this->ctrl);//画面振動
				this->cnt++;
				*/


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				////end_flag = true;
				break;
			}
			
		
		}
		break;
	case 3://要石が下から拡大してフェードイン
		{
			/*
			if (
				this->m_uiAnimeCounter >= 120//指定フレーム経過した場合、次の状態へ
				)
			{
				//天子スプライトを切り替え
				this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_1;
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			
			const int iUpFrame = 120;
			float fCurrY = LunaAnimation::Add2<float, int>(this->m_vec3DStartPos.y + 350.0f, this->m_vec3DStartPos.y + 250.0f, iUpFrame, this->m_uiAnimeCounter);//着地まで
			this->m_sprKanameLower.m_vec3DPos.y = static_cast<int>(fCurrY);

			//拡大
			float fScale = 1.0f;
			fScale = static_cast<float>(LunaAnimation::Add2<float, int>(1.0f, 2.0f, iUpFrame, this->m_uiAnimeCounter));
			this->m_sprKanameLower.m_vec3DScale.x = fScale;
			this->m_sprKanameLower.m_vec3DScale.y = fScale;
			
			//フェードイン
			int iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Add2<float, int>(0, 24, iUpFrame, this->m_uiAnimeCounter));
			this->m_sprKanameLower.m_iAlpha = static_cast<int>(iAlpha);

		
			//天子
			 fCurrY = LunaAnimation::Add2<float, int>(this->m_vec3DStartPos.y + 300.0f, this->m_vec3DStartPos.y + 100.0f, iUpFrame, this->m_uiAnimeCounter);//着地まで
			this->m_sprTenko.m_vec3DPos.y = static_cast<int>(fCurrY);
			//拡大
			 fScale = 1.0f;
			fScale = static_cast<float>(LunaAnimation::Add2<float, int>(0.5f, 1.0f, iUpFrame, this->m_uiAnimeCounter));
			this->m_sprTenko.m_vec3DScale.x = fScale;
			this->m_sprTenko.m_vec3DScale.y = fScale;
			//フェードイン
			 iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Add2<float, int>(0, 28, iUpFrame, this->m_uiAnimeCounter));
			this->m_sprTenko.m_iAlpha = static_cast<int>(iAlpha);

			////if (this->m_uiAnimeCounter >= 30)
			////{
			////	//天子スプライトを切り替え
			////	this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_1;
			////}
			*/
		}
		break;
	case 4://てんこジャンプ + 要石拡大
		{
			/*
			if (
				this->m_uiAnimeCounter >= 45//指定フレーム経過した場合、次の状態へ
				)
			{
				//天子スプライトを切り替え
				this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_3;

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			
			const int iUpFrame = 45;
			////float fCurrY = LunaAnimation::Sub<float, int>(this->m_vec3DStartPos.y + 250.0f, this->m_vec3DStartPos.y + 150.0f, iUpFrame, this->m_uiAnimeCounter);//着地まで
			float fCurrY = LunaAnimation::Sub<float, int>(this->m_vec3DStartPos.y + 250.0f, this->m_pBall->y - this->m_pBall->z + 80.0f, iUpFrame, this->m_uiAnimeCounter);//着地まで
			this->m_sprKanameLower.m_vec3DPos.y = static_cast<int>(fCurrY);

			//拡大
			float fScale = 1.0f;
			fScale = static_cast<float>(LunaAnimation::Sub<float, int>(2.0f, 3.0f, iUpFrame, this->m_uiAnimeCounter));
			this->m_sprKanameLower.m_vec3DScale.x = fScale;
			this->m_sprKanameLower.m_vec3DScale.y = fScale;
			
			//フェードイン
			int iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Sub<float, int>(28, 31, iUpFrame, this->m_uiAnimeCounter));
			this->m_sprKanameLower.m_iAlpha = static_cast<int>(iAlpha);

		
			//てんこジャンプ
			 fCurrY = LunaAnimation::Add2<float, int>(this->m_vec3DStartPos.y + 100.0f, this->m_pBall->y - this->m_pBall->z - 500.0f, iUpFrame, this->m_uiAnimeCounter);//着地まで
			this->m_sprTenko.m_vec3DPos.y = static_cast<int>(fCurrY);

			if (this->m_uiAnimeCounter >= 20)
			{
				//天子スプライトを切り替え
				this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_2;
			}
			*/
		}
		break;
	case 5://要石落下
		{
			if (
				this->m_uiAnimeCounter >= 60//指定フレーム経過した場合、次の状態へ
				)
			{
				//天子スプライトを切り替え
				//this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_4;

				this->ctrl->playEffect();//play 3D effect

				nn::yb::main::CEffectCtrl::setScreenPos((float)this->m_pBall->x + 16, (float)(this->m_pBall->y - this->m_pBall->z));//エフェクトの座標設定

				sound_nnsPlaySe( SE_GROUND);//着弾SE

				//着地振動
				this->ctrl->push_event( nn::yb::main::EFFECT_TENKO::call_back, this, NULL);
				nn::yb::main::effect_quake(this, nn::yb::enum_effect::EFFECT_ANIME_SUB, 100, 35, 15, this->ctrl);//画面振動
				this->cnt++;


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				////end_flag = true;
				break;
			}
			
			const int iFallFrame = 60;
			////float fCurrZ = LunaAnimation::Add2<float, int>(this->m_vec3DStartPos.y - 300.0f, this->m_vec3DStartPos.y, iFallFrame, this->m_uiAnimeCounter);//着地まで
			float fCurrZ = LunaAnimation::Add2<float, int>((float)(this->m_pBall->y - this->m_pBall->z - 400.0), (float)(this->m_pBall->y - this->m_pBall->z) - 10.0f, iFallFrame, this->m_uiAnimeCounter);//着地まで

			this->m_sprKaname.m_vec3DPos.y = fCurrZ;
		
			this->m_sprTenko.m_vec3DPos.y = fCurrZ - 120.0f;
		}
		break;
	case 6://しゃがみ
		{
			if (
				this->m_uiAnimeCounter >= 100//指定フレーム経過した場合、次の状態へ
				)
			{
				//天子スプライトを切り替え
				this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_4;

				this->m_pBall->setDispState(nn::yb::e::ball::DISP_SATTE_PERA, 120);//ゆっくりをぺらぺら状態に

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				////end_flag = true;
				break;
			}
			const int iMoveFrame = 60;

			//拡大
			float fScale = LunaAnimation::Sub<float, int>(
				1.0f, 2.0f,
				iMoveFrame, this->m_uiAnimeCounter);//
			this->ctrl->setScale(fScale, fScale, fScale);

		}
		break;
	case 7://要石消去
		{
			if (
				this->m_uiAnimeCounter >= 13//指定フレーム経過した場合、次の状態へ
				)
			{
				//天子スプライトを切り替え
				this->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_5;
				this->m_sprKaname.m_iFileTexture = ::OBJ_ID_KANAMEISHI_ITEM0_2;

				//this->m_pBall->setDispState(nn::yb::e::ball::DISP_SATTE_PERA, 120);//ゆっくりをぺらぺら状態に

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				////end_flag = true;
				break;
			}

		}
		break;
	case 8://消去処理
		{
			if (
				this->m_uiAnimeCounter >= 30//指定フレーム経過した場合、次の状態へ
				)
			{
				this->ctrl->stopEffect();//stop 3D effect

				////m_vec3DPos = nn::yb::main::D3DXVECTOR3(this->m_vec3DStartPos.x - 240, this->m_vec3DStartPos.y - 180, this->m_vec3DStartPos.z);//スタート座標
				m_vec3DPos = nn::yb::main::D3DXVECTOR3(this->m_vec3DStartPos.x, this->m_vec3DStartPos.y, this->m_vec3DStartPos.z);//スタート座標

				/*
				this->m_pBall->speed *= 0.2f;//スピードは急減速
				*/

				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				////end_flag = true;
				break;
			}
			const int iFrame = 30;
			//フェードアウト
			int iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Sub2<float, int>(31, 0, iFrame, this->m_uiAnimeCounter));
		
			this->m_sprKaname.m_iAlpha = static_cast<int>(iAlpha);

			//this->m_sprKanameLower.m_iAlpha = static_cast<int>(iAlpha);

			//てんこ画面外へ
			float fTenkoStartY = static_cast<float>(this->m_pBall->y - this->m_pBall->z - 10.0f - 96.0f);
			float fCurrX = LunaAnimation::Sub2<float, int>(this->m_vec3DStartPos.x, this->m_vec3DStartPos.x - 50, iFrame, this->m_uiAnimeCounter);//
			float fCurrY = LunaAnimation::Sub2<float, int>(fTenkoStartY, fTenkoStartY - 640, iFrame, this->m_uiAnimeCounter);//

			this->m_sprTenko.m_vec3DPos.x = fCurrX;
			this->m_sprTenko.m_vec3DPos.y = fCurrY;

		}
		break;
		/*
	case 8://ぺらぺらのまま移動処理
		{
			if (
				this->m_uiAnimeCounter >= 90//指定フレーム経過した場合、次の状態へ
				)
			{
 
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				end_flag = true;
				break;
			}
			const int iFrame = 90;
			const int iMoveFrame = 60;

			const float fMoveDistance = this->m_pBall->speed * 25.0f;

			//十字キーでゆっくり移動

			//キー入力で若干操作可能
			const float cfKeyMoveLength = 10.0f;
			float fVecX = 0.0f;
			float fVecY = 0.0f;
			if(key->trigger & nn::common::e::PAD_WIN_KEY_LEFT) {
				// 左キー
				fVecX = cfKeyMoveLength * ::cos( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
				fVecY = cfKeyMoveLength * ::sin( ( ( (360 - this->angle - 45 ) ) % 360) * PI / 180.0);
				::nn_commonLog(1,  "LeftKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
				//進行方向に対して左側にカーブ
			}
			else if(key->trigger & nn::common::e::PAD_WIN_KEY_RIGHT) {
				// 右キー
				fVecX = cfKeyMoveLength * ::cos( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
				fVecY = cfKeyMoveLength * ::sin( ( ( (360 - this->angle + 45 ) ) % 360) * PI / 180.0);
				::nn_commonLog(1,  "RightKey is pressed. fVecX, fVecY= (%f, %f)\n", fVecX, fVecY);
				//進行方向に対して右側にカーブ
			}

			//移動
			float fDiffX = fMoveDistance * ::cos( ( ( (360 - this->angle) ) % 360) * PI / 180.0) / iMoveFrame;
			float fDiffY = fMoveDistance * ::sin( ( ( (360 - this->angle) ) % 360) * PI / 180.0) / iMoveFrame;
			m_vec3DPos.x += fDiffX;//進行方向へ移動
			m_vec3DPos.y += fDiffY;

			//キー入力を受け付けるのは現状態のフレーム数の前半2/3のみ、一定フレームたったらキー入力を受け付けない
			if (this->m_uiAnimeCounter < iMoveFrame * 2 / 3.0)
			{
				m_vec3DPos.x += fVecX;//キー入力による左右への補正
				m_vec3DPos.y += fVecY;
			}

			float fCurrX = m_vec3DPos.x ;
			float fCurrY = m_vec3DPos.y ;
				

			//ジャンプ中の場合、x,yの移動は殆ど無くなり、z方向の上昇は無くなりスロウに下降
			if (!this->m_pBall->flyGroundOnFlag)
			{
				if (this->m_pBall->zSpeed > 0)
				{
					this->m_pBall->zSpeed *= 0.1;
				}
				else if (
					this->m_pBall->zSpeed < -2.0
					//&& this->m_pBall->zSpeed  
					)
				{
					this->m_pBall->zSpeed = -0.5;
				}
			}


			//ゆっくりも移動
			this->m_pBall->x = static_cast<int>(fCurrX);
			this->m_pBall->y = static_cast<int>(fCurrY);

			//カメラも移動(竜巻は画面中心位置のまま固定)
			this->m_pBall->holeCtrl->ctrl->camera->moveCameraPos(fCurrX, fCurrY - this->m_pBall->z);
			
		}
		break;
		*/
	case 9://終了処理
		{
			if (
				this->m_uiAnimeCounter >= 30//指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				////end_flag = true;
				break;
			}
		}
		break;
	case 10://終了処理
		{
			if (
				this->m_uiAnimeCounter >= 30//指定フレーム経過した場合、次の状態へ
				)
			{
				this->m_taskTenkoAnime = NULL;

				this->ctrl->stopEffect();
				this->ctrl->unloadEffect();//unload 3D effect


				this->m_uiAnimeCounter = 0;
				this->m_eState++;
				////end_flag = true;

				nn::yb::main::EFFECT_TENKO::call_back( this, 0);
				
				////if (end_flag)//スキップの場合、暗転エフェクト等が残っていても強制終了
				{
					this->m_pBall->spellEffectEndTenko();
					this->initialize();
					this->ctrl->clear_cb();// clear callback
					this->taskDelete();// 
					this->m_pBall->m_pCurrSpellEffect = NULL;
				
					this->m_taskCutIn = NULL;
				}

				break;
			}
			const int iFrame = 30;
			//フェードアウト
			int iAlpha = 0;
			iAlpha = static_cast<int>(LunaAnimation::Sub2<float, int>(31, 0, iFrame, this->m_uiAnimeCounter));
		
			//this->m_sprKaname.m_iAlpha = static_cast<int>(iAlpha);

			this->m_sprKanameLower.m_iAlpha = static_cast<int>(iAlpha);


		}
		break;
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_TENKO::disp() {

	nn::yb::main::EFFECT_SPELL::disp();

	this->m_sprTenko.disp();
	this->m_sprKaname.disp();
	this->m_sprKanameLower.disp();

	const int ciWidthScale = 20;
	switch ( this->m_eState )
	{
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		{
			if(5 <= this->m_eState && this->m_eState <= 6) {
				disp_commonObjCenterColor( OBJ_ID_MAIN_SHADOW_0, 320, 240,
					nn::yb::e::global::DISP_Z_SPELL_BALL + 1 + 513, RGB_WHITE, 16
				);
			}
			if((6 <= this->m_eState && this->m_eState <= 7) ||
				( this->m_eState == 5 && this->m_uiAnimeCounter >=30)
			) {
				OBJ * obj_kanameishi = disp_commonGetObj( OBJ_ID_KANAMEISHI_ITEM0_0);
				::disp_commonObjCenterFull( 0, OBJ_ID_MAIN_SHADOW_0, 320, 234,
					nn::yb::e::global::DISP_Z_SPELL_BALL - 1 + 513,
					260, 100, 0, FALSE, FALSE,
					RGB_WHITE, 16
				);
			}
		}
		break;
	}
}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_TENKO::call_back( void * effectTENKOArg, void * dummy) {

	nn::yb::main::EFFECT_TENKO * effect = (nn::yb::main::EFFECT_TENKO *)effectTENKOArg;

	effect->cnt--;
	if(effect->cnt == 0) {
		effect->ctrl->pop_event();
		effect->taskDelete();
	}

}

//---------------------------------------------------------------------------

void nn::yb::main::EFFECT_TENKO::anime_cb( void * effectTENKOArg) {

	nn::yb::main::EFFECT_TENKO * effect = (nn::yb::main::EFFECT_TENKO *)effectTENKOArg;

	effect->m_sprTenko.m_iFileTexture = ::OBJ_ID_GAME_CHAR_TENKO_3;

	effect->m_uiAnimeCounter = 0;
	effect->m_eState = 5;

	effect->m_sprKanameLower.m_vec3DPos = D3DXVECTOR3(320, 264, 180 + 513);
	OBJ * obj_kanameishi = disp_commonGetObj( OBJ_ID_KANAMEISHI_ITEM0_0);
	effect->m_sprKanameLower.m_vec3DSize = D3DXVECTOR3((float)obj_kanameishi->w, (float)obj_kanameishi->h, 0.0f);

}

//---------------------------------------------------------------------------


