
#include "edit_hole_obj.h"

#include "save_manager.h"
#include "data_hole.h"
#include "yb_save_manager.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::edit::CDataHole::dataRecode[] = {
	DATA_LOCAL(	nn::yb::edit::CDataHole,	nn::yb::edit::CDataHole::w),
	DATA_LOCAL(	nn::yb::edit::CDataHole,	nn::yb::edit::CDataHole::h),
	DATA_LOCAL(	nn::yb::edit::CDataHole,	nn::yb::edit::CDataHole::objCnt),
	DATA_PTR( 	nn::yb::edit::CDataHole,	nn::yb::edit::CDataHole::obj),
	DATA_BASE( ::TASK_SAVE::dataRecode ),
	DATA_END,
};
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole::CDataHole() : ::TASK_SAVE() {
	
	printf( "CDataHole() Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole::CDataHole( ::nn_common::SaveManager * m) : ::TASK_SAVE( m) {
	
	printf( "CDataHole( SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole::CDataHole( ::TASK_SAVE * parent, ::nn_common::SaveManager * m) : ::TASK_SAVE( parent, m) {
	
	printf( "CDataHole( TASK_SAVE, SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void ::nn::yb::edit::CDataHole::constructorCommon() {

	memset( this->obj, 0, sizeof( this->obj));
	this->objCnt = 0;
	this->w = 0;
	this->h = 0;
	this->windowW = 0;
	this->windowH = 0;
	this->edit = NULL;

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole::~CDataHole() {

	printf( "CDataHole() Delete\n");

}
//---------------------------------------------------------------------------
::nn_common::DATARECODE * ::nn::yb::edit::CDataHole::getDataRecode() {

	return ::nn::yb::edit::CDataHole::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::edit::CDataHole::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE;

}
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::edit::CDataHoleObj::dataRecode[] = {
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::category),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::vec),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::x1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::y1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::x2),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::y2),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::z1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj,	nn::yb::edit::CDataHoleObj::z2),
	DATA_END,
};
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHoleObj::CDataHoleObj() : ::TASK_SAVE() {
	
	printf( "CDataHoleObj() Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHoleObj::CDataHoleObj( ::nn_common::SaveManager * m) : ::TASK_SAVE( m) {
	
	printf( "CDataHoleObj( SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHoleObj::~CDataHoleObj() {
	
	printf( "CDataHoleObj() Delete\n");

}
//---------------------------------------------------------------------------
void ::nn::yb::edit::CDataHoleObj::constructorCommon() {

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
::nn_common::DATARECODE * ::nn::yb::edit::CDataHoleObj::getDataRecode() {

	return ::nn::yb::edit::CDataHoleObj::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::edit::CDataHoleObj::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_OBJ;

}
//---------------------------------------------------------------------------
