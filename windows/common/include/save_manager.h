#ifndef __SAVE_MANAGER_H__
#define __SAVE_MANAGER_H__

#include <vector>
#include "save_obj_base.h"
#include "type.h"

namespace nn_common {
	
namespace e {
namespace save_manager {

enum {
	CLASS_ID_COMMON_DISP_3D_BG,
	CLASS_ID_COMMON_ANIME,
	CLASS_ID_COMMON_END			= 100
};

} // namespace save_manager
} // namespace e

#define SAVE_MANAGER_NO_ELEM (-1)
#define SAVE_MANAGER_CONNECT_OK (1)

class SaveManager {

public:
	SaveManager();
	virtual ~SaveManager();
	int save( ::std::string filePath, bool crcFlag, unsigned short key);
	void addSaveObj( SaveObjBase * obj);
	int load( ::std::string filePath, bool crcFlag);
	int load( char * buf, int size, bool crcFlag);
	int load();
	virtual ::nn_common::SaveObjBase * createObj( int classId) = 0;
	int connectPtr( ::nn_common::DATARECODE * dataArray, ::nn_common::SaveObjBase * obj);
	int read( ::nn_common::DATARECODE * dataArray, ::nn_common::SaveObjBase * obj);
	int write( ::nn_common::DATARECODE * dataArray, ::nn_common::SaveObjBase * obj);
	int ptr2Elem( void * pos);
	void loadSkip();
	void eraseSaveObj( SaveObjBase * obj);
	bool isLoad( ::std::string filePath);
	void dump( ::std::string filePath);
	void _dump();
	void readBuf( char * dst, int size);
	bool isBufEnd();
	void writeBuf( char * src, int size);
	static void setCrc( u8 * buf, int size);		// size = CRC=2Byte含むサイズ
	static u16 crc( u8 * buf, int n);
	static int checkCrc( u8 * buf, int size);		// return = 0 正しいデータ size = CRC=2Byte含むサイズ
	int open( char ** buf, std::string filePath);
	void dumpSaveObj();
	bool isExistSaveObj( ::nn_common::SaveObjBase * checkObj);	// checkObjがセーブするinstanceに登録されていたらtrueを返す
	void dumpDataType(char ** buf);
	void dumpSaveData(char * buf);
	void eraseSaveObjAll();
	
#if 0
	::std::ofstream * ofs;
	::std::ifstream * ifs;
#endif
	FILE * ifp;
#if 0
	FILE * ofp;
#endif

protected:
	::std::vector<::nn_common::SaveObjBase *> saveObjVector;
	
private:
	char * buf;
	char * bufEnd;
	char * bufStart;

};

} //nn_common

#endif //__SAVE_MANAGER_H__
