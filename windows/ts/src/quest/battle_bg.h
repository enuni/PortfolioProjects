// BATTLE_BG を置換
// battle_bg を置換
// BattleBg を置換
// ts を置換
// quest を置換

#ifndef __BATTLE_BG_H__
#define __BATTLE_BG_H__

#include "task_save.h"
#include "master.h"
#include "disp_3d_bg.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace battle_bg {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

enum DIRECT {
	DIRECT_MAX = 16
};

} // namespece battle_bg
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct  _battle_bg_direct_member {
	nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER scene;													// CameraやFogの移動するScene情報
	nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER polygons[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int polygonIndexTbl[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int polygonN;
	bool sceneFlag;																					// CameraやFogの移動するScene情報の有無
	int frame;
	int acc;
} BATTLE_BG_DIRECT_MEMBER;

typedef struct _battle_bg_link_member {
	void (*directEndCBFunc)(void *);
	void * directEndCBFuncArg;
	int directEndCBFuncId;
	void * directEndCBFuncOwner;
	nn::dxlib::disp_3d_bg::CDisp3dBg * disp3dBg;
} BATTLE_BG_LINK_MEMBER;

typedef struct _battle_bg_ctrl_member {
	nn::ts::quest::BATTLE_BG_DIRECT_MEMBER dm[nn::ts::e::battle_bg::DIRECT_MAX];
	int dmN;
	int dmIndex;
	nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER initScene;
	nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER initPolygons[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int initPolygonHandles[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int initPolygonN;
	int battleIndex;																					// 演出開始時にインクリメントする バトル1の演出時=1
} BATTLE_BG_CTRL_MEMBER;

typedef struct _battle_bg_member {
	nn::ts::quest::BATTLE_BG_CTRL_MEMBER cm;
	nn::ts::quest::BATTLE_BG_LINK_MEMBER lm;
} BATTLE_BG_MEMBER;
	
class CBattleBg : public TASK_SAVE {
	
public:
	CBattleBg();
	CBattleBg( TASK_SAVE * oya);
	CBattleBg( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CBattleBg();
	void constructCommon();					// メンバー0クリアなどnewしたらconstructorで必ずやる事
	void task();
	void disp();
	
	void setMember( void * owner);			// 自分を作ったTASKから必要なパラメータをもらう void *型は宣言から変えても良いし、関数内で型変換しても良い
	void setMemberLoad( void * owner);		// setMember()のロード時版、ロードして不要なものはセットしない
	void setMemberCommon( void * owner);	// setMember()とsetMemberLoad()の共通処理
	void start();							// setMember()後にやるTask立ち上げ処理
	void startLoad();						// start()のロード時版
	void startCommon();						// start()とstartLoad()の共通処理
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	
	static void nextDirectCB( void * argBattleBg);
	void nextDirect();
	void startDirect();

	void dump();

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;

	static nn_common::DATARECODE dataRecode[];

	nn::ts::quest::BATTLE_BG_MEMBER m;
	
}; // CBattleBg
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __BATTLE_BG_H__
