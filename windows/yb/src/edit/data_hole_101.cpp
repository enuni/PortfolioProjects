
#include "edit_hole_obj.h"

#include "save_manager.h"
#include "data_hole.h"
#include "yb_save_manager.h"
#include "data_hole_101.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::edit::CDataHole_101::dataRecode[] = {
	DATA_LOCAL(	nn::yb::edit::CDataHole_101,	nn::yb::edit::CDataHole_101::w),
	DATA_LOCAL(	nn::yb::edit::CDataHole_101,	nn::yb::edit::CDataHole_101::h),
	DATA_LOCAL(	nn::yb::edit::CDataHole_101,	nn::yb::edit::CDataHole_101::objCnt),
	DATA_PTR( 	nn::yb::edit::CDataHole_101,	nn::yb::edit::CDataHole_101::dataObj),
	DATA_PTR( 	nn::yb::edit::CDataHole_101,	nn::yb::edit::CDataHole_101::obj),
	DATA_BASE( ::TASK_SAVE::dataRecode ),
	DATA_END,
};
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole_101::CDataHole_101() : ::TASK_SAVE() {
	
	printf( "CDataHole_101() Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole_101::CDataHole_101( ::nn_common::SaveManager * m) : ::TASK_SAVE( m) {
	
	printf( "CDataHole_101( SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole_101::CDataHole_101( ::TASK_SAVE * parent, ::nn_common::SaveManager * m) : ::TASK_SAVE( parent, m) {
	
	printf( "CDataHole_101( TASK_SAVE, SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void ::nn::yb::edit::CDataHole_101::constructorCommon() {

	memset( this->obj, 0, sizeof( this->obj));
	memset( this->dataObj, 0, sizeof( this->dataObj));
	this->objCnt = 0;
	this->w = 0;
	this->h = 0;
	this->windowW = 0;
	this->windowH = 0;
	this->edit = NULL;

}
//---------------------------------------------------------------------------
::nn_common::DATARECODE * ::nn::yb::edit::CDataHole_101::getDataRecode() {

	return ::nn::yb::edit::CDataHole_101::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::edit::CDataHole_101::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_101;

}
//---------------------------------------------------------------------------
void ::nn::yb::edit::CDataHole_101::afterUpdate() {

#if 0
	// 121130～不要になったはず
	for( int i = 0; i < this->objCnt; i++) {

		if(this->obj[i] == NULL) {
			nn::yb::edit::CDataHoleObj_101 * load_obj = this->dataObj[i];
#if 1
			// クラスセーブは、タスク未対応
			nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this, this->saveManager);
#else
			nn::yb::edit::CEditHoleObj * obj = new nn::yb::edit::CEditHoleObj( this);
#endif
			obj->category = load_obj->category;
			obj->x1 = load_obj->x1;
			obj->y1 = load_obj->y1;
			obj->x2 = load_obj->x2;
			obj->y2 = load_obj->y2;
			obj->hole = this;
			obj->start();
			this->obj[i] = obj;
			delete load_obj;
			this->dataObj[i] = NULL;
		}

	}
#endif
}
//---------------------------------------------------------------------------
::nn_common::SaveObjBase * nn::yb::edit::CDataHole_101::update() {

	printf( "CDataHole_101::update\n");
	nn::yb::edit::CDataHole * hole = new nn::yb::edit::CDataHole();

	hole->windowW = this->w;
	hole->windowH = this->h;
	hole->w = this->w;
	hole->h = this->h;
	hole->objCnt = this->objCnt;
	::memset( hole->obj, -1, sizeof( hole->obj));
	::memcpy( hole->obj, this->obj, sizeof( this->obj));

	hole->parent = this->parent;
	hole->childTop = this->childTop;
	hole->childBottom = this->childBottom;
	hole->brotherUp = this->brotherUp;
	hole->brotherDown = this->brotherDown;
	hole->priority = this->priority;

	return hole;

}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::edit::CDataHoleObj_101::dataRecode[] = {
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::category),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::vec),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::x1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::y1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::x2),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::y2),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::z1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_101,	nn::yb::edit::CDataHoleObj_101::z2),
	DATA_END,
};
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHoleObj_101::CDataHoleObj_101() : ::TASK_SAVE() {
	
	printf( "CDataHoleObj_101() Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHoleObj_101::CDataHoleObj_101( ::nn_common::SaveManager * m) : ::TASK_SAVE( m) {
	
	printf( "CDataHoleObj_101( SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void ::nn::yb::edit::CDataHoleObj_101::constructorCommon() {

	this->category = 0;
	this->vec = 0;
	this->x1 = 0;
	this->x2 = 0;
	this->y1 = 0;
	this->y2 = 0;
	this->z1 = 0;
	this->z2 = 0;

}
//---------------------------------------------------------------------------
::nn_common::DATARECODE * ::nn::yb::edit::CDataHoleObj_101::getDataRecode() {

	return ::nn::yb::edit::CDataHoleObj_101::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::edit::CDataHoleObj_101::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_OBJ_101;

}
//---------------------------------------------------------------------------
::nn_common::SaveObjBase * ::nn::yb::edit::CDataHoleObj_101::update() {

	printf( "CDataHoleObj_101::update\n");
	nn::yb::edit::CDataHoleObj * obj = new nn::yb::edit::CDataHoleObj();

	obj->category = this->category;
	obj->x1 = this->x1;
	obj->x2 = this->x2;
	obj->y1 = this->y1;
	obj->y2 = this->y2;

	return obj;

}
//---------------------------------------------------------------------------
