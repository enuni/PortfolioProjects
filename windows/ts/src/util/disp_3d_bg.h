// DISP_3D_BG を置換
// disp_3d_bg を置換
// Disp3dBg を置換
// dxlib を置換
// disp_3d_bg を置換

#ifndef __DISP_3D_BG_H__
#define __DISP_3D_BG_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace dxlib {

// enum
namespace e {
namespace disp_3d_bg {

enum STATE {
	STATE_NONE,
	STATE_IDLE,
	STATE_DIRECT,
	
	STATE_MAX
};

enum POLYGON {
	POLYGON_MAX = 16
};

} // namespece disp_3d_bg
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace disp_3d_bg {

typedef struct _polygon_var_member {
	float size;			// XZ軸に+-size/2を頂点にする(target指定
	float uvRate;		// テクスチャ拡大率(0.5から+-0.5*uvRateをUV座標にする(target指定
	float r;			// Red(頂点カラー4つまとめて(target指定
	float g;			// Green(頂点カラー4つまとめて(target指定
	float b;			// Blue(頂点カラー4つまとめて(target指定
	float a;			// Alpha(頂点カラー4つまとめて(target指定
	float y;			// Y軸位置(target指定
	float u;			// U座標(target指定
	float v;			// V座標(target指定
	float varShiftU;	// U軸スクロール(1フレーム移動量指定(uvRateの影響は受けない
	float varShiftV;	// V軸スクロール(1フレーム移動量指定(uvRateの影響は受けない
} POLYGON_VAR_MEMBER;

typedef struct _polygon_member {
	int handle;
	float shiftU;
	float shiftV;
	POLYGON_VAR_MEMBER now;
	POLYGON_VAR_MEMBER target;
} POLYGON_MEMBER;

typedef struct _scene_var_member {
	float fogStart;
	float fogEnd;
	float cameraPositionX;
	float cameraPositionY;
	float cameraPositionZ;
	float cameraTargetX;
	float cameraTargetY;
	float cameraTargetZ;
} SCENE_VAR_MEMBER;

typedef struct _bg_member {
	SCENE_VAR_MEMBER now;
	SCENE_VAR_MEMBER target;
} SCENE_MEMBER;

typedef struct _disp_3d_bg_link_member {
	void (*directEndCBFunc)(void *);
	void * directEndCBFuncArg;
	int directEndCBFuncId;
	void * directEndCBFuncOwner;
} DISP_3D_BG_LINK_MEMBER;

typedef struct _disp_3d_bg_ctrl_member {
	int acc;
	int frame;
	int frameN;
	SCENE_MEMBER scene;
	POLYGON_MEMBER polygons[nn::dxlib::e::disp_3d_bg::POLYGON_MAX];
	int polygonN;
} DISP_3D_BG_CTRL_MEMBER;

typedef struct _disp_3d_bg_member {
	DISP_3D_BG_CTRL_MEMBER cm;
	DISP_3D_BG_LINK_MEMBER lm;
} DISP_3D_BG_MEMBER;
	
class CDisp3dBg : public TASK_SAVE {
	
public:
	CDisp3dBg();
	CDisp3dBg( TASK_SAVE * oya);
	CDisp3dBg( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CDisp3dBg();
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

	void initDirect();
	void setDirectScene( nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER * target);
	void setDirectPolygon( int index, nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER * target);
	void startDirect( int acc, int frame);

	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	MASTER * master;

	static nn_common::DATARECODE dataRecode[];

	DISP_3D_BG_MEMBER m;
	
}; // CDisp3dBg
	 
} // namespece disp_3d_bg

} // namespace dxlib
} // namespace nn
	

#endif // __DISP_3D_BG_H__
