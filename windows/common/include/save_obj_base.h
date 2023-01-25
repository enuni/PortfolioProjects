#ifndef __SAVE_OBJ_BASE_H__
#define __SAVE_OBJ_BASE_H__

namespace nn_common {

class SaveManager;

enum {
	DATA_TYPE_LOCAL = 1,
	DATA_TYPE_PTR,
	DATA_TYPE_MEM,
	DATA_TYPE_BASE,
	DATA_TYPE_END,

	DATA_MAX
};

#define CALC_CLASS_MEMBER_OFFSET(CLASS, MEMBER) (&((CLASS *)0)->MEMBER)
#define CALC_CLASS_MEMBER_SIZE( CLASS, MEMBER) sizeof(((CLASS *)0)->MEMBER)
#define DATA_LOCAL( CLASS, MEMBER) { ::nn_common::DATA_TYPE_LOCAL, (int)CALC_CLASS_MEMBER_OFFSET( CLASS, MEMBER), CALC_CLASS_MEMBER_SIZE( CLASS, MEMBER) }
#define DATA_PTR( CLASS, MEMBER) { ::nn_common::DATA_TYPE_PTR, ( (__int64)&((CLASS*)0)->MEMBER ), CALC_CLASS_MEMBER_SIZE( CLASS, MEMBER) }
#define DATA_MEM( CLASS, MEMBER, SIZE) { ::nn_common::DATA_TYPE_MEM, ( (__int64)&((CLASS*)0)->MEMBER ), ( (__int64)&((CLASS*)0)->SIZE ) }
#define DATA_BASE( TABLE) { ::nn_common::DATA_TYPE_BASE, ((__int64)TABLE), 0 }
#define DATA_END { ::nn_common::DATA_TYPE_END, 0, 0 }

typedef struct _datarecode {
	char type;
	int offset;
	int size;
} DATARECODE;

class SaveObjBase {

public:
	SaveObjBase();
	SaveObjBase( nn_common::SaveManager * m);
	virtual ~SaveObjBase();
	virtual ::nn_common::DATARECODE * getDataRecode() = 0;
	virtual int getClassId() = 0;
	int connectPtr( ::nn_common::SaveManager * m);
	virtual ::nn_common::SaveObjBase * update();
	virtual void addSaveManager( ::nn_common::SaveManager * m);
	virtual void loadPostProc();

	nn_common::SaveManager * saveManager;

	char name[32];

};

} //namespace nn_common

#endif //__SAVE_OBJ_BASE_H__