
#ifndef __MASTER_H__
#define __MASTER_H__

#include "type.h"

//---------------------------------------------------------------------------

#define MASTER_CONFIG_PAD_N (32)
#define MASTER_CONFIG_KEY_N (14)

typedef struct {
	u32 trigger;	// 押した瞬間
	u32 press;		// 押されている状態
	u32 pulse;		// 押した瞬間としばらく押していると一定間隔で発生
	u32 hold;		// しばらく押していると毎フレーム発生
	BOOL foldFlag;
	u8 mode;
	int configPad[MASTER_CONFIG_PAD_N];
	int configKey[MASTER_CONFIG_KEY_N];
	u32 originalPadState;
	u32 originalKeyState;
} KEY_WORK;

typedef struct {
	char trigger[256];
	char press[256];
} KEYBOARD_WORK;

#ifndef __TOUCH_WORK__
#define __TOUCH_WORK__

typedef struct _touch_work {
	s16 touchPoint;	//タッチイベント 場所が正確
	s16 touchDisp;	//タッチイベント 場所が不正確でも
	s16 first_x;	//現在のタッチイベントで最初にタッチした場所のx座標
	s16 first_y;	//現在のタッチイベントで最初にタッチした場所のy座標
	s16 release_x;	//現在のタッチイベントで放した場所のx座標
	s16 release_y;	//現在のタッチイベントで話した場所のy座標
	s16 x;	//現在タッチペンがタッチしている場所のx座標
	s16 y;	//現在タッチペンがタッチしている場所のy座標
	
	s16 slideStartX;
	s16 slideStartY;
} TOUCH_WORK;

#endif //__TOUCH_WORK__

typedef struct {
	KEY_WORK keyWork;	//現在のキーの状態
	TOUCH_WORK touchWork;	//現在のタッチの状態
	KEYBOARD_WORK keyboardWork;	//現在のキーボードの状態
	u8 inputState;	//キーモードかタッチモードか
	BOOL pouseFlag;
	BOOL gameContinueFlag;
} MASTER;

enum {
	MASTER_INPUT_STATE_KEY,
	MASTER_INPUT_STATE_TOUCH,
	MASTER_INPUT_STATE_MAX
};

enum {
	PAD_MODE_WIN,
	PAD_MODE_DS,

	PAD_MODE_MAX
};

// 互換性のため残し
enum {
	INPUT_KEY_DOWN =	0x0001,
	INPUT_KEY_LEFT =	0x0002,
	INPUT_KEY_RIGHT =	0x0004,
	INPUT_KEY_UP =		0x0008,
	INPUT_BUTTON_1 =	0x0010,
	INPUT_BUTTON_2 =	0x0020,
	INPUT_BUTTON_3 =	0x0040,
	INPUT_BUTTON_4 =	0x0080,
	INPUT_BUTTON_5 =	0x0100,
	INPUT_BUTTON_6 =	0x0200,
	INPUT_BUTTON_7 =	0x0400,
	INPUT_BUTTON_8 =	0x0800,
	INPUT_BUTTON_9 =	0x1000,
	INPUT_BUTTON_10 =	0x2000,
	
	INPUT_MAX
};

namespace nn {
namespace common {
namespace e {

// 互換性のため残し
enum PAD_WIN {
	PAD_WIN_KEY_DOWN		=	INPUT_KEY_DOWN,
	PAD_WIN_KEY_LEFT		=	INPUT_KEY_LEFT,
	PAD_WIN_KEY_RIGHT		=	INPUT_KEY_RIGHT,
	PAD_WIN_KEY_UP			=	INPUT_KEY_UP,
	PAD_WIN_BUTTON_A		=	INPUT_BUTTON_1,
	PAD_WIN_BUTTON_B		=	INPUT_BUTTON_2,
	PAD_WIN_BUTTON_X		=	INPUT_BUTTON_3,
	PAD_WIN_BUTTON_Y		=	INPUT_BUTTON_4,
	PAD_WIN_BUTTON_L1		=	INPUT_BUTTON_5,
	PAD_WIN_BUTTON_R1		=	INPUT_BUTTON_6,
	PAD_WIN_BUTTON_START	=	INPUT_BUTTON_7,
	PAD_WIN_BUTTON_SLECT	=	INPUT_BUTTON_8,
	PAD_WIN_BUTTON_L2		=	INPUT_BUTTON_9,
	PAD_WIN_BUTTON_R2		=	INPUT_BUTTON_10,
	
