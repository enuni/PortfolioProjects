// SPELL_GET を置換
// spell_get を置換
// SpellGet を置換
// yb を置換
// effect を置換

#ifndef __SPELL_GET_H__
#define __SPELL_GET_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace yb {

// enum
namespace e {
namespace spell_get {

enum STATE {
	STATE_NONE,
	STATE_INCIDENCE,
	STATE_MOVE_CARD,
	STATE_HIT_CARD,
	STATE_MOVE_KEY,
	STATE_HIT_KEY,
	
	STATE_MAX
};

} // namespece spell_get
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace effect {
	
class CSpellGet : public TASK_SAVE {
	
public:
	CSpellGet( TASK_SAVE * oya);
	CSpellGet( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CSpellGet();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	static void hitCardCb( void * spellGetArg);
	void hitCard();
	static void hitKeyCb( void * spellGetArg);
	void hitKey();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;
	
	double x;
	double y;
	double vx;
	double vy;

	double dstX;
	double dstY;
	double speed;
	double limitAngle;

	int frameCnt;
	int dispAngle;

	void (*hitCardCbCb)( void *);
	void * hitCardCbCbArg;
	int hitCardEffectState;
	void (*hitKeyCbCb)( void *);
	void * hitKeyCbCbArg;
	int hitKeyEffectState;

	int objIdSpellCardIcon;

	
}; // CSpellGet
	 
} // namespece effect

} // namespace yb
} // namespace nn
	

#endif // __SPELL_GET_H__
