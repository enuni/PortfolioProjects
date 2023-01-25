#ifndef __TECO__
#pragma once

// ゲーム固有のコマンド定義はこのファイルをゲーム名_script_cmd.hなどで持っていく
// namespaceを更新する
// script_cmd.hのincludeのコメントアウトを外しこのファイルをincludeする(パスはゲームのディレクトリに合せて適宜修正)
// RR_CMD_STARTのコメントアウトを外す
// enum CMD_OBJ以下のコマンドIDを削除してゲーム固有のものを追加する
// macro obj_simple以下のスクリプトマクロ定義を削除してゲーム固有のものを追加する
// script共通のheaderファイルでゲーム名_script_cmd.hをincludeしてscript_cmd.h
// ゲーム内のCScriptインスタンスにint (* extFunc)( void * instance, void * script, int id, int * arg); と void * extFuncInstance; を設定しておくと
// 追加したコマンドIDをスクリプトマクロを呼ぶと、extFuncが呼ばれる
/*
// extFunc sample
int scriptFunc(void *instance, void * script, int id, int *arg) {

	(void)instance;
	(void)script;
	
	switch( id ) {
	case nn::sample::e::script::SAMPLE_CMD_SAMPLE:
		{
			int value = arg[0];
			printf( "value = %d\n", value);
			return nn::common::teco::CTecoVM::RET_COMMAND_OK_GO_NEXT;
		}
	}
	return nn::common::teco::CTecoVM::RET_COMMAND_ERROR_STOP;
	
}
*/

namespace nn {
namespace dxlib {
namespace e {
namespace script {
#endif //__TECO__

//#include "..\..\..\common\dxlib\include\script_cmd.h"

// 命令の種類
enum
{
/*
#ifndef __TECO__
	UNIQUE_CMD_START = nn::dxlib::e::script::CMD_MAX,
#endif //__TECO__
#ifndef WIN32
	UNIQUE_CMD_START = CMD_MAX,
#endif //WIN32
*/
	CMD_OBJ,				// obj
	CMD_OBJ_SIMPLE,			// obj_simple
	CMD_OBJ_CENTER_SIMPLE,	// obj_center_simple
	CMD_OBJ_SIMPLE_COLOR,	// obj_simple_color
	CMD_OBJ_DELETE,			// obj_delete
	CMD_OBJ_CHANGE_OBJID,	// obj_change_objid
	CMD_BOARD,				// board
	CMD_WAIT,				// wait
	CMD_MSG,				// msg
	CMD_MSG_REMAIN,			// msg_remain
	CMD_MSG_ASYNC,			// msg_async
	CMD_MSG_DELETE,			// msg_edelete
	CMD_WAIT_ASYNC_MSG,		// wait_async_msg
	CMD_ANIME,				// anime
	CMD_ANIME_SIMPLE,		// anime_simple
	CMD_PRINT,				// print
	CMD_BG,					// bg
	CMD_CHR,				// chr
	CMD_CHR_ERASE,			// chr_erase
	CMD_BGM,				// bgm
	CMD_FADEIN,				// fadein
	CMD_FADEOUT,			// fadeout
	CMD_SELECT,				// select
	CMD_SELECT_TEXT,		// select_text
	CMD_ADD_SELECT_TEXT,	// add_select_text
	CMD_CLEAR_SELECT_TEXT,	// clear_select_text
	CMD_NEXT_SCRIPT,		// next_script
	CMD_NEXT_SCENE,			// next_scene

	CMD_TALK,				// talk
	
	CMD_MAX,
};

#ifndef WIN32
macro obj_simple int index, int layoutId
{
	Com CMD_OBJ_SIMPLE, index, layoutId
}

macro obj_center_simple int index, int layoutId
{
	Com CMD_OBJ_CENTER_SIMPLE, index, layoutId
}

macro obj_simple_color int index, int layoutId, int r, int g, int b, int blend
{
	Com CMD_OBJ_SIMPLE_COLOR, index, layoutId, r, g, b, blend
}

macro obj int index, int objId, int x, int y, int z
{
	Com CMD_OBJ, index, objId, x, y, z
}

macro board int index, int x, int y, int z, int w, int h, int r, int g, int b, int a
{
	Com CMD_BOARD, index, x, y, z, w, h, r, g, b, a
}

macro obj_delete int index
{
	Com CMD_OBJ_DELETE, index
}

macro obj_change_objid int index, int objId
{
	Com CMD_OBJ_CHANGE_OBJID, index, objId
}

macro wait int frame
{
	Com CMD_WAIT, frame
}

macro msg str text
{
	Com CMD_MSG, text
}

macro msg_remain int index, str text
{
	Com CMD_MSG_REMAIN, index, text
}

macro msg_async int index, str text
{
	Com CMD_MSG_ASYNC, index, text
}

macro msg_delete int index
{
	Com CMD_MSG_DELETE, index
}

macro wait_async_msg int index
{
	Com CMD_WAIT_ASYNC_MSG, index
}

macro select
{
	Com CMD_SELECT
}

macro next_script str file_name, int seg_no
{
	Com CMD_NEXT_SCRIPT, file_name, seg_no
}

macro next_scene str file_name, int seg_no
{
	Com CMD_NEXT_SCENE, file_name, seg_no
}

macro anime int anime_id, int x, int y, int z
{
	Com CMD_ANIME, anime_id, x, y, z
}

macro anime_simple int anime_id, int layout_id
{
	Com CMD_ANIME_SIMPLE, anime_id, layout_id
}

macro print str text, int var
{
	Com CMD_PRINT, text, var
}

macro select_text int x, int y
{
	Com CMD_SELECT_TEXT, x, y
}

macro add_select_text str text
{
	Com CMD_ADD_SELECT_TEXT, text
}

macro clear_select_text
{
	Com CMD_CLEAR_SELECT_TEXT
}

#endif

#ifndef __TECO__
} // script
} // e
} // namespace dxlib
} // namespace nn
#endif //__TECO__
