// HOLE を置換
// hole を置換
// Hole を置換
// yb を置換
// stage を置換

#ifndef __HOLE_H__
#define __HOLE_H__

#include "task_save.h"
#include "master.h"
#include "nn_common2.h"

//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {

namespace def {

typedef struct _hole_obj {
	int category;

	int x1;
	int y1;
	int z1;
	int x2;
	int y2;
	int z2;

	int frameCnt;
	int state;

	int vec;

	int onFlag;

	nn::common::def::graphics2d::DDA_DATA dda;
} HOLE_OBJ;

typedef struct _hole_data {

	nn::yb::def::HOLE_OBJ holeObj[1024];
	int holeObjN;
	int w;
	int h;

} HOLE_DATA;

namespace hole {

} // namespace hole

} // namesspace def

namespace e {
namespace hole {

/*enum OBJ_CATEGORY {
	OBJ_CATEGORY_GROUND,
	OBJ_CATEGORY_BUMPER,

	OBJ_CATEGORY_MAX
};
*/

enum OKU_STATE {
	OKU_STATE_NORMAL,
	OKU_STATE_ATTACK_BEFORE,
	OKU_STATE_ATTACK,

	OKU_STATE_MAX
};

enum UNZAN_STATE {
	UNZAN_STATE_WENT_BEFORE,
	UNZAN_STATE_WENT,
	UNZAN_STATE_COMEBACK_BEFORE,
	UNZAN_STATE_COMEBACK,

	UNZAN_STATE_MAX
};

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

} // namespece hole
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace stage {
	
class CHole : public TASK_SAVE {
	
public:
	CHole( TASK_SAVE * oya);
	virtual ~CHole();
	void task();
	void disp();
	
	void start();

	void defeatedEnemy( void * enemy);
	void lastEnemy2Cup();
	void dispGround( nn::yb::def::HOLE_OBJ * obj);
	void setHole( char * buf, int size);
	void setHole( char * dataFileName);
	void setHole( nn::yb::edit::CDataHole * data);

	static void lastEnemy2CupExecCb( void * argHole);
	void lastEnemy2CupCbExec();
	static void lastEnemy2CupAfterCb( void * argHole);
	void lastEnemy2CupAfter();
	void dispWarp( nn::yb::def::HOLE_OBJ * obj);
	void setDispDataCommon( void * dispData, void * obj);
	void taskOku( nn::yb::def::HOLE_OBJ * obj);
	void taskUnzanMove( nn::yb::def::HOLE_OBJ * obj);

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	int par;

	nn::yb::stage::CCup * cup;

	nn::yb::main::CEnemy * enemy[16];

	nn::common::def::graphics2d::LINE line[16];
	int lineN;

	int enemyN;

	nn::yb::stage::CHoleCtrl * holeCtrl;

	nn::yb::def::HOLE_DATA data;

	nn::yb::main::CEnemy * enemyLast;
	int stateLastEnemy2Cup;

	nn::yb::common::CYbSaveManager * saveManager;

	int openingCnt;
	int openingX[128];
	int openingY[128];

	int warpDispCnt;

	int startX;
	int startY;
	int endX;
	int endY;

	unsigned int frameCnt;
	
}; // CHole
	 
} // namespece stage

} // namespace yb
} // namespace nn
	

#endif // __HOLE_H__
