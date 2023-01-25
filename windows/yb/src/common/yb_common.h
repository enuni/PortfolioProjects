#ifndef __YB_COMMON_H__
#define __YB_COMMON_H__

#include "task_save.h"
#include "master.h"
#include "nn_common.h"
#include "nn_common2.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {

namespace e {

namespace button {

enum STATE {
	STATE_NONE,
	STATE_ON,
	STATE_OFF,
	STATE_HIDE,
	
	STATE_MAX
};

} // namespace button

namespace dialog {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

} // namespece dialog

} // namespace e

//---------------------------------------------------------------------------
// class
namespace common {

void yb_commonDispGround( int objId, int x1, int y1, int x2, int y2, int z, int wUnit, int hUnit);
void yb_commonDispGroundEx( int objId, int x1, int y1, int x2, int y2, int z, int wUnit, int hUnit,
										   BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
);
void yb_commonDispGroundScale( int objId, int x1, int y1, int x2, int y2, int z, int wUnit, int hUnit,
							  int xCenter, int yCenter, double hScale, double vScale,
							  BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
);
void yb_commonDispObjRepeatScale( int objId, int x1, int y1, int x2, int y2, int z,
								 int xCenter, int yCenter, double hScale, double vScale,
								 BOOL flipH, BOOL flipV, GXRgb rgb, u16 alpha, int blend
);

MASTER * getMaster();

class CButton : public TASK_SAVE {
	
public:
	CButton( TASK_SAVE * oya);
	virtual ~CButton();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	char caption[32];
	int x;
	int y;
	int w;
	int h;
	void (* onCb)( void *, void *);
	void * onCbArg;

	int id;
	bool enableFlag;

	GXRgb color;
	GXRgb darkColor;
	
}; // CButton

class CDialog : public TASK_SAVE {
	
public:
	CDialog( TASK_SAVE * oya);
	virtual ~CDialog();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int id;
	int x;
	int y;
	int w;
	int h;
	char msg[256];
	char buttonCaption[8][32];
	int buttonN;
	int buttonNo;
	void (* onCb)( void *, void *);
	void * onArg;

	nn::common::def::graphics2d::RECT button[8];

	int cursorX;
	
}; // CDialog

typedef struct _disp_score_list_data {
	int x;
	int y;
	int z;
	int holeCnt;
	int score[8];
	int shotCnt;
	int totalScore;
	bool nowHoleDispFlag;
	bool totalDispFlag;
} DISP_SCORE_LIST_DATA;

void yb_commonDispScoreList( DISP_SCORE_LIST_DATA * data);

void yb_commonPlaySeq( int seqNo);

typedef struct _calc_ball_data {
	double * zSpeed;
	bool groundSideFlag;
	double * z;
	double * angle;
	double * rotHorizon;
	double * rotVertical;
	double * speed;
	bool groundFlag;
	bool * groundOnFlag;
	bool * flyBoundFlag;
	// 110412
	bool cupFlag;
	bool * cupinFlag;
	double * addZ;
	// 120521
	int ballForm;
} CALC_BALL_DATA;

typedef struct _calc_shot_data {
	double * zSpeed;
	double * rotHorizon;
	double * rotVertical;
	double * speed;

	int powerState;
	int flyState;
	int cursorSpinHrizon;
	double power;
	int stopSpinVertical;
} CALC_SHOT_DATA;

typedef struct _calc_move_data {
	double * speed;
	double angle;
	double * x;
	double * y;
	double z;
	double * addX;
	double * addY;
	bool flyBoundFlag;
	double zSpeed;
	// 120521
	int ballForm;
} CALC_MOVE_DATA;

typedef struct _calc_spin_data {
	double * angle;
	double * spinHorizon;
} CALC_SPIN_DATA;

typedef struct _calc_bumper_data {
	nn::common::def::graphics2d::LINE * paraBumper;			// 変更されない
	nn::common::def::graphics2d::SEGMENT2_D * paraMove;		// 変更されない
	double paraZ;
	int paraBumperZ;

	bool * returnResult;										// 変更されるかも
	double * returnAngle;										// 変更されるかも
	double * returnSpeed;										// 変更されるかも
	double * returnX;											// 変更されるかも
	double * returnY;											// 変更されるかも
	nn::common::def::graphics2d::VECTOR2_D * returnCollision;	// 変更されるかも
} CALC_BUMPER_DATA;

typedef struct _disp_hole_obj_data {
	int category;
	int vec;
	int subCategory;
	int x1;
	int x2;
	int y1;
	int y2;
	int z1;
	int z2;
	int dispZ;
	GXRgb color;
	double hScale;
	double vScale;
	int sceneNo;

	int centerX;
	int centerY;

	unsigned int frameCnt;
} DISP_HOLE_OBJ_DATA;

void yb_commonCalcDown( CALC_BALL_DATA * data);
void yb_commonCalcShot( CALC_SHOT_DATA * data);
void yb_commonCalcMove( CALC_MOVE_DATA * data);
void yb_commonCalcSpin( CALC_SPIN_DATA * data);
void yb_commonCalcBumper( CALC_BUMPER_DATA * data);

void yb_commonAddAngle( double * angle, double add);

void yb_commonDispMsgBox( int x, int y, int z, int w, int h, int objIdEdge, GXRgb color, int alpha);

void yb_commonDispHoleObj( DISP_HOLE_OBJ_DATA * d);

void yb_commonMoveHomingBullet(
	double& x, double& y,    // 弾の座標
	double& vx, double& vy,  // 弾の速度
	double mx, double my,    // 自機の座標
	double speed,           // 弾の速さ
	double theta            // 旋回角度の上限
);

bool yb_commonIsSkip( void * masterArg);

double yb_commonAngleSpeed2MoveX( double angle, double speed);
double yb_commonAngleSpeed2MoveY( double angle, double speed);

double yb_commonVec2angle( nn::common::def::graphics2d::VECTOR2_D * vec0, nn::common::def::graphics2d::VECTOR2_D * vec);
double yb_commonVec2angle( double x, double y);

} // namespece common
} // namespece yb
} // namespece nn

#endif //__YB_COMMON_H__

