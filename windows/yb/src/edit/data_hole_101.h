#ifndef __DATA_HOLE_101_H__
#define __DATA_HOLE_101_H__

#include "task_save.h"

//---------------------------------------------------------------------------
// ëOï˚êÈåæ
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {
namespace e {
namespace data_hole {

} // namespece data_hole
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace edit {

class CDataHoleObj_101 : public ::TASK_SAVE {
public:
	CDataHoleObj_101();
	CDataHoleObj_101( nn_common::SaveManager * m);
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void constructorCommon();
	::nn_common::SaveObjBase * update();
	
	static ::nn_common::DATARECODE dataRecode[];
	
	int category;
	int vec;
	int x1;
	int y1;
	int x2;
	int y2;
	int z1;
	int z2;

};
//---------------------------------------------------------------------------
class CDataHole_101 : public ::TASK_SAVE {
	
public:
	CDataHole_101();
	CDataHole_101( nn_common::SaveManager * m);
	CDataHole_101( ::TASK_SAVE * parent, nn_common::SaveManager * m);
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void constructorCommon();
	void afterUpdate();
	::nn_common::SaveObjBase * update();
	
	static ::nn_common::DATARECODE dataRecode[];
	
	int windowW;
	int windowH;
	int w;
	int h;
	nn::yb::edit::CEditHoleObj * obj[256];
	nn::yb::edit::CDataHoleObj_101 * dataObj[256];
	int objCnt;
	nn::yb::edit::CEdit * edit;
	
};

} // namespece edit
//---------------------------------------------------------------------------
} // namespace yb
} // namespace nn
#endif //__DATA_HOLE_101_H__