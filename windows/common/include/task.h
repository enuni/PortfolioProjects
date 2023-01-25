
#ifndef __TASK_H__
#define __TASK_H__

#define TASK_MAX (256)			//タスクの数
#define TASK_PRIORITY_MAX (256)			//タスクのプライオリティ最大値
#define TASK_DEFAULT_PRIORITY (128)			//タスクのデフォルトプライオリティ

#define TASK_FLAGS_DELETE	(0x00000002)			//デリート

typedef struct _task TASK;
struct _task {
	TASK * parent;			//親
	TASK * childTop;			//長男
	TASK * childBottom;			//末弟
	TASK * brotherUp;			//兄
	TASK * brotherDown;			//弟
	TASK * next;			//汎用のリンク
	void (* func)(TASK *);			//処理へのポインタ
	void (* destructor)(TASK *);			//デストラクタ
	
	int flags;			//フラグbit列
	int priority;			//実行優先順位
	
	void * work;			//タスク固有のワーク
};

//---------------------------------------------------------------------------

void taskInit( void);
TASK * taskCreate( void (* func)(TASK *), TASK * oya);
void taskDelete( TASK * task);
void taskExec(void);
void taskDump( TASK * task);

//---------------------------------------------------------------------------
#endif	//__TASK_H__

