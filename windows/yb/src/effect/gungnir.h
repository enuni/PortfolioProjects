
#ifndef __GUNGNIR_H__
#define __GUNGNIR_H__


#include "task.h"
#include "task_save.h"
#include "effect.h"
#include "effect_spell.h"
#include "bit.h"
#include "class_list.h"

namespace nn {
namespace yb {

//-----------------------------------------------------------------------------

namespace main {

struct D3DXVECTOR3;

//スプライトクラス
class EFFECT_SPRITE 
{
public:
	EFFECT_SPRITE();
	virtual ~EFFECT_SPRITE();

	void update();
	void disp();
	void dispMain();

public:

	D3DXVECTOR3	m_vec3DPos;	
	D3DXVECTOR3	m_vec3DSize;		//幅、高さ

	D3DXVECTOR3	m_vec3DRotation;
	D3DXVECTOR3	m_vec3DScale;

	int m_iFileTexture;	//テクスチャのObjID

	int m_iAlpha;//アルファ値

	int m_iBlendMode;//Blend mode

};


class CBall;
//-----------------------------------------------------------------------------

//神槍「スピア・ザ・グングニル」
	class EFFECT_GUNGNIR : public nn::yb::main::EFFECT_SPELL {

public:
	EFFECT_GUNGNIR();
	EFFECT_GUNGNIR( ::TASK_SAVE * parent, nn::yb::main::EFFECT_GUNGNIR * effectGUNGNIRArg);
	virtual ~EFFECT_GUNGNIR();
	void task();
	void disp();
	static void call_back( void * effectGUNGNIRArg, void * dummy);
	static void anime_cb( void * effectGUNGNIRArg);
	


public:
	D3DXVECTOR3	m_vec3DTargetPos;//ターゲットの座標	
	D3DXVECTOR3	m_vec3DWavePos;//着地衝撃波の座標	

	bool m_bIsGround;//地面に着地できるかどうか
	float m_fGroundZ;//着地のZ座標(地面がある場合は０、無い場合は落下)

 
	nn::yb::main::CBall * m_pBall;//CBallへの参照
	::TASK_SAVE * m_taskBigbulletParent;

protected:
	////int cnt;
	////int frameCnt;

	int frame;	//表示する最大フレーム


	

protected:
	EFFECT_SPRITE m_sprRemiria;
	EFFECT_SPRITE m_sprGungnir;
	////EFFECT_SPRITE m_sprMagicCircle;
	////EFFECT_SPRITE m_sprLightPillar;

};
//-----------------------------------------------------------------------------
} //namespace main

} //namespece yb
} //namespace nn

#endif __GUNGNIR_H__