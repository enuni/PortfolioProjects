// EDIT_HOLE_OBJ を置換
// edit_hole_obj を置換
// EditHoleObj を置換
// yb を置換
// edit を置換

#ifndef __EDIT_HOLE_OBJ_H__
#define __EDIT_HOLE_OBJ_H__

#include "task_save.h"
#include "master.h"
#include "save_obj_base.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {

namespace e {
namespace edit_hole_obj {

enum STATE {
	STATE_NONE,
	STATE_MOVE,
	STATE_TRANS,
	
	STATE_MAX
};

enum OKU_CATEGORY {
	OKU_CATEGORY_NORMAL,
	OKU_CATEGORY_ATTACK,

	OKU_CATEGORY_MAX
};

enum WARP2_VEC {
	WARP2_VEC_NONE,
	WARP2_VEC_UP,
	WARP2_VEC_RIGHT_UP,
	WARP2_VEC_RIGHT,
	WARP2_VEC_RIGHT_DOWN,
	WARP2_VEC_DOWN,
	WARP2_VEC_LEFT_DOWN,
	WARP2_VEC_LEFT,
	WARP2_VEC_LEFT_UP,

	WARP2_VEC_MAX
};

enum GROUND_CATEGORY {
	GROUND_CATEGORY_GREEN,
	GROUND_CATEGORY_RED,

	GROUND_CATEGORY_MAX
};

enum CATEGORY {
	CATEGORY_BALL,	// edit.h BUTTON_ID と順序が合っている必要がある
	CATEGORY_GROUND,
	CATEGORY_BUMPER,
	CATEGORY_ENEMY,
	CATEGORY_ENEMY_MUSO,
	CATEGORY_ENEMY_SPEA,

	CATEGORY_SLOPE_UP,
	CATEGORY_SLOPE_RIGHT_UP,
	CATEGORY_SLOPE_RIGHT,
	CATEGORY_SLOPE_RIGHT_DOWN,
	CATEGORY_SLOPE_DOWN,
	CATEGORY_SLOPE_LEFT_DOWN,
	CATEGORY_SLOPE_LEFT,
	CATEGORY_SLOPE_LEFT_UP,

	CATEGORY_PILLAR,

	CATEGORY_WARP,
	CATEGORY_WARP_UP_UP,
	CATEGORY_WARP_UP_RIGHT,
	CATEGORY_WARP_UP_DOWN,
	CATEGORY_WARP_UP_LEFT,
	CATEGORY_WARP_RIGHT_UP,
	CATEGORY_WARP_RIGHT_RIGHT,
	CATEGORY_WARP_RIGHT_DOWN,
	CATEGORY_WARP_RIGHT_LEFT,
	CATEGORY_WARP_DOWN_UP,
	CATEGORY_WARP_DOWN_RIGHT,
	CATEGORY_WARP_DOWN_DOWN,
	CATEGORY_WARP_DOWN_LEFT,
	CATEGORY_WARP_LEFT_UP,
	CATEGORY_WARP_LEFT_RIGHT,
	CATEGORY_WARP_LEFT_DOWN,
	CATEGORY_WARP_LEFT_LEFT,

	CATEGORY_KICK_UP,
	CATEGORY_KICK_RIGHT,
	CATEGORY_KICK_DOWN,
	CATEGORY_KICK_LEFT,
	
	CATEGORY_ENEMY_SKY,
	CATEGORY_ENEMY_MUSO_SKY,
	CATEGORY_ENEMY_SPEA_SKY,
	CATEGORY_ENEMY_KEDAMA,
	CATEGORY_ENEMY_KEDAMA_SKY,
	CATEGORY_ENEMY_HANEMEDAMA,
	CATEGORY_ENEMY_HANEMEDAMA_SKY,

	CATEGORY_DASH,
	CATEGORY_DASH_UP,
	CATEGORY_DASH_RIGHT,
	CATEGORY_DASH_DOWN,
	CATEGORY_DASH_LEFT,

	CATEGORY_JUMP,

	CATEGORY_CAMERA,
	
	CATEGORY_ENEMY_MSPA,
	CATEGORY_ENEMY_MSPA_SKY,

	CATEGORY_DAMAGE, //ダメージ床
	CATEGORY_BUNKER, //バンカー
	CATEGORY_GRAVITY, //重力エリア
	CATEGORY_DANMAKU, //弾幕

	CATEGORY_ENEMY_MOMIJI,
	CATEGORY_ENEMY_TENKO,
	CATEGORY_ENEMY_GREEN_FAIRY,
	CATEGORY_ENEMY_BLUE_FAIRY,
	CATEGORY_UNZAN,//雲山
	CATEGORY_OKU, //お空

