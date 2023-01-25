
#ifndef __MASTER_H__
#define __MASTER_H__

#include "type.h"

//---------------------------------------------------------------------------

#define MASTER_CONFIG_PAD_N (32)
#define MASTER_CONFIG_KEY_N (14)

typedef struct {
	u32 trigger;	// �������u��
	u32 press;		// ������Ă�����
	u32 pulse;		// �������u�ԂƂ��΂炭�����Ă���ƈ��Ԋu�Ŕ���
	u32 hold;		// ���΂炭�����Ă���Ɩ��t���[������
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
	s16 touchPoint;	//�^�b�`�C�x���g �ꏊ�����m
	s16 touchDisp;	//�^�b�`�C�x���g �ꏊ���s���m�ł�
	s16 first_x;	//���݂̃^�b�`�C�x���g�ōŏ��Ƀ^�b�`�����ꏊ��x���W
	s16 first_y;	//���݂̃^�b�`�C�x���g�ōŏ��Ƀ^�b�`�����ꏊ��y���W
	s16 release_x;	//���݂̃^�b�`�C�x���g�ŕ������ꏊ��x���W
	s16 release_y;	//���݂̃^�b�`�C�x���g�Řb�����ꏊ��y���W
	s16 x;	//���݃^�b�`�y�����^�b�`���Ă���ꏊ��x���W
	s16 y;	//���݃^�b�`�y�����^�b�`���Ă���ꏊ��y���W
	
	s16 slideStartX;
	s16 slideStartY;
} TOUCH_WORK;

#endif //__TOUCH_WORK__

