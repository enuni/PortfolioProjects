
#ifndef __EFFECT_STAR_H__
#define __EFFECT_STAR_H__


#include "task.h"
#include "task_save.h"
#include "effect\\effect.h"

#include "bit.h"

#include <math.h>
#include <queue>

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
struct D3DXVECTOR3;
//☆エフェクトクラス
class CEffectStar : public ::TASK_SAVE {

public:
	CEffectStar();
	CEffectStar( ::TASK_SAVE * parent, nn::yb::main::CEffectStar * effectArg, int frame, int x, int y);
	virtual ~CEffectStar();
	void task();
	void disp();
	static void call_back( void * effectArg, void * dummy);
	
	::nn::yb::main::CEffectCtrl * ctrl;
	
protected:
	int cnt;
	int frameCnt;

	int totalFrame;

	
	int blend;	//ブレンドモード(enum DISP_COMMON_BLEND_MODE参照)
	int frame;	//表示する最大フレーム

protected:
	UINT m_eState;//状態変数

	UINT m_uiScaleCounter;//拡大縮小用カウンター
	float m_fScaleLimit;

	UINT m_uiAnimeCounter;
	UINT m_uiAnimeCounterLimit;

	
	D3DXVECTOR3 m_vec3DSpeed;//速度
	D3DXVECTOR3 m_vec3DAccel;//加速度


	
	int objIdOffset;	//表示オブジェIDのオフセット(色の切り替え)

	static int POS_STARBIT_FROM_X;
	static int POS_STARBIT_FROM_Y;
	static int POS_STARBIT_WIDTH;
	static int POS_STARBIT_HEIGHT;
	
	static float STARBIT_SPEED_X_INIT;
	static float STARBIT_SPEED_Y_INIT ;
	static float STARBIT_ACC_X_INIT ;
	static float STARBIT_ACC_Y_INIT ;

public:
	int objId;	//表示オブジェID
	
	float w;	//縦
	float h;	//横
	float r;	//角度
	float a;	//不透明度

	D3DXVECTOR3 m_vec3DPos;
	D3DXVECTOR3 m_vec3DScale;
	D3DXVECTOR3 m_vec3DTarget;//移動目標座標
	
	int m_iStarNum;//★オブジェクトの個数
	float m_fScale;//★オブジェクトのScale

};

//-----------------------------------------------------------------------------

nn::yb::main::CEffectStar * effect_star_arg( TASK_SAVE * parent, nn::yb::main::CEffectStar * effectAnimArg, int frame, int  x, int y);

//-----------------------------------------------------------------------------
nn::yb::main::CEffectStar * effect_star( TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, int numEffect, float fScale,  nn::yb::main::CEffectCtrl * ctrlArg);
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb

} //namespece nn

#endif __EFFECT_STAR_H__