	CATEGORY_FUKIAGE, //吹き上げエリア
	CATEGORY_CONVEYOR_UP,//コンベアー
	CATEGORY_CONVEYOR_RIGHT,
	CATEGORY_CONVEYOR_DOWN,
	CATEGORY_CONVEYOR_LEFT,
	CATEGORY_JUMP_WEAK, //小ジャンプ
	CATEGORY_JUMP_STRONG, //大ジャンプ
	CATEGORY_SE_FLOOR, //SE床

	CATEGORY_KICK_UP_RIGHT,
	CATEGORY_KICK_UP_LEFT,
	CATEGORY_KICK_DOWN_RIGHT,
	CATEGORY_KICK_DOWN_LEFT,

	CATEGORY_DASH_UP_RIGHT,
	CATEGORY_DASH_UP_LEFT,
	CATEGORY_DASH_DOWN_RIGHT,
	CATEGORY_DASH_DOWN_LEFT,

	CATEGORY_WARP2_NONE,
	CATEGORY_WARP2_UP,
	CATEGORY_WARP2_RIGHT_UP,
	CATEGORY_WARP2_RIGHT,
	CATEGORY_WARP2_RIGHT_DOWN,
	CATEGORY_WARP2_DOWN,
	CATEGORY_WARP2_LEFT_DOWN,
	CATEGORY_WARP2_LEFT,
	CATEGORY_WARP2_LEFT_UP,

	CATEGORY_ENEMY_NITORI,
	CATEGORY_ENEMY_FLAN,
	CATEGORY_ENEMY_ORIN,
	
	CATEGORY_UNZAN_REMIFLA,			//雲山レミフラ
	CATEGORY_UNZAN_MOVE,			//雲山移動
	CATEGORY_UNZAN_MOVE_REMIFLA,	//雲山移動レミフラ

	CATEGORY_TIME_ATTACK_START,		// スタート地点
	CATEGORY_TIME_ATTACK_GOAL,		// ゴール地点

	CATEGORY_MAX
};

enum IS_TOUCH {
	IS_TOUCH_NONE			=	0x0000,
	IS_TOUCH_BALL			=	0x0001,
	IS_TOUCH_PILLAR			=	0x0002,
	IS_TOUCH_LINE			=	0x0004,
	IS_TOUCH_CORNER_X1_Y1	=	0x0008,
	IS_TOUCH_CORNER_X2_Y1	=	0x0010,
	IS_TOUCH_CORNER_X1_Y2	=	0x0020,
	IS_TOUCH_CORNER_X2_Y2	=	0x0040,
	IS_TOUCH_RECT			=	0x0080,
	IS_TOUCH_WARP_X1_Y1		=	0x0100,
	IS_TOUCH_WARP_X2_Y2		=	0x0200,
	IS_TOUCH_LINE_X1_Y1		=	0x0400,
	IS_TOUCH_LINE_X2_Y2		=	0x0800,
	IS_TOUCH_MAX
};

} // namespece edit_hole_obj
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace edit {

#if 1
// クラスセーブは、タスク未対応
	class CEditHoleObj : public TASK_SAVE  {
#else
class CEditHoleObj : public TASK_SAVE {
#endif
	
public:
#if 1
// クラスセーブは、タスク未対応
	CEditHoleObj();
	CEditHoleObj( TASK_SAVE * oya);
	CEditHoleObj( TASK_SAVE * oya, nn_common::SaveManager * m);
#else
	CEditHoleObj( TASK_SAVE * oya);
#endif
	virtual ~CEditHoleObj();
	void task();
	void disp();
	void constructorCommon();
	
	void start();

	void taskTouchBall();
	void taskTouchLine();
	void taskTouchRect();

	void active();

	bool isTouch();		// そのオブジェクトをタッチしたかを返す(Categoryごと)
	void touchActive();
	int checkTouch();	// そのオブジェクトのどこをタッチしたか種類を返す(全Category共通)
	void setDispDataCommon( void * dispData);
	
#if 1
// クラスセーブは、タスク未対応
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	
	static ::nn_common::DATARECODE dataRecode[];
#endif
	static int getObjCategory2EnemyType( int category);
	static int getObjCategory2Spell( int category);

	int state;
	int oldState;
	int waitCnt;

	bool activeFlag;

	int xSlideOld;
	int ySlideOld;

// セーブメンバ
//--------------------
	int category;
	int vec;
	int x1;
	int y1;
	int x2;
	int y2;
	int z1;
	int z2;
	nn::yb::edit::CDataHole * hole;
//--------------------
	
}; // CEditHoleObj

char * gEditObjNameTbl[];
	 
} // namespece edit

} // namespace yb
} // namespace nn
	

#endif // __EDIT_HOLE_OBJ_H__
