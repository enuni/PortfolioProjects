//Ts
//Ts
//ts

#include <stdio.h>

#include "disp_3d_bg.h"
#include "anim.h"

#include "ball.h"
#include "battle.h"
#include "enemy.h"
#include "quest.h"
#include "pull.h"
#include "battle_bg.h"
#include "quest_control.h"
#include "battle_item.h"

#include "ts_save_manager.h"
//---------------------------------------------------------------------------
::nn_common::SaveObjBase * ::nn::ts::common::CTsSaveManager::createObj( int classId) {

	switch(classId) {
	case nn::ts::e::ts_save_manager::CLASS_ID_BALL:
		return new nn::ts::quest::CBall();
	case nn::ts::e::ts_save_manager::CLASS_ID_BATTLE:
		return new nn::ts::quest::CBattle();
	case nn::ts::e::ts_save_manager::CLASS_ID_ENEMY:
		return new nn::ts::quest::CEnemy();
	case nn::ts::e::ts_save_manager::CLASS_ID_QUEST:
		return new nn::ts::quest::CQuest();
	case nn::ts::e::ts_save_manager::CLASS_ID_PULL:
		return new nn::ts::quest::CPull();
	case nn_common::e::save_manager::CLASS_ID_COMMON_DISP_3D_BG:
		return new nn::dxlib::disp_3d_bg::CDisp3dBg();
	case nn::ts::e::ts_save_manager::CLASS_ID_BATTLE_BG:
		return new nn::ts::quest::CBattleBg();
	case nn_common::e::save_manager::CLASS_ID_COMMON_ANIME:
		return new nn::dxlib::CAnim();
	case nn::ts::e::ts_save_manager::CLASS_ID_QUEST_CONTROL:
		return new nn::ts::quest::CQuestControl();
	case nn::ts::e::ts_save_manager::CLASS_ID_BATTLE_ITEM:
		return new nn::ts::quest::CBattleItem();
	}

	return NULL;

}
//---------------------------------------------------------------------------
void ::nn::ts::common::CTsSaveManager::ClearObjVector( void) {

	this->saveObjVector.clear();

}
//---------------------------------------------------------------------------