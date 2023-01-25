#ifndef __DATA_HOLE_100_H__
#define __DATA_HOLE_100_H__

#include "save_obj_base.h"

//---------------------------------------------------------------------------
// ëOï˚êÈåæ
namespace nn {
namespace yb {

namespace edit {
class CEditHoleObj;
} // namespece edit

} // namespace yb
} // namespace nn
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

class CDataHoleObj_100 : public nn_common::SaveObjBase {
public:
	CDataHoleObj_100();
	CDataHoleObj_100( nn_common::SaveManager * m);
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	::nn_common::SaveObjBase * update();
	void constructorCommon();
	
	static ::nn_common::DATARECODE dataRecode[];
	
	int category;
	int x1;
	int y1;
	int x2;
	int y2;

};
//---------------------------------------------------------------------------
class CDataHole_100 : public nn_common::SaveObjBase {
	
public:
	CDataHole_100();
	CDataHole_100( nn_common::SaveManager * m);
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void constructorCommon();
	::nn_common::SaveObjBase * update();
	
	static ::nn_common::DATARECODE dataRecode[];
	
	int windowW;
	int windowH;
	int w;
	int h;
	nn::yb::edit::CEditHoleObj * obj[256];
	nn::yb::edit::CDataHoleObj_100 * dataObj[256];
	int objCnt;
	
};

} // namespece edit
//---------------------------------------------------------------------------
} // namespace yb
} // namespace nn
#endif //__DATA_HOLE_100_H__