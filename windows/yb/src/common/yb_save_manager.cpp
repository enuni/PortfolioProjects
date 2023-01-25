#include <stdio.h>

#include "data_hole.h"
#include "data_hole_100.h"
#include "edit_hole_obj.h"
#include "save_data.h"
#include "data_hole_101.h"

#include "yb_save_manager.h"
//---------------------------------------------------------------------------
::nn_common::SaveObjBase * ::nn::yb::common::CYbSaveManager::createObj( int classId) {

	switch(classId) {
		case ::nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE:
			return new nn::yb::edit::CDataHole();
		case ::nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_OBJ:
			return new nn::yb::edit::CDataHoleObj();
#if 1
// クラスセーブは、タスク未対応
		case nn::yb::e::yb_save_manager::CLASS_ID_EDIT_HOLE_OBJ: 
			return new nn::yb::edit::CEditHoleObj();
#endif
		case ::nn::yb::e::yb_save_manager::CLASS_ID_SAVE_DATA:
			return new nn::yb::main::CSaveData();
		case ::nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_100:
			// Ver.100との差異がないので最新版を返す。
			return new nn::yb::edit::CDataHole_100();
		case ::nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_OBJ_100:
			return new nn::yb::edit::CDataHoleObj_100();
		case ::nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_101:
			return new nn::yb::edit::CDataHole_101();
		case ::nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_OBJ_101:
			return new nn::yb::edit::CDataHoleObj_101();
	}

	return NULL;

}
//---------------------------------------------------------------------------
void ::nn::yb::common::CYbSaveManager::ClearObjVector( void) {

	this->saveObjVector.clear();

}
//---------------------------------------------------------------------------