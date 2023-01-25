
#include <string.h>

#include "save_manager.h"
#include "data_hole_100.h"
#include "yb_save_manager.h"
#include "data_hole_101.h"
#include "edit_hole_obj.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::edit::CDataHole_100::dataRecode[] = {
	DATA_LOCAL(	nn::yb::edit::CDataHole_100,	nn::yb::edit::CDataHole_100::w),
	DATA_LOCAL(	nn::yb::edit::CDataHole_100,	nn::yb::edit::CDataHole_100::h),
	DATA_LOCAL(	nn::yb::edit::CDataHole_100,	nn::yb::edit::CDataHole_100::objCnt),
	DATA_PTR( 	nn::yb::edit::CDataHole_100,	nn::yb::edit::CDataHole_100::dataObj),
	DATA_END,
};
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole_100::CDataHole_100() : nn_common::SaveObjBase() {
	
	printf( "CDataHole_100() Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHole_100::CDataHole_100( ::nn_common::SaveManager * m) : nn_common::SaveObjBase( m) {
	
	printf( "CDataHole_100( SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void ::nn::yb::edit::CDataHole_100::constructorCommon() {
	
	memset( this->obj, 0, sizeof( this->obj));
	memset( this->dataObj, 0, sizeof( this->dataObj));
	this->objCnt = 0;
	this->w = 0;
	this->h = 0;
	this->windowW = 0;
	this->windowH = 0;

}
//---------------------------------------------------------------------------
::nn_common::DATARECODE * ::nn::yb::edit::CDataHole_100::getDataRecode() {

	return ::nn::yb::edit::CDataHole_100::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::edit::CDataHole_100::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_100;

}
//---------------------------------------------------------------------------
::nn_common::SaveObjBase * nn::yb::edit::CDataHole_100::update() {

	printf( "CDataHole_100::update\n");
	nn::yb::edit::CDataHole_101 * hole = new nn::yb::edit::CDataHole_101();

	hole->w = this->w;
	hole->h = this->h;
	hole->objCnt = this->objCnt;
	::memcpy( hole->dataObj, this->dataObj, sizeof( hole->dataObj));
	memset( hole->obj, -1, sizeof( hole->obj));

	hole->parent = (::TASK_SAVE *)-1;
	hole->childTop = (::TASK_SAVE *)-1;
	hole->childBottom = (::TASK_SAVE *)-1;
	hole->brotherUp = (::TASK_SAVE *)-1;
	hole->brotherDown = (::TASK_SAVE *)-1;
	hole->priority = TASK_DEFAULT_PRIORITY;

	return hole;

}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::edit::CDataHoleObj_100::dataRecode[] = {
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_100,	nn::yb::edit::CDataHoleObj_100::category),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_100,	nn::yb::edit::CDataHoleObj_100::x1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_100,	nn::yb::edit::CDataHoleObj_100::y1),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_100,	nn::yb::edit::CDataHoleObj_100::x2),
	DATA_LOCAL(	nn::yb::edit::CDataHoleObj_100,	nn::yb::edit::CDataHoleObj_100::y2),
	DATA_END,
};
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHoleObj_100::CDataHoleObj_100() : nn_common::SaveObjBase() {
	
	printf( "CDataHoleObj_100() Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::edit::CDataHoleObj_100::CDataHoleObj_100( ::nn_common::SaveManager * m) : nn_common::SaveObjBase( m) {
	
	printf( "CDataHoleObj_100( SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void ::nn::yb::edit::CDataHoleObj_100::constructorCommon() {

}
//---------------------------------------------------------------------------
::nn_common::DATARECODE * ::nn::yb::edit::CDataHoleObj_100::getDataRecode() {

	return ::nn::yb::edit::CDataHoleObj_100::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::edit::CDataHoleObj_100::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_DATA_HOLE_OBJ_100;

}
//---------------------------------------------------------------------------
::nn_common::SaveObjBase * ::nn::yb::edit::CDataHoleObj_100::update() {

	printf( "CDataHoleObj_100::update\n");
	nn::yb::edit::CDataHoleObj_101 * obj = new nn::yb::edit::CDataHoleObj_101();

	obj->category = this->category;
	obj->x1 = this->x1;
	obj->x2 = this->x2;
	obj->y1 = this->y1;
	obj->y2 = this->y2;

	switch(obj->category) {
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SKY:
			obj->category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY;
			obj->z1 = 64;
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO_SKY:
			obj->category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MUSO;
			obj->z2 = 128;
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA_SKY:
			obj->category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_SPEA;
			obj->z2 = 128;
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA_SKY:
			obj->category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_KEDAMA;
			obj->z2 = 128;
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA_SKY:
			obj->category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_HANEMEDAMA;
			obj->z2 = 192;
			break;
		case nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA_SKY:
			obj->category = nn::yb::e::edit_hole_obj::CATEGORY_ENEMY_MSPA;
			obj->z2 = 128;
			break;

	}


	return obj;

}
//---------------------------------------------------------------------------