typedef struct {
	KEY_WORK keyWork;	//���݂̃L�[�̏��
	TOUCH_WORK touchWork;	//���݂̃^�b�`�̏��
	KEYBOARD_WORK keyboardWork;	//���݂̃L�[�{�[�h�̏��
	u8 inputState;	//�L�[���[�h���^�b�`���[�h��
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

// �݊����̂��ߎc��
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

// �݊����̂��ߎc��
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

// 110610�ȍ~
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

#define 	KEYBOARD_BACK					KEY_INPUT_BACK			// �o�b�N�X�y�[�X�L�[
#define 	KEYBOARD_TAB					KEY_INPUT_TAB			// �^�u�L�[
#define 	KEYBOARD_RETURN					KEY_INPUT_RETURN		// �G���^�[�L�[

#define 	KEYBOARD_LSHIFT					KEY_INPUT_LSHIFT		// ���V�t�g�L�[
#define 	KEYBOARD_RSHIFT					KEY_INPUT_RSHIFT		// �E�V�t�g�L�[
#define 	KEYBOARD_LCONTROL				KEY_INPUT_LCONTROL		// ���R���g���[���L�[
#define 	KEYBOARD_RCONTROL				KEY_INPUT_RCONTROL		// �E�R���g���[���L�[
#define 	KEYBOARD_ESCAPE					KEY_INPUT_ESCAPE		// �G�X�P�[�v�L�[
#define 	KEYBOARD_SPACE					KEY_INPUT_SPACE			// �X�y�[�X�L�[
#define 	KEYBOARD_PGUP					KEY_INPUT_PGUP			// �o�������t�o�L�[
#define 	KEYBOARD_PGDN					KEY_INPUT_PGDN			// �o�������c�������L�[
#define 	KEYBOARD_END					KEY_INPUT_END			// �G���h�L�[
#define 	KEYBOARD_HOME					KEY_INPUT_HOME			// �z�[���L�[
#define 	KEYBOARD_LEFT					KEY_INPUT_LEFT			// ���L�[
#define 	KEYBOARD_UP						KEY_INPUT_UP			// ��L�[
#define 	KEYBOARD_RIGHT					KEY_INPUT_RIGHT			// �E�L�[
#define 	KEYBOARD_DOWN					KEY_INPUT_DOWN			// ���L�[
#define 	KEYBOARD_INSERT					KEY_INPUT_INSERT		// �C���T�[�g�L�[
#define 	KEYBOARD_DELETE					KEY_INPUT_DELETE		// �f���[�g�L�[

#define 	KEYBOARD_MINUS					KEY_INPUT_MINUS			// �|�L�[
#define 	KEYBOARD_YEN					KEY_INPUT_YEN			// ���L�[
#define 	KEYBOARD_PREVTRACK				KEY_INPUT_PREVTRACK		// �O�L�[
#define 	KEYBOARD_PERIOD					KEY_INPUT_PERIOD		// �D�L�[
#define 	KEYBOARD_SLASH					KEY_INPUT_SLASH			// �^�L�[
#define 	KEYBOARD_LALT					KEY_INPUT_LALT			// ���`�k�s�L�[
#define 	KEYBOARD_RALT					KEY_INPUT_RALT			// �E�`�k�s�L�[
#define 	KEYBOARD_SCROLL					KEY_INPUT_SCROLL		// ScrollLock�L�[
#define 	KEYBOARD_SEMICOLON				KEY_INPUT_SEMICOLON		// �G�L�[
#define 	KEYBOARD_COLON					KEY_INPUT_COLON			// �F�L�[
#define 	KEYBOARD_LBRACKET				KEY_INPUT_LBRACKET		// �m�L�[
#define 	KEYBOARD_RBRACKET				KEY_INPUT_RBRACKET		// �n�L�[
#define 	KEYBOARD_AT						KEY_INPUT_AT			// ���L�[
#define 	KEYBOARD_BACKSLASH				KEY_INPUT_BACKSLASH		// �_�L�[
#define 	KEYBOARD_COMMA					KEY_INPUT_COMMA			// �C�L�[
#define 	KEYBOARD_KANJI					KEY_INPUT_KANJI			// �����L�[
#define 	KEYBOARD_CONVERT				KEY_INPUT_CONVERT		// �ϊ��L�[
#define 	KEYBOARD_NOCONVERT				KEY_INPUT_NOCONVERT		// ���ϊ��L�[
#define 	KEYBOARD_KANA					KEY_INPUT_KANA			// �J�i�L�[
#define 	KEYBOARD_APPS					KEY_INPUT_APPS			// �A�v���P�[�V�������j���[�L�[
#define 	KEYBOARD_CAPSLOCK				KEY_INPUT_CAPSLOCK		// CaspLock�L�[
#define 	KEYBOARD_SYSRQ					KEY_INPUT_SYSRQ			// PrintScreen�L�[
#define 	KEYBOARD_PAUSE					KEY_INPUT_PAUSE			// PauseBreak�L�[
#define 	KEYBOARD_LWIN					KEY_INPUT_LWIN			// ���v�����L�[
#define 	KEYBOARD_RWIN					KEY_INPUT_RWIN			// �E�v�����L�[

#define 	KEYBOARD_NUMLOCK				KEY_INPUT_NUMLOCK		// �e���L�[�O
#define 	KEYBOARD_NUMPAD0				KEY_INPUT_NUMPAD0		// �e���L�[�O
#define 	KEYBOARD_NUMPAD1				KEY_INPUT_NUMPAD1		// �e���L�[�P
#define 	KEYBOARD_NUMPAD2				KEY_INPUT_NUMPAD2		// �e���L�[�Q
#define 	KEYBOARD_NUMPAD3				KEY_INPUT_NUMPAD3		// �e���L�[�R
#define 	KEYBOARD_NUMPAD4				KEY_INPUT_NUMPAD4		// �e���L�[�S
#define 	KEYBOARD_NUMPAD5				KEY_INPUT_NUMPAD5		// �e���L�[�T
#define 	KEYBOARD_NUMPAD6				KEY_INPUT_NUMPAD6		// �e���L�[�U
#define 	KEYBOARD_NUMPAD7				KEY_INPUT_NUMPAD7		// �e���L�[�V
#define 	KEYBOARD_NUMPAD8				KEY_INPUT_NUMPAD8		// �e���L�[�W
#define 	KEYBOARD_NUMPAD9				KEY_INPUT_NUMPAD9		// �e���L�[�X
#define 	KEYBOARD_MULTIPLY				KEY_INPUT_MULTIPLY		// �e���L�[���L�[
#define 	KEYBOARD_ADD					KEY_INPUT_ADD			// �e���L�[�{�L�[
#define 	KEYBOARD_SUBTRACT				KEY_INPUT_SUBTRACT		// �e���L�[�|�L�[
#define 	KEYBOARD_DECIMAL				KEY_INPUT_DECIMAL		// �e���L�[�D�L�[
#define 	KEYBOARD_DIVIDE					KEY_INPUT_DIVIDE		// �e���L�[�^�L�[
#define 	KEYBOARD_NUMPADENTER			KEY_INPUT_NUMPADENTER	// �e���L�[�̃G���^�[�L�[

#define 	KEYBOARD_F1						KEY_INPUT_F1			// �e�P�L�[
#define 	KEYBOARD_F2						KEY_INPUT_F2			// �e�Q�L�[
#define 	KEYBOARD_F3						KEY_INPUT_F3			// �e�R�L�[
#define 	KEYBOARD_F4						KEY_INPUT_F4			// �e�S�L�[
#define 	KEYBOARD_F5						KEY_INPUT_F5			// �e�T�L�[
#define 	KEYBOARD_F6						KEY_INPUT_F6			// �e�U�L�[
#define 	KEYBOARD_F7						KEY_INPUT_F7			// �e�V�L�[
#define 	KEYBOARD_F8						KEY_INPUT_F8			// �e�W�L�[
#define 	KEYBOARD_F9						KEY_INPUT_F9			// �e�X�L�[
#define 	KEYBOARD_F10					KEY_INPUT_F10			// �e�P�O�L�[
#define 	KEYBOARD_F11					KEY_INPUT_F11			// �e�P�P�L�[
#define 	KEYBOARD_F12					KEY_INPUT_F12			// �e�P�Q�L�[

#define 	KEYBOARD_A						KEY_INPUT_A				// �`�L�[
#define 	KEYBOARD_B						KEY_INPUT_B				// �a�L�[
#define 	KEYBOARD_C						KEY_INPUT_C				// �b�L�[
#define 	KEYBOARD_D						KEY_INPUT_D				// �c�L�[
#define 	KEYBOARD_E						KEY_INPUT_E				// �d�L�[
#define 	KEYBOARD_F						KEY_INPUT_F				// �e�L�[
#define 	KEYBOARD_G						KEY_INPUT_G				// �f�L�[
#define 	KEYBOARD_H						KEY_INPUT_H				// �g�L�[
#define 	KEYBOARD_I						KEY_INPUT_I				// �h�L�[
#define 	KEYBOARD_J						KEY_INPUT_J				// �i�L�[
#define 	KEYBOARD_K						KEY_INPUT_K				// �j�L�[
#define 	KEYBOARD_L						KEY_INPUT_L				// �k�L�[
#define 	KEYBOARD_M						KEY_INPUT_M				// �l�L�[
#define 	KEYBOARD_N						KEY_INPUT_N				// �m�L�[
#define 	KEYBOARD_O						KEY_INPUT_O				// �n�L�[
#define 	KEYBOARD_P						KEY_INPUT_P				// �o�L�[
#define 	KEYBOARD_Q						KEY_INPUT_Q				// �p�L�[
#define 	KEYBOARD_R						KEY_INPUT_R				// �q�L�[
#define 	KEYBOARD_S						KEY_INPUT_S				// �r�L�[
#define 	KEYBOARD_T						KEY_INPUT_T				// �s�L�[
#define 	KEYBOARD_U						KEY_INPUT_U				// �t�L�[
#define 	KEYBOARD_V						KEY_INPUT_V				// �u�L�[
#define 	KEYBOARD_W						KEY_INPUT_W				// �v�L�[
#define 	KEYBOARD_X						KEY_INPUT_X				// �w�L�[
#define 	KEYBOARD_Y						KEY_INPUT_Y				// �x�L�[
#define 	KEYBOARD_Z						KEY_INPUT_Z				// �y�L�[

#define 	KEYBOARD_0 						KEY_INPUT_0 			// �O�L�[
#define 	KEYBOARD_1						KEY_INPUT_1				// �P�L�[
#define 	KEYBOARD_2						KEY_INPUT_2				// �Q�L�[
#define 	KEYBOARD_3						KEY_INPUT_3				// �R�L�[
#define 	KEYBOARD_4						KEY_INPUT_4				// �S�L�[
#define 	KEYBOARD_5						KEY_INPUT_5				// �T�L�[
#define 	KEYBOARD_6						KEY_INPUT_6				// �U�L�[
#define 	KEYBOARD_7						KEY_INPUT_7				// �V�L�[
#define 	KEYBOARD_8						KEY_INPUT_8				// �W�L�[
#define 	KEYBOARD_9						KEY_INPUT_9				// �X�L�[

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
