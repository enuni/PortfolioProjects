//TS
//Ts
//ts

#ifndef __TS_SAVE_MANAGER_H__
#define __TS_SAVE_MANAGER_H__

#include "save_manager.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace ts {
namespace e {
namespace ts_save_manager {
	
enum {
	CLASS_ID_QUEST = nn_common::e::save_manager::CLASS_ID_COMMON_END,
	CLASS_ID_ENEMY,
	CLASS_ID_BATTLE,
	CLASS_ID_BALL,
	CLASS_ID_PULL,
	CLASS_ID_BATTLE_BG,
	CLASS_ID_QUEST_CONTROL,
	CLASS_ID_BATTLE_ITEM,
	CLASS_ID_END
};
	
} // namespace ts_save_manager
} // namespace e
//---------------------------------------------------------------------------
// classs
namespace common {
class CTsSaveManager : public ::nn_common::SaveManager {

public:
	::nn_common::SaveObjBase * createObj( int classId);
	void ClearObjVector( void);
};

} //namespace common

} //namespace ts
} //namespace nn

#endif //__TS_SAVE_MANAGER_H__