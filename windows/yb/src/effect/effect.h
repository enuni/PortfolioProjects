
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "task_save.h"

////namespace yb {

// enum
namespace nn {
namespace yb {

namespace enum_effect {

enum {
	SLIDE_MODE_FRAME,
	SLIDE_MODE_DISP,

	SLIDE_MODE_MAX
};

enum SPELL_EFFECT_KIND{
	KIND_MUSOFUIN,//霊符「夢想封印」
	KIND_MUSOFUIN_SAN,//夢符「夢想封印・散」
	KIND_SHINREI_MUSOFUIN,//神霊「夢想封印」
	KIND_FUUMAZIN,//夢符「封魔陣」
	KIND_NIJUUKEKKAI,//夢符「二重結界」
	KIND_NIJUUDAIKEKKAI,//夢境「二重大結界」

	KIND_GUNGNIR,//神槍「スピア・ザ・グングニル」

	EFFECT_SPELL_KIND_MAX
};


enum {
	EFFECT_ANIME_FLAT,
	EFFECT_ANIME_ADD,
	EFFECT_ANIME_SUB,

	EFFECT_ANIME_MAX
};

}

//-----------------------------------------------------------------------------

namespace main {
	
class CEffectCtrl;

class EFFECT_THROW : public ::TASK_SAVE {

public:
	EFFECT_THROW();
	EFFECT_THROW( ::TASK_SAVE * parent, nn::yb::main::EFFECT_THROW * effectThrowArg);
	virtual ~EFFECT_THROW();
	void task();
	void disp();
	
	int dispObjId;
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int frame;
	int dispX;
	int dispY;
	int varX;
	int varY;

private:
	int cnt;

};

//-----------------------------------------------------------------------------

//srcPosXyからdstPosXyまでのobjDispIdが移動するエフェクトタスクを生成する
//PosXyマス上の座標
//終了の通知をctrlに対してのevet_pop()で行う
//親タスクを設定しておけば 親タスクが削除されると削除されるように出来る
nn::yb::main::EFFECT_THROW * effect_throw( TASK_SAVE * parent, int srcPosXy, int dstPosXy, int objDispId, nn::yb::main::CEffectCtrl * ctrlArg);

//-----------------------------------------------------------------------------

class EFFECT_SLIDE : public ::TASK_SAVE {

public:
	EFFECT_SLIDE();
	EFFECT_SLIDE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_SLIDE * effectSlideArg);
	virtual ~EFFECT_SLIDE();
	virtual void task();
	virtual void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int mode;	//スライドモード
	int objId;	//表示オブジェID
	
	bool flipH;	//上下反転描画フラグ
	bool flipV;	//左右反転描画フラグ
	
	float x;	//表示x座標
	float y;	//表示y座標
	float vX;	//表示x座標の1フレームでの変化量
	float vY;	//表示y座標の1フレームでの変化量
	int z;	//表示優先順位
	int frame;	//スライドするフレーム モードがフレーム時のみ有効
	float w;	//縦
	float h;	//横
	float vW;	//横の1フレームでの変化量
	float vH;	//縦の1フレームでの変化量
	float r;	//角度
	float vR;	//角度の1フレームでの変化量
	float a;	//不透明度
	float vA;	//不透明度の1フレームでの変化量
	
	int blend;	//ブレンドモード(enum DISP_COMMON_BLEND_MODE参照)
	float aX;	//表示x座標の加速度
	float aY;	//表示y座標の加速度
	float aW;	//横の加速度
	float aH;	//縦の加速度
	float aR;	//角度の加速度
	float aA;	//不透明度の加速度
	
	float lX;	//表示x座標の限界値
	float lY;	//表示y座標の加速度
	float lW;	//横の加速度
	float lH;	//縦の加速度
	float lR;	//角度の加速度
	float lA;	//不透明度の加速度
	
