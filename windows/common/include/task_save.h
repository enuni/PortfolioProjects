#ifndef __TASK_SAVE_H__
#define __TASK_SAVE_H__

#include "task.h"
#include "save_obj_base.h"
#include "save_manager.h"

//-----------------------------------------------------------------------------

#define TASK_FLAGS_ENABLE	(0x00000004)			//実行する表示する状態にする
#define TASK_FLAGS_DISABLE	(0x00000008)			//実行しない表示しない状態にする

enum TASK_STATE {
	TASK_STATE_ENABLE,
	TASK_STATE_DISABLE,

	TASK_STATE_MAX
};

//-----------------------------------------------------------------------------

class TASK_SAVE;

typedef void (TASK_SAVE::*TASK_SAVE_EXEC)();

class TASK_SAVE : public ::nn_common::SaveObjBase {
	
public:
	TASK_SAVE();
	TASK_SAVE( ::nn_common::SaveManager * m);
	TASK_SAVE( ::TASK_SAVE * parant);
	TASK_SAVE( ::TASK_SAVE * parant, ::nn_common::SaveManager * m);
	virtual ~TASK_SAVE();
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void constructCommon();
	void dump();
	void dump_in();
	static bool isLive( ::TASK_SAVE * t);
	static void excludeNotSaveTask( TASK_SAVE * t, ::nn_common::SaveManager * sm);
		// セーブする前にセーブするTASKのrootを渡して呼ぶ事で、root以下のTASK treeからセーブしないTASKを除外する。
		// セーブ後にrestoreNotSaveTask()を呼んで必ず元に戻すようにする。
	static void restoreNotSaveTask();		// excludeNotSaveTask() → セーブ → restoreNotSaveTask()と呼ぶ
	
	TASK_SAVE * parent;			//親
	TASK_SAVE * childTop;		//長男
	TASK_SAVE * childBottom;	//末弟
	TASK_SAVE * brotherUp;		//兄
	TASK_SAVE * brotherDown;	//弟
	TASK_SAVE * next;			//汎用のリンク
	TASK_SAVE_EXEC exec;		//task()をオーバーライドせずに使うためのメンバ関数ポインタ
	
	int flags;					//フラグbit列
	int priority;				//実行優先順位
	int taskState;				//状態

	virtual void task();
	virtual void disp();
	
	static void taskExec(void);
	static void taskDispExec(void);
	void taskDelete(void);
	static void addTask( TASK_SAVE * parent, TASK_SAVE * task);
	void taskDeleteInstant(void);
	
	static void init();
	void taskEnable();
	void taskDisable();
	static void setDumpLogIndex( int index);
	
	static ::TASK_SAVE * sRoot;					// 全てのタスクの先祖
	static ::TASK_SAVE * sNotSaveTasksRoot;		// excludeNotSaveTask() → restoreNotSaveTask()で使う
	static bool sDispFlag;
	static int sDumpLogIndex;

protected:
	static ::nn_common::DATARECODE dataRecode[];
	
private:
	static TASK_SAVE * sPriorityTbl[TASK_PRIORITY_MAX];	// タスク 優先順位テーブル
	static TASK_SAVE * sPriorityTblBitmap[TASK_PRIORITY_MAX];// タスク 優先順位テーブル 作成時使用
	
	static void task_delete( TASK_SAVE * task);
	static void check_flags( TASK_SAVE * task);
	static void set_priority_tbl( TASK_SAVE * task);
	bool is_live( ::TASK_SAVE * save);
	static void task_set_state( TASK_SAVE * task, int state);
	static void task_set_state_in( TASK_SAVE * task, int state);
	static void task_exclude( ::TASK_SAVE * task);
	static void task_include( ::TASK_SAVE * task);
	static void task_restore( ::TASK_SAVE * task);
	
};

#endif //__TASK_SAVE_H__

