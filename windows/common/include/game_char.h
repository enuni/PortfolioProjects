#ifndef __GAME_CHARACTER_H__
#define __GAME_CHARACTER_H__

#include "task_save.h"

namespace nn {
namespace common {

namespace e {
namespace game_char {

enum HIT_TYPE {
	HIT_TYPE_CIRCLE,
	HIT_TYPE_RECT,

	HIT_TYPE_MAX
};

} //namespace game_char
} //namespace e

typedef struct _game_char_hit_double {
	double x;
	double y;
	double w;
	double h;
	double r;
	nn::common::e::game_char::HIT_TYPE hitType;
} GAME_CHAR_HIT_DOUBLE;

typedef struct _game_char_game_member {
	nn::common::GAME_CHAR_HIT_DOUBLE hit;
	bool enableFlag;
} GAME_CHAR_GAME_MEBMER;

typedef struct _game_char_member {
	nn::common::GAME_CHAR_GAME_MEBMER game;
} GAME_CHAR_MEBMER;

class CGameChar : public TASK_SAVE {
	
public:
	CGameChar();
	CGameChar( ::nn_common::SaveManager * m);
	CGameChar( ::TASK_SAVE * parant);
	CGameChar( ::TASK_SAVE * parant, ::nn_common::SaveManager * m);
	virtual ~CGameChar();
	void constructCommon();
	
protected:
	static ::nn_common::DATARECODE dataRecode[];
	
public:
	nn::common::GAME_CHAR_MEBMER gameChar;

	nn::common::CGameChar * next;
	nn::common::CGameChar * prev;
	
};

} // namaspace common
} // namespace nn

#endif //__GAME_CHARACTER_H__