	int camera_offset_x;//カメラオフセットX
	int camera_offset_y;//カメラオフセットY
protected:
	int cnt;
	
};

//-----------------------------------------------------------------------------

//オブジェがxyから放射状に移動するエフェクトタスクを生成する
//終了の通知をctrlに対してのevet_pop()で行う
//親タスクを設定しておけば 親タスクが削除されると削除されるように出来る
nn::yb::main::EFFECT_SLIDE * effect_out_simple( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, nn::yb::main::CEffectCtrl * ctrlArg);
nn::yb::main::EFFECT_SLIDE * effect_out( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, int h, int r, int a, nn::yb::main::CEffectCtrl * ctrlArg);
////nn::yb::main::EFFECT_SLIDE * effect_out_ex( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg);
nn::yb::main::EFFECT_SLIDE * effect_out_ex( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg, int frame = 60);
nn::yb::main::EFFECT_SLIDE * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_SLIDE * effectSlideArg);

//-----------------------------------------------------------------------------

//added by nullpo 20090112 
//-----------------------------------------------------------------------------
//スペルカード共通エフェクト
nn::yb::main::EFFECT_SLIDE * effect_out_ex_a( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg,
													 float aX = 0.0f, float aY = 0.0f, float aW = 0.0f, float aH = 0.0f, float aR = 0.0f, float aA = 0.0f);//加速度設定有り

nn::yb::main::EFFECT_SLIDE * effect_out_ex_a_spd( TASK_SAVE * parent, int objId, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg,
													 float aSpeed = 0.0f, float aW = 0.0f, float aH = 0.0f, float aR = 0.0f, float aA = 0.0f, bool flipH = FALSE, bool flipV = FALSE, int cameraX = 0, int cameraY = 0);//加速度設定有り(speedの加速度を設定)

nn::yb::main::EFFECT_SLIDE * effect_out_ex_limit( TASK_SAVE * parent, int objId,
													 int x, int y, int z,
													 int angle, int speed, 
													 int w, float vW, int h, float vH, int r, float vR, int a, float vA,
													 int blend, nn::yb::main::CEffectCtrl * ctrlArg,
													 float aX = 0.0f, float aY = 0.0f, float aW = 0.0f, float aH = 0.0f, float aR = 0.0f, float aA = 0.0f,
													 float lX = 640.0f, float lY = 480.0f, float lW = 1024.0f, float lH = 1024.0f, float lR = 359.0f, float lA = 31.0f
													 );//加速度＋限界設定有り
nn::yb::main::EFFECT_SLIDE * effect_bg_mahouzin( TASK_SAVE * parent, int x, int y, int z, nn::yb::main::CEffectCtrl * ctrlArg);//背景魔方陣
nn::yb::main::EFFECT_SLIDE * effect_first_flash( TASK_SAVE * parent, int x, int y, int z, nn::yb::main::CEffectCtrl * ctrlArg);//開始時衝撃波


//-----------------------------------------------------------------------------
//移動・拡大縮小・α変化などのアニメーションを行うエフェクト
class EFFECT_ANIM : public ::TASK_SAVE {

public:
	EFFECT_ANIM();
	EFFECT_ANIM( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM * effectAnimArg);
	virtual ~EFFECT_ANIM();
	virtual void task();
	virtual void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int mode;	//スライドモード
	int objId;	//表示オブジェID
	
	float x;	//表示x座標
	float y;	//表示y座標
	float vX;	//表示x座標の1フレームでの変化量
	float vY;	//表示y座標の1フレームでの変化量
	int z;	//表示優先順位
	int frame;	//スライドするフレーム モードがフレーム時のみ有効
	float w;	//縦
	float h;	//横
	float vW;	//横の1フレームでの変化量
	float vH;	//縦の1フレームでの変化量
	float r;	//角度
	float vR;	//角度の1フレームでの変化量
	float a;	//不透明度
	float vA;	//不透明度の1フレームでの変化量
	
	int blend;	//ブレンドモード(enum DISP_COMMON_BLEND_MODE参照)
	int kind;	//アニメーションスピード(enum EFFECT_ANIME参照)

	float aX;	//表示x座標の加速度
	float aY;	//表示y座標の加速度
	float aW;	//横の加速度
	float aH;	//縦の加速度
	float aR;	//角度の加速度
	float aA;	//不透明度の加速度
	
	//始点・終点・始点から終点までにかける時間
	int sx; int ex; int tx;
	int sy; int ey; int ty;
	int sw; int ew; int tw;
	int sh; int eh; int th;
	int sr; int er; int tr;
	int sa; int ea; int ta;

	
	int camera_offset_x;//カメラオフセットX
	int camera_offset_y;//カメラオフセットY
	GXRgb color;

protected:
	int cnt;
	
};
//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_ANIM * effect_start_end_total( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int sx, int ex, int tx,
													 int sy, int ey, int ty,
													 int z, 
													 int sw, int ew, int tw,
													 int sh, int eh, int th,
													 int sr, int er, int tr,
													 int sa, int ea, int ta,
													 int blend, 
													 int kind, 
													 GXRgb color,
													 nn::yb::main::CEffectCtrl * ctrlArg
													 );//初期値・終了値・全体値設定

