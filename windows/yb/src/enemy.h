// ENEMY ��u��
// enemy ��u��
// Enemy ��u��
// yb ��u��
// main ��u��

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "task_save.h"
#include "master.h"

// �O���錾
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
	
	// ���̏��L�[��������\��Ȃ�
	//MASTER * master;

	unsigned int cnt;

	int x;
	int y;
	int z;

	int type;

	nn::yb::stage::CHoleCtrl * holeCtrl;

	//effect
	bool bIsExist;//�������Ă��邩�ǂ����̃t���O
	nn::yb::main::CEffectCtrl* effectCtrl;
	static void effectCb( void * enemyArg, void * dummy);
	unsigned int ctrl_cnt;//effect���J�E���^

	
}; // CEnemy
	 
} // namespece main
} // namespace yb
} // namespace nn
	

#endif // __ENEMY_H__
