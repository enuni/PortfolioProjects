// ENEMY を置換
// enemy を置換
// Enemy を置換
// yb を置換
// main を置換

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "task_save.h"
#include "master.h"

// 前方宣言
#include "class_list.h"


// enum
namespace nn {
namespace yb {

namespace e {
namespace enemy {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

enum TYPE {
	TYPE_BAKEBAKE,
	TYPE_BULE_FAIRY,
	TYPE_RED_FAIRY,
	TYPE_KEDAMA,
	TYPE_HANEMEDAMA,
	TYPE_MSPA,
	TYPE_MOMIJI,
	TYPE_TENKO,
	TYPE_GREEN_FAIRY,
	TYPE_BLUE_FAIRY,
	TYPE_NITORI,
	TYPE_FLAN,
	TYPE_ORIN,

	TYPE_MAX
};

} // namespece enemy
} // namespece e

namespace main {

class CEffectCtrl;
	
class CEnemy : public TASK_SAVE {
	
public:
	CEnemy( TASK_SAVE * oya);
	virtual ~CEnemy();
	void task();
	void disp();
	
	void start();

	void dead();

	static int getEnemyType2Spell( int type);

	int state;
	int oldState;
	int waitCnt;
	
	// 今の所キー処理する予定なし
	//MASTER * master;

	unsigned int cnt;

	int x;
	int y;
	int z;

	int type;

	nn::yb::stage::CHoleCtrl * holeCtrl;

	//effect
	bool bIsExist;//生存しているかどうかのフラグ
	nn::yb::main::CEffectCtrl* effectCtrl;
	static void effectCb( void * enemyArg, void * dummy);
	unsigned int ctrl_cnt;//effect個数カウンタ

	
}; // CEnemy
	 
} // namespece main
} // namespace yb
} // namespace nn
	

#endif // __ENEMY_H__