	PAD_WIN_MAX
};

// 110610以降
enum PAD {
	PAD_DOWN	=	0x0001,
	PAD_LEFT	=	0x0002,
	PAD_RIGHT	=	0x0004,
	PAD_UP		=	0x0008,
	PAD_A		=	0x0010,
	PAD_B		=	0x0020,
	PAD_X		=	0x0040,
	PAD_Y		=	0x0080,
	PAD_L1		=	0x0100,
	PAD_R1		=	0x0200,
	PAD_START	=	0x0400,
	PAD_SELECT	=	0x0800,
	PAD_L2 		=	0x1000,
	PAD_R2 		=	0x2000,
	
	PAD_MAX
};

enum PAD_INDEX {
	PAD_INDEX_NONE = 0,
	PAD_INDEX_DOWN,
	PAD_INDEX_LEFT,
	PAD_INDEX_RIGHT,
	PAD_INDEX_UP,
	PAD_INDEX_A,
	PAD_INDEX_B,
	PAD_INDEX_X,
	PAD_INDEX_Y,
	PAD_INDEX_L1,
	PAD_INDEX_R1,
	PAD_INDEX_START,
	PAD_INDEX_SELECT,
	PAD_INDEX_L2,
	PAD_INDEX_R2,
	
	PAD_INDEX_MAX
};

enum PAD_ORG_INDEX {
	PAD_ORG_INDEX_DOWN = 0,
	PAD_ORG_INDEX_LEFT,
	PAD_ORG_INDEX_RIGHT,
	PAD_ORG_INDEX_UP,
	PAD_ORG_INDEX_1,
	PAD_ORG_INDEX_2,
	PAD_ORG_INDEX_3,
	PAD_ORG_INDEX_4,
	PAD_ORG_INDEX_5,
	PAD_ORG_INDEX_6,
	PAD_ORG_INDEX_7,
	PAD_ORG_INDEX_8,
	PAD_ORG_INDEX_9,
	PAD_ORG_INDEX_10,
	
	PAD_ORG_INDEX_MAX
};

enum KEYBOARD_ORG_INDEX {
	KEYBOARD_ORG_INDEX_DOWN = 0,
	KEYBOARD_ORG_INDEX_LEFT,
	KEYBOARD_ORG_INDEX_RIGHT,
	KEYBOARD_ORG_INDEX_UP,
	KEYBOARD_ORG_INDEX_Z,
	KEYBOARD_ORG_INDEX_X,
	KEYBOARD_ORG_INDEX_C,
	KEYBOARD_ORG_INDEX_A,
	KEYBOARD_ORG_INDEX_S,
	KEYBOARD_ORG_INDEX_D,
	KEYBOARD_ORG_INDEX_Q,
	KEYBOARD_ORG_INDEX_W,
	KEYBOARD_ORG_INDEX_ESC,
	KEYBOARD_ORG_INDEX_SPACE,
	
