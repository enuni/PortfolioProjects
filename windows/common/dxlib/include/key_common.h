#ifndef __KEY_COMMON_H__
#define __KEY_COMMON_H__

#include "master.h"

#include "key_common.h"
#include "touch.h"
#include "layout.h"

enum KEY_COMMON_CURSOR_ITEM_STATE {
	KEY_COMMON_CURSOR_ITEM_STATE_SKIP,			// �����ɃJ�[�\�����ړ�����ƃX�L�b�v
	KEY_COMMON_CURSOR_ITEM_STATE_NORMAL,		// �����ɃJ�[�\�����ړ����Ă��������Ȃ�
	KEY_COMMON_CURSOR_ITEM_STATE_SAME_SKIP,		// �����ɃJ�[�\�����ړ����Ĉړ��O�Ɠ���STATE�Ȃ�X�L�b�v
	KEY_COMMON_CURSOR_ITEM_STATE_H_MOVE_ONLY,	// �����ɃJ�[�\�������鎞�͐����ɂ����ړ����Ȃ�
	KEY_COMMON_CURSOR_ITEM_STATE_V_MOVE_ONLY,	// �����ɃJ�[�\�������鎞�͐����ɂ����ړ����Ȃ�

	KEY_COMMON_CURSOR_ITEM_STATE_MAX
};

enum KC_SKIP {
	KC_SKIP,	// KEY_COMMON_CURSOR_ITEM_STATE_SKIP
	KC_NORM,	// KEY_COMMON_CURSOR_ITEM_STATE_NORMAL
	KC_SAME,	// KEY_COMMON_CURSOR_ITEM_STATE_SAME_SKIP
	KC_HONL,	// KEY_COMMON_CURSOR_ITEM_STATE_H_MOVE_ONLY
	KC_VONL,	// KEY_COMMON_CURSOR_ITEM_STATE_V_MOVE_ONLY

	KC_MAX		// KEY_COMMON_CURSOR_ITEM_STATE_MAX
};

BOOL key_commonIsLayoutTouch( TOUCH_WORK * touch, LAYOUT * layout);
BOOL key_commonIsXywhTouch( TOUCH_WORK * touch, int x, int y, int w, int h);
BOOL key_commonIsLayoutTouchSimple( TOUCH_WORK * touch, int layoutId);
BOOL key_commonIsLayoutTouchSimpleShift( TOUCH_WORK * touch,
	int layoutIdBase, int layoutId
);

//Max�� ���ڂ̂Ƃ�ő�l �͂� ������ �̏ꍇ 1
BOOL key_commonCursorMoveLR( KEY_WORK * key, int xMax, int * cursorX);
BOOL key_commonCursorMoveUpDown( KEY_WORK * key, int yMax, int * cursorY);
BOOL key_commonCursorMove( KEY_WORK * key, int xMax, int yMax, int * cursorX, int * cursorY);
BOOL key_commonCUrsorMoveStateTbl(
	int * cursorX, int * cursorY, KEY_WORK * key, int xMax, int yMax, int * stateTbl
);

#endif //__KEY_COMMON_H__