//-----------------------------------------------------------------------------
//移動・拡大縮小・α変化などのアニメーションを行うエフェクト(fade in/animation/fade outの3状態遷移)
class EFFECT_ANIM_FADE : public ::TASK_SAVE {

public:
	EFFECT_ANIM_FADE();
	EFFECT_ANIM_FADE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_FADE * effectAnimArg);
	virtual ~EFFECT_ANIM_FADE();
	void task();
	void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	int mode;	//スライドモード
	int objId;	//表示オブジェID
	
	float x;	//表示x座標
	float y;	//表示y座標
	int z;	//表示優先順位
	int frame;	//スライドするフレーム モードがフレーム時のみ有効
	float w;	//縦
	float h;	//横
	float r;	//角度
	float a;	//不透明度
	
	int blend;	//ブレンドモード(enum DISP_COMMON_BLEND_MODE参照)
	int kind;	//アニメーションスピード(enum EFFECT_ANIME参照)

	
	//始点・終点・始点から終点までにかける時間
	 //fade in
	 int intime;
	 int isx; int iex; 
	 int isy; int iey;
	 int isw; int iew; 
	 int ish; int ieh; 
	 int isr; int ier; 
	 int isa; int iea; 
	 //animation
	 int animtime;
	 int asx; int aex; 
	 int asy; int aey;
	 int asw; int aew; 
	 int ash; int aeh; 
	 int asr; int aer; 
	 int asa; int aea; 
	 //fade out
	 int outtime;
	 int osx; int oex; 
	 int osy; int oey;
	 int osw; int oew; 
	 int osh; int oeh; 
	 int osr; int oer; 
	 int osa; int oea; 
	 
	int camera_offset_x;//カメラオフセットX
	int camera_offset_y;//カメラオフセットY

protected:
	int cnt;
	
};

//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_FADE * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_FADE * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_ANIM_FADE * effect_fadein_out( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int z, 
													 int blend, 
													 int kind, 
													 //fade in
													 int intime,
													 int isx, int iex, 
													 int isy, int iey,
													 int isw, int iew, 
													 int ish, int ieh, 
													 int isr, int ier, 
													 int isa, int iea, 
													 //animation
													 int animtime,
													 int asx, int aex, 
													 int asy, int aey,
													 int asw, int aew, 
													 int ash, int aeh, 
													 int asr, int aer, 
													 int asa, int aea, 
													 //fade out
													 int outtime,
													 int osx, int oex, 
													 int osy, int oey,
													 int osw, int oew, 
													 int osh, int oeh, 
													 int osr, int oer, 
													 int osa, int oea, 
													 nn::yb::main::CEffectCtrl * ctrlArg
													 );//初期値・終了値・全体値設定
//-----------------------------------------------------------------------------

//ループアニメーションエフェクト
class EFFECT_LOOP : public nn::yb::main::EFFECT_ANIM_FADE {

public:
	EFFECT_LOOP();
	EFFECT_LOOP( ::TASK_SAVE * parent, nn::yb::main::EFFECT_LOOP * effectAnimArg);
	virtual ~EFFECT_LOOP();
	void task();
	void disp();
	

	float m_fLoopSpeedX;//ループする速度
	float m_fLoopSpeedY;
	float m_fMoveSpeedX;//このオブジェクト全体が移動する速度
	float m_fMoveSpeedY;

protected:
	float m_fLoopStartPosX;//m_cLoobObjの内、画像を描画する開始座標
	float m_fLoopStartPosY;//m_cLoobObjの内、画像を描画する開始座標
	int m_iLoopNumX;
	int m_iLoopNumY;//x,ｙ方向に描画する回数

	
};

//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_LOOP * effect_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_LOOP * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_LOOP * effect_loop_fadein_out( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int z, 
													 int blend, 
													 int kind, 
													 //loop speed
													 int speed_x, int speed_y,
													 //fade in
													 int intime,
													 int isx, int iex, 
													 int isy, int iey,
													 int isw, int iew, 
													 int ish, int ieh, 
													 int isr, int ier, 
													 int isa, int iea, 
													 //animation
													 int animtime,
													 int asx, int aex, 
													 int asy, int aey,
													 int asw, int aew, 
													 int ash, int aeh, 
													 int asr, int aer, 
													 int asa, int aea, 
													 //fade out
													 int outtime,
													 int osx, int oex, 
													 int osy, int oey,
													 int osw, int oew, 
													 int osh, int oeh, 
													 int osr, int oer, 
													 int osa, int oea, 
													 nn::yb::main::CEffectCtrl * ctrlArg
													 );//初期値・終了値・全体値設定
