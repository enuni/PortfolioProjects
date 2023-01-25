#ifndef __TS_COMMON_H__
#define __TS_COMMON_H__

#include "class_list.h"
#include "nn_common.h"
#include "nn_common2.h"

namespace nn {
namespace ts {

namespace e {
namespace common {

enum CRC_KEY {
	CRC_KEY_DEBUG_QUEST		= 0xF1E5,
};

enum HIT_TYPE {
	HIT_TYPE_CIRCLE,
	HIT_TYPE_RECT,

	HIT_TYPE_MAX
};

enum COLLISION_ID {
	COLLISION_ID_LINE,
	COLLISION_ID_ENEMY,
	COLLISION_ID_BALL,
	COLLISION_ID_ITEM,

	COLLISION_ID_MAX
};

enum CHAR_ID {
	CHAR_ID_NONE,
	CHAR_ID_REIMU,
	CHAR_ID_MARISA,
	CHAR_ID_SANAE,
	CHAR_ID_YOMU,

	CHAR_ID_MAX
};

enum ENEMY_ID {
	ENEMY_ID_NONE,
	ENEMY_ID_RUMIA,
	ENEMY_ID_YOSEIAO,
	ENEMY_ID_YOSEIMIDORI,
	ENEMY_ID_OKUU,
	
	ENEMY_ID_MAX
};

enum BATTLE_ITEM_ID {
	BATTLE_ITEM_ID_NONE,
	BATTLE_ITEM_ID_RECOVERY,

	BATTLE_ITEM_ID_MAX
};

enum BATTLE_ITEM_LEVEL_N {
	BATTLE_ITEM_LEVEL_N_NONE		= 0,
	BATTLE_ITEM_LEVEL_N_RECOVERY	= 3,

	BATTLE_ITEM_LEVEL_N_MAX
};

enum CONST_N {
	QUEST_BALL_N =  4,
	BALL_R       = 21,

	CONST_N_MAX
};

enum HIT_TARGET_BIT {
	HIT_TARGET_BIT_ENEMY  = 0x01,
	HIT_TARGET_BIT_FRIEND = 0x02,
	HIT_TARGET_BIT_BALL   = 0x04,

	HIT_TARGET_BIT_MAX
};

enum Z_TITLE {
	Z_TITLE_TEXT			=  39,
	Z_TITLE_BG				= 500,

	Z_TITLE_MAX
};

enum Z_COMMON {
	Z_COMMON_DEBUG_MENU_TEXT	=  20,
	Z_COMMON_DEBUG_MENU_BG		=  21,

	Z_COMMON_MAX,
};

enum Z_BATTLE {
	Z_BATTLE_TEXT           =  39,
	Z_BATTLE_TEXT_BG        =  40,
	Z_BATTLE_IF             =  50,
	Z_BATTLE_FIELD_OUT_MASK =  60,
	Z_BATTLE_DAMAGE_DISP    = 150,
	Z_BATTLE_EFFECT         = 170,
	Z_BATTLE_ARROW			= 190,
	Z_BATTLE_BALL_TURN      = 200,
	Z_BATTLE_ITEM			= 210,
	Z_BATTLE_ENEMY_TURN     = 230,
	Z_BATTLE_ENEMY          = 250,
	Z_BATTLE_BALL_WAIT      = 240,
	Z_BATTLE_BATTLE_FRAME   = 300,
	Z_BATTLE_FIELD_BG       = 500,

	Z_BATTLE_MAX
};

enum ATTRIBUTE {
	ATTRIBUTE_FIRE,
	ATTRIBUTE_WATER,
	ATTRIBUTE_WOOD,
	ATTRIBUTE_LIGHT,
	ATTRIBUTE_DARK,

	ATTRIBUTE_MAX
};

enum CB_FUNC_ID {
	CB_FUNC_ID_NONE,
	CB_FUNC_ID_PULL_DECIDE,
	CB_FUNC_ID_END_ATTACK_ENEMY,
	CB_FUNC_ID_NEXT_MOVE_QUEST,
	CB_FUNC_ID_NEXT_DIRECT_BATTLE_BG,

	CB_FUNC_ID_MAX
};

enum HIT_CASE {
	HIT_CASE_BALL_2_ENMEY,
	HIT_CASE_SHOT_2_ENMEY,

	HIT_CASE_MAX
};

enum BATTLE_FIELD {
	BATTLE_FIELD_X = 1173,
	BATTLE_FIELD_Y = 1030,
	BATTLE_FIELD_W = 540,
	BATTLE_FIELD_H = 420,

	BATTLE_FIELD_MAX
};

enum LOG_DST_INDEX {
	LOG_DST_INDEX_CLASS_DUMP = nn::common::e::LOG_DST_INDEX_COMMON_MAX,

	LOG_DST_INDEX_MAX
};

enum DEBUG_MENU_COMMAND {
	DEBUG_COMMAND_SAVE,
	DEBUG_COMMAND_LOAD,
	DEBUG_COMMAND_UNDO,
	DEBUG_COMMAND_REDO,
	DEBUG_COMMAND_BATTLE_NEXT,
	DEBUG_COMMAND_QUEST_RESTART,
	DEBUG_COMMAND_END,

	DEBUG_COMMAND_MAX
};

#define NN_TS_COMMON_E_LOG_KIND_CLASS_DUMP	(0x0000000000000100)
#define NN_TS_COMMON_E_LOG_KIND_XML			(0x0000000000000200)
#define LOG_CLASS_DUMP(...)		nn::common::log( nn::ts::e::common::LOG_DST_INDEX_CLASS_DUMP, \
									NN_TS_COMMON_E_LOG_KIND_CLASS_DUMP,__VA_ARGS__)
#define LOG_XML(...)		nn::common::log( -1, \
									NN_TS_COMMON_E_LOG_KIND_XML,__VA_ARGS__)

} // namespace common
} // namespace e

namespace common {
	
typedef struct _hit {
	nn::ts::e::common::HIT_TYPE hitType;
	int x;
	int y;
	int w;
	int h;
	int r;
} HIT;

typedef struct _ts_hit {
	bool weakPointFlag;
	bool weakPointOnFlag;
	nn::ts::common::HIT hit;
	void * owner;
} TS_HIT;

typedef struct _attacker_info {
	nn::ts::e::common::HIT_CASE hitCase;
	int damage;
} ATTACKER_INFO;

extern const char gCharactersSaveDataDebugQuest[];

void setDispMember();
void setDispMemberBall();
void setDispMemberEnemy();
void setDispMemberPull();
void setDispMemberDamageDisp();
void setDispMemberQuest();
void setDispMemberMasterSpark();
void setDispMemberLaser();
void setDispMemberBullet();
void setDispMemberTitle();
void setDIspMemverBattleItem();

void setCallBackFunc();

void hit( nn::common::util::type::t2d::COLLISION_ITEM * ci,
		  nn::ts::common::ATTACKER_INFO * ai
);

} // namespace common
} // namespace ts
} // namespace nn

#endif // __TS_COMMON_H__
