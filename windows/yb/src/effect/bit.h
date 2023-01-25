
#ifndef __BIT_H__
#define __BIT_H__


#include "task.h"
#include "task_save.h"
#include "effect\\effect.h"
#include "ball.h"

#include <math.h>
#include <queue>

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {
	
//-----------------------------------------------------------------------------
struct D3DXVECTOR3
{
	float x;
	float y;
	float z;
public:
	D3DXVECTOR3()
	{
		x = y = z = 0.0f;
	}
	D3DXVECTOR3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	float GetLength()//3次元の距離
	{
		return ::sqrt(x * x + y * y + z * z);
	}
	float Get2DLength()//x, yのみの距離
	{
		return ::sqrt(x * x + y * y);
	}
	void Add (const D3DXVECTOR3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}
	void Sub (const D3DXVECTOR3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}
	D3DXVECTOR3 &D3DXVECTOR3::operator = (const D3DXVECTOR3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}
	D3DXVECTOR3 &D3DXVECTOR3::operator += ( const D3DXVECTOR3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};

//ホーミングビットクラス
class BIT : public ::TASK_SAVE {

public:
	BIT();
	BIT( ::TASK_SAVE * parent, nn::yb::main::BIT * effectArg, int frame, int x, int y);
	virtual ~BIT();
	void task();
	void disp();
	static void call_back( void * effectArg, void * dummy);
	
	nn::yb::main::CEffectCtrl * ctrl;
	
protected:
	int cnt;
	int frameCnt;

	int totalFrame;

	
	int blend;	//ブレンドモード(enum DISP_COMMON_BLEND_MODE参照)
	int frame;	//表示する最大フレーム

protected:
	UINT m_eState;//状態変数

	float m_fTargetAngle;//目標への角度
	float m_fHomingTargetAngle;//目標への角度を1回のホーミング処理で変化させる時の角度
	UINT m_uiScaleCounter;//拡大縮小用カウンター
	float m_fScaleLimit;

	UINT m_uiAnimeCounter;
	UINT m_uiAnimeCounterLimit;

	
	D3DXVECTOR3 m_vec3DSpeed;//速度
	D3DXVECTOR3 m_vec3DAccel;//加速度

public:
	int objId;	//表示オブジェID
	
	float w;	//縦
	float h;	//横
	float r;	//角度
	float a;	//不透明度

	std::vector< D3DXVECTOR3 > m_vecAnimPos;
	
	D3DXVECTOR3 m_vec3DPos;
	D3DXVECTOR3 m_vec3DScale;
	D3DXVECTOR3 m_vec3DTarget;//移動目標座標

	bool m_bTargetHitEffect;//目標座標に着弾エフェクトを出すかどうかのフラグ

	int m_iID;//ビット着弾時コールバック用ID
	
	////void (* eventCallBackFuncStack[256])( void *, void *);
	void  (*spellEffectMusoCb)( void *, int);//ビット着弾時コールバック関数
	nn::yb::main::CBall* ball;//ビット着弾時コールバック関数の引数(CBall)
	
protected:
	void updateHoming();//ホーミング処理
};

//-----------------------------------------------------------------------------

nn::yb::main::BIT * effect_bit_arg( TASK_SAVE * parent, nn::yb::main::BIT * effectAnimArg, int frame, int  x, int y);

//-----------------------------------------------------------------------------
nn::yb::main::BIT * effect_bit2( TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, nn::yb::main::CBall* pBall, int ballCnt, void  (*pSpellEffectMusoCb)( void *, int), nn::yb::main::CEffectCtrl * ctrlArg, bool m_bTargetHitEffect);
nn::yb::main::BIT * effect_bit( TASK_SAVE * parent, int frame, int x, int y, int tx, int ty, int size, int color, nn::yb::main::CBall* pBall, int ballCnt, void  (*pSpellEffectMusoCb)( void *, int), nn::yb::main::CEffectCtrl * ctrlArg);
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb

} //namespece nn
#endif __BIT_H__