// EDIT を置換
// edit を置換
// Edit を置換
// yb を置換
// edit を置換

#ifndef __EDIT_H__
#define __EDIT_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {

namespace e {
namespace edit {

enum STATE {
	STATE_NONE,
	
	STATE_MAX
};

enum MODE {
	MODE_CREATE,
	MODE_EDIT,

	MODE_MAX
};

enum CREATE_STATE {
	CREATE_STATE_NONE,
	CREATE_STATE_CREATE,

	CREATE_STATE_MAX
};

enum BUTTON_ID {
	BUTTON_ID_CREATE,
	BUTTON_ID_EDIT,

	BUTTON_ID_CREATE_GROUND,	// edit_hole_obj.h CATEGORY と合っている必要がある
	BUTTON_ID_CREATE_BUMPER,
	BUTTON_ID_CREATE_ENEMY,
	BUTTON_ID_CREATE_ENEMY_MUSO,
	BUTTON_ID_CREATE_ENEMY_SPEA,

	BUTTON_ID_CREATE_SLOPE_UP,
	BUTTON_ID_CREATE_SLOPE_RIGHT_UP,
	BUTTON_ID_CREATE_SLOPE_RIGHT,
	BUTTON_ID_CREATE_SLOPE_RIGHT_DOWN,
	BUTTON_ID_CREATE_SLOPE_DOWN,
	BUTTON_ID_CREATE_SLOPE_LEFT_DOWN,
	BUTTON_ID_CREATE_SLOPE_LEFT,
	BUTTON_ID_CREATE_SLOPE_LEFT_UP,

	BUTTON_ID_CREATE_PILLAR,

	BUTTON_ID_CREATE_WARP,
	BUTTON_ID_CREATE_WARP_UP_UP,
	BUTTON_ID_CREATE_WARP_UP_RIGHT,
	BUTTON_ID_CREATE_WARP_UP_DOWN,
	BUTTON_ID_CREATE_WARP_UP_LEFT,
	BUTTON_ID_CREATE_WARP_RIGHT_UP,
	BUTTON_ID_CREATE_WARP_RIGHT_RIGHT,
	BUTTON_ID_CREATE_WARP_RIGHT_DOWN,
	BUTTON_ID_CREATE_WARP_RIGHT_LEFT,
	BUTTON_ID_CREATE_WARP_DOWN_UP,
	BUTTON_ID_CREATE_WARP_DOWN_RIGHT,
	BUTTON_ID_CREATE_WARP_DOWN_DOWN,
	BUTTON_ID_CREATE_WARP_DOWN_LEFT,
	BUTTON_ID_CREATE_WARP_LEFT_UP,
	BUTTON_ID_CREATE_WARP_LEFT_RIGHT,
	BUTTON_ID_CREATE_WARP_LEFT_DOWN,
	BUTTON_ID_CREATE_WARP_LEFT_LEFT,

	BUTTON_ID_CREATE_KICK_UP,
	BUTTON_ID_CREATE_KICK_RIGHT,
	BUTTON_ID_CREATE_KICK_DOWN,
	BUTTON_ID_CREATE_KICK_LEFT,
	
	BUTTON_ID_CREATE_ENEMY_SKY,
	BUTTON_ID_CREATE_ENEMY_MUSO_SKY,
	BUTTON_ID_CREATE_ENEMY_SPEA_SKY,
	BUTTON_ID_CREATE_ENEMY_KEDAMA,
	BUTTON_ID_CREATE_ENEMY_KEDAMA_SKY,
	BUTTON_ID_CREATE_ENEMY_HANEMEDAMA,
	BUTTON_ID_CREATE_ENEMY_HANEMEDAMA_SKY,

	BUTTON_ID_CREATE_DASH,
	BUTTON_ID_CREATE_DASH_UP,
	BUTTON_ID_CREATE_DASH_RIGHT,
	BUTTON_ID_CREATE_DASH_DOWN,
	BUTTON_ID_CREATE_DASH_LEFT,

	BUTTON_ID_CREATE_JUMP,

	BUTTON_ID_CREATE_CAMERA,

	BUTTON_ID_CREATE_ENEMY_MSPA,
	BUTTON_ID_CREATE_ENEMY_MSPA_SKY,

	BUTTON_ID_CREATE_DAMAGE, //ダメージ床
	BUTTON_ID_CREATE_BUNKER, //バンカー
	BUTTON_ID_CREATE_GRAVITY, //重力エリア
	BUTTON_ID_CREATE_DANMAKU, //弾幕

	BUTTON_ID_CREATE_ENEMY_MOMIJI,
	BUTTON_ID_CREATE_ENEMY_TENKO,
	BUTTON_ID_CREATE_ENEMY_GREEN_FAIRY,
	BUTTON_ID_CREATE_ENEMY_BLUE_FAIRY,
	BUTTON_ID_CREATE_UNZAN,
	BUTTON_ID_CREATE_OKU,

	BUTTON_ID_CREATE_FUKIAGE, //吹き上げエリア
	BUTTON_ID_CREATE_CONVEYOR_LEFT_UP,//コンベアー
	BUTTON_ID_CREATE_CONVEYOR_LEFT_RIGHT,
	BUTTON_ID_CREATE_CONVEYOR_LEFT_DOWN,
	BUTTON_ID_CREATE_CONVEYOR_LEFT_LEFT,
	BUTTON_ID_CREATE_JUMP_WEAK, //小ジャンプ
	BUTTON_ID_CREATE_JUMP_STRONG, //大ジャンプ
	BUTTON_ID_CREATE_SE_FLOOR, //SE床