	KEYBOARD_ORG_INDEX_MAX
};

#define 	KEYBOARD_BACK					KEY_INPUT_BACK			// バックスペースキー
#define 	KEYBOARD_TAB					KEY_INPUT_TAB			// タブキー
#define 	KEYBOARD_RETURN					KEY_INPUT_RETURN		// エンターキー

#define 	KEYBOARD_LSHIFT					KEY_INPUT_LSHIFT		// 左シフトキー
#define 	KEYBOARD_RSHIFT					KEY_INPUT_RSHIFT		// 右シフトキー
#define 	KEYBOARD_LCONTROL				KEY_INPUT_LCONTROL		// 左コントロールキー
#define 	KEYBOARD_RCONTROL				KEY_INPUT_RCONTROL		// 右コントロールキー
#define 	KEYBOARD_ESCAPE					KEY_INPUT_ESCAPE		// エスケープキー
#define 	KEYBOARD_SPACE					KEY_INPUT_SPACE			// スペースキー
#define 	KEYBOARD_PGUP					KEY_INPUT_PGUP			// ＰａｇｅＵＰキー
#define 	KEYBOARD_PGDN					KEY_INPUT_PGDN			// ＰａｇｅＤｏｗｎキー
#define 	KEYBOARD_END					KEY_INPUT_END			// エンドキー
#define 	KEYBOARD_HOME					KEY_INPUT_HOME			// ホームキー
#define 	KEYBOARD_LEFT					KEY_INPUT_LEFT			// 左キー
#define 	KEYBOARD_UP						KEY_INPUT_UP			// 上キー
#define 	KEYBOARD_RIGHT					KEY_INPUT_RIGHT			// 右キー
#define 	KEYBOARD_DOWN					KEY_INPUT_DOWN			// 下キー
#define 	KEYBOARD_INSERT					KEY_INPUT_INSERT		// インサートキー
#define 	KEYBOARD_DELETE					KEY_INPUT_DELETE		// デリートキー

#define 	KEYBOARD_MINUS					KEY_INPUT_MINUS			// －キー
#define 	KEYBOARD_YEN					KEY_INPUT_YEN			// ￥キー
#define 	KEYBOARD_PREVTRACK				KEY_INPUT_PREVTRACK		// ＾キー
#define 	KEYBOARD_PERIOD					KEY_INPUT_PERIOD		// ．キー
#define 	KEYBOARD_SLASH					KEY_INPUT_SLASH			// ／キー
#define 	KEYBOARD_LALT					KEY_INPUT_LALT			// 左ＡＬＴキー
#define 	KEYBOARD_RALT					KEY_INPUT_RALT			// 右ＡＬＴキー
#define 	KEYBOARD_SCROLL					KEY_INPUT_SCROLL		// ScrollLockキー
#define 	KEYBOARD_SEMICOLON				KEY_INPUT_SEMICOLON		// ；キー
#define 	KEYBOARD_COLON					KEY_INPUT_COLON			// ：キー
#define 	KEYBOARD_LBRACKET				KEY_INPUT_LBRACKET		// ［キー
#define 	KEYBOARD_RBRACKET				KEY_INPUT_RBRACKET		// ］キー
#define 	KEYBOARD_AT						KEY_INPUT_AT			// ＠キー
#define 	KEYBOARD_BACKSLASH				KEY_INPUT_BACKSLASH		// ＼キー
#define 	KEYBOARD_COMMA					KEY_INPUT_COMMA			// ，キー
#define 	KEYBOARD_KANJI					KEY_INPUT_KANJI			// 漢字キー
#define 	KEYBOARD_CONVERT				KEY_INPUT_CONVERT		// 変換キー
#define 	KEYBOARD_NOCONVERT				KEY_INPUT_NOCONVERT		// 無変換キー
#define 	KEYBOARD_KANA					KEY_INPUT_KANA			// カナキー
#define 	KEYBOARD_APPS					KEY_INPUT_APPS			// アプリケーションメニューキー
#define 	KEYBOARD_CAPSLOCK				KEY_INPUT_CAPSLOCK		// CaspLockキー
#define 	KEYBOARD_SYSRQ					KEY_INPUT_SYSRQ			// PrintScreenキー
#define 	KEYBOARD_PAUSE					KEY_INPUT_PAUSE			// PauseBreakキー
#define 	KEYBOARD_LWIN					KEY_INPUT_LWIN			// 左Ｗｉｎキー
#define 	KEYBOARD_RWIN					KEY_INPUT_RWIN			// 右Ｗｉｎキー

#define 	KEYBOARD_NUMLOCK				KEY_INPUT_NUMLOCK		// テンキー０
#define 	KEYBOARD_NUMPAD0				KEY_INPUT_NUMPAD0		// テンキー０
#define 	KEYBOARD_NUMPAD1				KEY_INPUT_NUMPAD1		// テンキー１
#define 	KEYBOARD_NUMPAD2				KEY_INPUT_NUMPAD2		// テンキー２
#define 	KEYBOARD_NUMPAD3				KEY_INPUT_NUMPAD3		// テンキー３
#define 	KEYBOARD_NUMPAD4				KEY_INPUT_NUMPAD4		// テンキー４
#define 	KEYBOARD_NUMPAD5				KEY_INPUT_NUMPAD5		// テンキー５
#define 	KEYBOARD_NUMPAD6				KEY_INPUT_NUMPAD6		// テンキー６
#define 	KEYBOARD_NUMPAD7				KEY_INPUT_NUMPAD7		// テンキー７
#define 	KEYBOARD_NUMPAD8				KEY_INPUT_NUMPAD8		// テンキー８
#define 	KEYBOARD_NUMPAD9				KEY_INPUT_NUMPAD9		// テンキー９
#define 	KEYBOARD_MULTIPLY				KEY_INPUT_MULTIPLY		// テンキー＊キー
#define 	KEYBOARD_ADD					KEY_INPUT_ADD			// テンキー＋キー
#define 	KEYBOARD_SUBTRACT				KEY_INPUT_SUBTRACT		// テンキー－キー
#define 	KEYBOARD_DECIMAL				KEY_INPUT_DECIMAL		// テンキー．キー
#define 	KEYBOARD_DIVIDE					KEY_INPUT_DIVIDE		// テンキー／キー
#define 	KEYBOARD_NUMPADENTER			KEY_INPUT_NUMPADENTER	// テンキーのエンターキー

#define 	KEYBOARD_F1						KEY_INPUT_F1			// Ｆ１キー
#define 	KEYBOARD_F2						KEY_INPUT_F2			// Ｆ２キー
#define 	KEYBOARD_F3						KEY_INPUT_F3			// Ｆ３キー
#define 	KEYBOARD_F4						KEY_INPUT_F4			// Ｆ４キー
#define 	KEYBOARD_F5						KEY_INPUT_F5			// Ｆ５キー
#define 	KEYBOARD_F6						KEY_INPUT_F6			// Ｆ６キー
#define 	KEYBOARD_F7						KEY_INPUT_F7			// Ｆ７キー
#define 	KEYBOARD_F8						KEY_INPUT_F8			// Ｆ８キー
#define 	KEYBOARD_F9						KEY_INPUT_F9			// Ｆ９キー
#define 	KEYBOARD_F10					KEY_INPUT_F10			// Ｆ１０キー
#define 	KEYBOARD_F11					KEY_INPUT_F11			// Ｆ１１キー
#define 	KEYBOARD_F12					KEY_INPUT_F12			// Ｆ１２キー

#define 	KEYBOARD_A						KEY_INPUT_A				// Ａキー
#define 	KEYBOARD_B						KEY_INPUT_B				// Ｂキー
#define 	KEYBOARD_C						KEY_INPUT_C				// Ｃキー
#define 	KEYBOARD_D						KEY_INPUT_D				// Ｄキー
#define 	KEYBOARD_E						KEY_INPUT_E				// Ｅキー
#define 	KEYBOARD_F						KEY_INPUT_F				// Ｆキー
#define 	KEYBOARD_G						KEY_INPUT_G				// Ｇキー
#define 	KEYBOARD_H						KEY_INPUT_H				// Ｈキー
#define 	KEYBOARD_I						KEY_INPUT_I				// Ｉキー
#define 	KEYBOARD_J						KEY_INPUT_J				// Ｊキー
#define 	KEYBOARD_K						KEY_INPUT_K				// Ｋキー
#define 	KEYBOARD_L						KEY_INPUT_L				// Ｌキー
#define 	KEYBOARD_M						KEY_INPUT_M				// Ｍキー
#define 	KEYBOARD_N						KEY_INPUT_N				// Ｎキー
#define 	KEYBOARD_O						KEY_INPUT_O				// Ｏキー
#define 	KEYBOARD_P						KEY_INPUT_P				// Ｐキー
#define 	KEYBOARD_Q						KEY_INPUT_Q				// Ｑキー
#define 	KEYBOARD_R						KEY_INPUT_R				// Ｒキー
#define 	KEYBOARD_S						KEY_INPUT_S				// Ｓキー
#define 	KEYBOARD_T						KEY_INPUT_T				// Ｔキー
#define 	KEYBOARD_U						KEY_INPUT_U				// Ｕキー
#define 	KEYBOARD_V						KEY_INPUT_V				// Ｖキー
#define 	KEYBOARD_W						KEY_INPUT_W				// Ｗキー
#define 	KEYBOARD_X						KEY_INPUT_X				// Ｘキー
#define 	KEYBOARD_Y						KEY_INPUT_Y				// Ｙキー
#define 	KEYBOARD_Z						KEY_INPUT_Z				// Ｚキー

#define 	KEYBOARD_0 						KEY_INPUT_0 			// ０キー
#define 	KEYBOARD_1						KEY_INPUT_1				// １キー
#define 	KEYBOARD_2						KEY_INPUT_2				// ２キー
#define 	KEYBOARD_3						KEY_INPUT_3				// ３キー
#define 	KEYBOARD_4						KEY_INPUT_4				// ４キー
#define 	KEYBOARD_5						KEY_INPUT_5				// ５キー
#define 	KEYBOARD_6						KEY_INPUT_6				// ６キー
#define 	KEYBOARD_7						KEY_INPUT_7				// ７キー
#define 	KEYBOARD_8						KEY_INPUT_8				// ８キー
#define 	KEYBOARD_9						KEY_INPUT_9				// ９キー

} // namespace e
} // namespace common
} // namespace nn

//---------------------------------------------------------------------------
BOOL masterIsMainLoop( MASTER * master);
BOOL masterKeyTouchTask( MASTER * master);
void masterKeyTask( KEY_WORK * keyWork);
void masterKeyReset( KEY_WORK * key);
void masterKeyboardTask( KEYBOARD_WORK * keyboardWork);

BOOL masterGetGameContinue( MASTER * master);
void masterSetGamecontinue( MASTER * master, BOOL flag);
//---------------------------------------------------------------------------

#endif //__KEY_WORK_H__
