#ifndef __YB_SAVE_MANAGER_H__
#define __YB_SAVE_MANAGER_H__

#include "save_manager.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {
namespace e {
namespace yb_save_manager {
	
enum {
	CLASS_ID_DATA_HOLE_100,
	CLASS_ID_DATA_HOLE_OBJ_100,
	CLASS_ID_EDIT_HOLE_OBJ,
	CLASS_ID_SAVE_DATA,
	CLASS_ID_DATA_HOLE_101,
	CLASS_ID_DATA_HOLE_OBJ_101,
	CLASS_ID_DATA_HOLE,
	CLASS_ID_DATA_HOLE_OBJ,

	CLASS_ID_END
};
	
} // namespace yb_save_manager
} // namespace e
//---------------------------------------------------------------------------
// classs
namespace common {
class CYbSaveManager : public ::nn_common::SaveManager {

public:
	::nn_common::SaveObjBase * createObj( int classId);
	void ClearObjVector( void);
};

} //namespace common

} //namespace yb
} //namespace nn

#endif //__YB_SAVE_MANAGER_H__