//-----------------------------------------------------------------------------
//画面振動エフェクトクラス
class EFFECT_QUAKE : public ::TASK_SAVE {

public:
	EFFECT_QUAKE();
	EFFECT_QUAKE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_QUAKE * effectArg);
	virtual ~EFFECT_QUAKE();
	void task();
	void disp();
	
	nn::yb::main::CEffectCtrl * ctrl;
	
	
	int kind;	//通常振動/減衰振動/増加振動
	int frame;	//フレーム
	float w;	//縦
	float h;	//横
	
private:
	int cnt;//カウンタ
	
};
//-----------------------------------------------------------------------------
//揺らすフレーム、幅、高さを指定
nn::yb::main::EFFECT_QUAKE * effect_quake( TASK_SAVE * parent, int kind, int frame, int w, int h, nn::yb::main::CEffectCtrl * ctrlArg );

//-----------------------------------------------------------------------------

class EFFECT_MUSOFUINSAN : public ::TASK_SAVE {

public:
	EFFECT_MUSOFUINSAN();
	EFFECT_MUSOFUINSAN( ::TASK_SAVE * parent, nn::yb::main::EFFECT_MUSOFUINSAN * effectMusofuinArg);
	virtual ~EFFECT_MUSOFUINSAN();
	void task();
	void disp();
	static void call_back( void * effectMusoufuinArg, void * dummy);
	
	nn::yb::main::CEffectCtrl * ctrl;
	
private:
	int cnt;
	int ballCnt;
	int cardCnt;
	int frameCnt;
	
};

//-----------------------------------------------------------------------------

//複数の画像によるアニメーションエフェクト
class EFFECT_FRAME_ANIMATION : public nn::yb::main::EFFECT_SLIDE {

public:
	EFFECT_FRAME_ANIMATION();
	EFFECT_FRAME_ANIMATION( ::TASK_SAVE * parent, nn::yb::main::EFFECT_FRAME_ANIMATION * effectSlideArg);
	virtual ~EFFECT_FRAME_ANIMATION();
	void task();
	void disp();

	int objIdMax;//アニメーションの全枚数
	
};
nn::yb::main::EFFECT_FRAME_ANIMATION * effect_frame_anime( TASK_SAVE * parent, int objId, int objIdMax, int x, int y, int z, int angle, int speed, int w, float vW, int h, float vH, int r, float vR, int a, float vA, int blend, nn::yb::main::CEffectCtrl * ctrlArg, int frame = 60);
nn::yb::main::EFFECT_FRAME_ANIMATION * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_FRAME_ANIMATION * effectArg) ;


//-----------------------------------------------------------------------------
//移動・拡大縮小・α変化などのアニメーションを行うエフェクト(ホールでのエフェクト用)
class EFFECT_ANIM_HOLE : public nn::yb::main::EFFECT_ANIM {

public:
	EFFECT_ANIM_HOLE();
	EFFECT_ANIM_HOLE( ::TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_HOLE * effectAnimArg);
	virtual ~EFFECT_ANIM_HOLE();
	void task();
	void disp();

public:
	void (* pop_exec_cb)(void *, void *);//pop処理関数のポインタ
};
//-----------------------------------------------------------------------------

nn::yb::main::EFFECT_ANIM_HOLE * effect_out_arg( TASK_SAVE * parent, nn::yb::main::EFFECT_ANIM_HOLE * effectAnimArg);

//-----------------------------------------------------------------------------
nn::yb::main::EFFECT_ANIM_HOLE * effect_start_end_total_hole( TASK_SAVE * parent, int objId, int mode,
													 int frame,
													 int sx, int ex, int tx,
													 int sy, int ey, int ty,
													 int z, 
													 int sw, int ew, int tw,
													 int sh, int eh, int th,
													 int sr, int er, int tr,
													 int sa, int ea, int ta,
													 int blend, 
													 int kind, 
													 GXRgb color,
													 nn::yb::main::CEffectCtrl * ctrlArg,
													 void (*pop_exec_cb)(void *, void *)
													 );//初期値・終了値・全体値設定
//-----------------------------------------------------------------------------
} //namespace main

} //namespace yb
} //namespece nn

#endif __EFFECT_H__