#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "CTecoDll.h"
#include "CTecoVM.h"
#include "CTecoData.h"
#include "script_cmd.h"
#include "script_var.h"
#include "master.h"
#include "layout.h"
#include "obj.h"
#include "task_save.h"

namespace nn {
namespace dxlib {

namespace e {
namespace script {
enum {
	DISP_OBJS_N = 256
};

enum STATE {
	STATE_NONE,
	STATE_PROC,
	STATE_WAIT,
	STATE_NEXT_FRAME,
	STATE_WAIT_ASYNC_MSG,

	STATE_MAX
};

enum OPE_MODE {
	OPE_MODE_TOUCH,
	OPE_MODE_PAD,
	OPE_MODE_TOUCH_PAD,

	OPE_MODE_MAX
};

enum SELECT_CURSOR_MODE {
	SELECT_CURSOR_MODE_LEFT_RIGHT,
	SELECT_CURSOR_MODE_UP_DOWN,

	SELECT_CURSOR_MODE_MAX
};

enum DISP_CATEGORY {
	DISP_CATEGORY_NONE,
	DISP_CATEGORY_OBJ,
	DISP_CATEGORY_OBJ_CENTER,
	DISP_CATEGORY_BOARD,

	DISP_CATEGORY_MAX
};

}
namespace script_task {

enum MODE {
	MODE_NONE,
	MODE_MSG,
	MODE_MSG_REMAIN,
	MODE_MSG_ASYNC,
	MODE_SELECT,
	MODE_SELECT_TEXT,

	MODE_MAX
};

}
}

class CScriptTask;
class CFont;

typedef struct _script_disp_obj {
	
	int category;
	int objId;
	int x;
	int y;
	int w;
	int h;
	int z;
	int a;
	GXRgb color;
	int rotate;
	BOOL flipH;
	BOOL flipV;
	int scale;
	
} SCRIPT_DISP_OBJ;

typedef struct _script_msg_param {
	LAYOUT layoutText;	// 開始位置
	int hSpace;			// 文字間
	int vSpace;			// 行間

	int layoutIdMsgBg;	// メッセージウィンドウ
	
	unsigned int skipButtonBit;				// どれがスキップボタンか
	unsigned int skipOnPressCnt;			// 何フレームスキップボタンを押し続けたら、スキップ状態になるか
	bool skip;								// スキップ機能の有効無効

	int opeMode;							// TOUCH PAD TOUCHandPAD
} SCRIPT_MSG_PARAM;

typedef struct _script_select_param {
	int buttonLayoutId[16];
	int cursorLayoutId[16];
	int touchLayoutId[16];
	int selectSeNo[16];
	int selectN;
	int buttonActiveAnimeId[16];
	int buttonIdleAnimeId[16];
	int buttonDecideAnimeId[16];
	int buttonBgLayoutId[16];
	int cursorAnimeIdDecide;
	
	int cursorMode;
} SCRIPT_SELECT_PARAM;

typedef struct _script_select_text_param {
	char selectTexts[8][256];
	int selectN;
	int x;
	int y;
} SCRIPT_SELECT_TEXT_PARAM;

class CScript : public nn::common::teco::CTecoVM
{
public:

	CScript()
		: m_pWork( NULL )
		, nextseqNo( 0 )
	{
		memset( this->dispObjs, 0, sizeof( this->dispObjs));
		memset( &this->msgParam, 0, sizeof( this->msgParam));
		memset( &this->msgTasks, 0, sizeof( this->msgTasks));
		this->master = NULL;
		memset( &this->selectParam, 0, sizeof( this->selectParam));
		memset( &this->selectTextParam, 0, sizeof( this->selectTextParam));
		memset( this->nextFileName, 0, sizeof( this->nextFileName));
		this->tecoDll = new CTecoDll();
		this->tecoDll->Init();
		memset( this->scriptDirectory, 0, sizeof( this->scriptDirectory));
		this->scriptEndCbFunc = NULL;
		this->scriptEndCbFuncArg = NULL;
		this->extFunc = NULL;
		this->extFuncInstance = NULL;
		this->font = NULL;
		this->waitAsyncMsgIndex = -1;
		this->skipButtonPressCnt = 0;
		this->skipOnFlag = false;
	}

	virtual ~CScript()
	{
		if( m_pWork )
			delete [] m_pWork;
	}

	bool Init( char * sceneFileName, void * pGlobal);
	bool Init( BYTE * pData, void * pGlobal);

	void run( void );

	// ユーザー命令の実行（必ず実装する）
	virtual eRetCommand CallCommand( nn::common::teco::CTecoData::SCommand *pSCommand, int *paArg );


	// オフセットから文字列取り出し
	char *OffsetToString( int offset )
	{
		return reinterpret_cast<char *>( GetTecoData()->OffsetToAddress( offset ) );
	}




	// キャラクターの種類
	enum
	{
		CHR_PLAYER,
		CHR_TARO,
	};

	// キャラクターの表示位置
	enum
	{
		LEFT,
		CENTER,
		RIGHT,
	};

	// talk 命令の引数
	typedef struct
	{
		int character;
		int message;

	} SCommand_Talk;

	nn::dxlib::script::GlobalCommonVar * m_globalVar;
	BYTE *m_pWork;

//-----------------------------------------------------------------------------
	void disp();
	static void setStateProcCb( void * scriptArg);
	void setStateProc();
	bool startScript( char * scriptFileName, int seqNo, void * pGlobal);
	bool startScene( char * sceneFileName, int seqNo, void * pGlobal);
	void endScript();
	TASK_SAVE * msg( TASK_SAVE *parent, char *msg,
		nn::dxlib::e::script_task::MODE mode,
		void (* endFunc)( void *), void *endFuncArg
	);
	void checkScriptEnd();
	void procScriptEnd();

	int state;
	nn::dxlib::SCRIPT_DISP_OBJ dispObjs[nn::dxlib::e::script::DISP_OBJS_N];
	nn::dxlib::SCRIPT_MSG_PARAM msgParam;
	nn::dxlib::CScriptTask * msgTasks[16];
	MASTER * master;

	nn::dxlib::SCRIPT_SELECT_PARAM selectParam;
	nn::dxlib::SCRIPT_SELECT_TEXT_PARAM selectTextParam;
	
	::CTecoDll * tecoDll;

	char nextFileName[256];
	int nextseqNo;
	char scriptDirectory[256];

	void (* scriptEndCbFunc)(void *);
	void * scriptEndCbFuncArg;

	int (* extFunc)( void * instance, void * script, int id, int * arg);
	void * extFuncInstance;
	CFont * font;
	int waitAsyncMsgIndex;

	unsigned int skipButtonPressCnt;
	bool skipOnFlag;

};

} // namespace dxlib
} // namespace nn

#endif //__SCRIPT_H__
