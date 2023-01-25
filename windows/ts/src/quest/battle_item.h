// BATTLE_ITEM を置換
// battle_item を置換
// BattleItem を置換
// ts を置換
// quest を置換

#ifndef __BATTLE_ITEM_H__
#define __BATTLE_ITEM_H__

#include "task_save.h"
#include "master.h"
#include "game_char.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
extern MASTER * master;
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace battle_item {

enum STATE {
	STATE_NONE,
	STATE_ACTIVE,
	
	STATE_MAX
};

} // namespece battle_item
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace quest {

typedef struct _battle_item_disp_member {
	int ** objIdRecoveryItems;
} BATTLE_ITEM_DISP_MEMBER;

typedef struct _battle_item_link_member {
	nn::ts::quest::CBattle * battle;
} BATTLE_ITEM_LINK_MEMBER;

typedef struct _battle_item_param {
	int id;
	int levelMax;
	int levelUpTurns[8];
} BATTLE_ITEM_PARAM;

typedef struct _battle_item_status {
	int level;
	int levelTurn;
} BATTLE_ITEM_STATUS;

typedef struct _battle_item_game_member {
	nn::ts::quest::BATTLE_ITEM_PARAM param;
	nn::ts::quest::BATTLE_ITEM_STATUS status;
} BATTLE_ITEM_GAME_MEMBER;

typedef struct _battle_item_member {
	nn::ts::quest::BATTLE_ITEM_LINK_MEMBER link;
	nn::ts::quest::BATTLE_ITEM_GAME_MEMBER game;
} BATTLE_ITEM_MEMBER;
	
class CBattleItem : public nn::common::CGameChar {
	
public:
	CBattleItem();
	CBattleItem( TASK_SAVE * oya);
	CBattleItem( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CBattleItem();
	void constructCommon();					// メンバー0クリアなどnewしたらconstructorで必ずやる事
	void task();
	void disp();
	
	void setMember( void * owner);			// 自分を作ったTASKから必要なパラメータをもらう void *型は宣言から変えても良いし、関数内で型変換しても良い
	void setMemberLoad( void * owner);		// setMember()のロード時版、ロードして不要なものはセットしない
	void setMemberCommon( void * owner);	// setMember()とsetMemberLoad()の共通処理
	void start();							// setMember()後にやるTask立ち上げ処理
	void startLoad();						// start()のロード時版
	void startCommon();						// start()とstartLoad()の共通処理
	void dump();								// メンバの値をデバッグ出力
	
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void hitBall( nn::ts::quest::CBall * ball);
	void updateTurn();
	
	
	int state;
	int oldState;
	int waitCnt;
	
	unsigned int frameCnt;
	
	static nn_common::DATARECODE dataRecode[];
	
	nn::ts::quest::BATTLE_ITEM_MEMBER m;
	static nn::ts::quest::BATTLE_ITEM_DISP_MEMBER dm;
	
}; // CBattleItem
	 
} // namespece quest

} // namespace ts
} // namespace nn
	

#endif // __BATTLE_ITEM_H__
