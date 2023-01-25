#ifndef __DATA_HOLE_H__
#define __DATA_HOLE_H__

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

class CDataHoleObj : public ::TASK_SAVE {
public:
	CDataHoleObj();
	CDataHoleObj( nn_common::SaveManager * m);
	virtual ~CDataHoleObj();
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void constructorCommon();
	
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
class CDataHole : public ::TASK_SAVE {
	
public:
	CDataHole();
	CDataHole( nn_common::SaveManager * m);
	CDataHole( ::TASK_SAVE * parent, nn_common::SaveManager * m);
	virtual ~CDataHole();
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void constructorCommon();
	
	static ::nn_common::DATARECODE dataRecode[];
	
	int windowW;
	int windowH;
	int w;
	int h;
	nn::yb::edit::CEditHoleObj * obj[1024];
	int objCnt;
	nn::yb::edit::CEdit * edit;
	
};

} // namespece edit
//---------------------------------------------------------------------------
} // namespace yb
} // namespace nn
#endif //__DATA_HOLE_H__