	BUTTON_ID_CREATE_KICK_UP_RIGHT,
	BUTTON_ID_CREATE_KICK_UP_LEFT,
	BUTTON_ID_CREATE_KICK_DOWN_RIGHT,
	BUTTON_ID_CREATE_KICK_DOWN_LEFT,

	BUTTON_ID_CREATE_DASH_UP_RIGHT,
	BUTTON_ID_CREATE_DASH_UP_LEFT,
	BUTTON_ID_CREATE_DASH_DOWN_RIGHT,
	BUTTON_ID_CREATE_DASH_DOWN_LEFT,

	BUTTON_ID_CREATE_WARP2_NONE,
	BUTTON_ID_CREATE_WARP2_UP,
	BUTTON_ID_CREATE_WARP2_RIGHT_UP,
	BUTTON_ID_CREATE_WARP2_RIGHT,
	BUTTON_ID_CREATE_WARP2_RIGHT_DOWN,
	BUTTON_ID_CREATE_WARP2_DOWN,
	BUTTON_ID_CREATE_WARP2_LEFT_DOWN,
	BUTTON_ID_CREATE_WARP2_LEFT,
	BUTTON_ID_CREATE_WARP2_LEFT_UP,
	
	BUTTON_ID_CREATE_ENEMY_NITORI,
	BUTTON_ID_CREATE_ENEMY_FLAN,
	BUTTON_ID_CREATE_ENEMY_ORIN,
	
	BUTTON_ID_CREATE_UNZAN_REMIFLA,
	BUTTON_ID_CREATE_UNZAN_MOVE,
	BUTTON_ID_CREATE_UNZAN_MOVE_REMIFLA,

	BUTTON_ID_CREATE_TIME_ATTACK_START,
	BUTTON_ID_CREATE_TIME_ATTACK_GOAL,

	BUTTON_ID_EDIT_DELETE,

	BUTTON_ID_COMMON_PLAY,
	BUTTON_ID_COMMON_SAVE,
	BUTTON_ID_COMMON_END,

	BUTTON_ID_MAX
};

enum DIALOG_ID {
	DIALOG_ID_SAVE,
	DIALOG_ID_END,

	DIALOG_ID_MAX
};

enum DIALOG_BUTTON_NO {
	DIALOG_BUTTON_NO_OK,
	DIALOG_BUTTON_NO_CANCEL,

	DIAGLOG_BUTTON_NO_MAX
};

} // namespece edit
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace edit {
	
class CEdit : public TASK_SAVE {
	
public:
	CEdit( TASK_SAVE * oya);
	virtual ~CEdit();
	void task();
	void disp();
	
	void start();

	static void onButtonModeCb( void * argEdit, void * argButton);
	void onButtonMode( void * argButton);

	void createButton( int id, void (* onCb)( void *, void *), char * caption, int x, int y,
		GXRgb color, GXRgb darkColor, nn::yb::common::CButton ** buttonContainer
	);
	void createButtonAll();

	static void onButtonCreateModeCb( void * argEdit, void * argButton);
	void onButtonCreateMode( void * argButton);

	void activeClearObjAll();

	void taskEdit();
	void taskCreate();

	static void onButtonEditModeCb( void * argEdit, void * argButton);
	void onButtonEditMode( void * argButton);

	static void onButtonCommonCb( void * argEdit, void * argButton);
	void onButtonCommon( void * argButton);

	static void onDialogCb( void * argEdit, void * argDialog);
	void onDialog( void * argDialog);

	void save();
	void load( char * dataFileName);

	bool taskWindowScrollHand();

	void play();

	void setWindowPos( int x, int y);
	void taskWindowScrollOut();
	void setButtonStateAllHide();
	void setButtonStateStart();
	void setButtonStateChangeMode();
	void setButtonStateChangeCreateMode();
	void setButtonStateCreateModeHide();
	void setButtonStateEditModeHide();

	static void getHoleDataFileName( char * dataFileName, int dataNo);
	void onSave();
	void onEnd();
	void changeMode(int mode);
	void onDelete();
	void loadFromImage( char * imageFileName, unsigned int checkColor, int setCategory);

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::edit::CDataHole * hole;

	int createMode;
	int mode;

	nn::yb::common::CYbSaveManager * saveManager;

	nn::yb::common::CButton * modeButton[16];
	nn::yb::common::CButton * createModeButton[32];

	int activeObjIndex;

	int createState;
	
	nn::yb::common::CButton * editModeButton[16];

	int objDataDispX;
	
	nn::yb::common::CButton * commonButton[16];

	int editDataNo;

	bool windowScrollFlag;
	int xSlideOld;
	int ySlideOld;
	int xWindow;
	int yWindow;
	int wWindow;
	int hWindow;

	nn::yb::main::CCtrl * ctrl;

	double scale;

	int spell;
	int createButtonN;
	int createButtonCursorX;
	
}; // CEdit
	 
} // namespece edit

} // namespace yb
} // namespace nn
	

#endif // __EDIT_H__
