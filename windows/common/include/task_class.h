
#ifndef __TASK_CLASS_H__
#define __TASK_CLASS_H__

#include "task.h"

class TASK_CLASS;
class TASK_ROOT;

typedef void (TASK_CLASS::*TASK_EXEC)();

class TASK_CLASS {
	
public:
	TASK_CLASS();
	TASK_CLASS( ::TASK_CLASS * parent);
	virtual ~TASK_CLASS();
	
	TASK_CLASS * parent;			//親
	TASK_CLASS * childTop;			//長男
	TASK_CLASS * childBottom;			//末弟
	TASK_CLASS * brotherUp;			//兄
	TASK_CLASS * brotherDown;			//弟
	TASK_CLASS * next;			//汎用のリンク
	TASK_EXEC exec;			//task()をオーバーライドせずに使うためのメンバ関数ポインタ
	
	int flags;			//フラグbit列
	int priority;			//実行優先順位

	virtual void task();
	static void init();
	static void addTask(TASK_CLASS *parent, TASK_CLASS *task);
	void constructCommon();
	
	static void taskExec(void);
	void taskDelete(void);
	void dump();
	
	static ::TASK_CLASS * sRoot;					// 全てのタスクの先祖
	
private:
	static TASK_CLASS * sPriorityTbl[TASK_PRIORITY_MAX];	// タスク 優先順位テーブル
	static TASK_CLASS * sPriorityTblBitmap[TASK_PRIORITY_MAX];// タスク 優先順位テーブル 作成時使用
	
	static void task_delete( TASK_CLASS * task);
	static void check_delete( TASK_CLASS * task);
	static void set_priority_tbl( TASK_CLASS * task);
	
};

#endif //__TASK_CLASS_H__

