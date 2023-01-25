#ifndef __TASK_SAVE_H__
#define __TASK_SAVE_H__

#include "task.h"
#include "save_obj_base.h"
#include "save_manager.h"

//-----------------------------------------------------------------------------

#define TASK_FLAGS_ENABLE	(0x00000004)			//���s����\�������Ԃɂ���
#define TASK_FLAGS_DISABLE	(0x00000008)			//���s���Ȃ��\�����Ȃ���Ԃɂ���

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
		// �Z�[�u����O�ɃZ�[�u����TASK��root��n���ČĂԎ��ŁAroot�ȉ���TASK tree����Z�[�u���Ȃ�TASK�����O����B
		// �Z�[�u���restoreNotSaveTask()���Ă�ŕK�����ɖ߂��悤�ɂ���B
	static void restoreNotSaveTask();		// excludeNotSaveTask() �� �Z�[�u �� restoreNotSaveTask()�ƌĂ�
	
	TASK_SAVE * parent;			//�e
	TASK_SAVE * childTop;		//���j
	TASK_SAVE * childBottom;	//����
	TASK_SAVE * brotherUp;		//�Z
	TASK_SAVE * brotherDown;	//��
	TASK_SAVE * next;			//�ėp�̃����N
	TASK_SAVE_EXEC exec;		//task()���I�[�o�[���C�h�����Ɏg�����߂̃����o�֐��|�C���^
	
	int flags;					//�t���Obit��
	int priority;				//���s�D�揇��
	int taskState;				//���

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
	
	static ::TASK_SAVE * sRoot;					// �S�Ẵ^�X�N�̐�c
	static ::TASK_SAVE * sNotSaveTasksRoot;		// excludeNotSaveTask() �� restoreNotSaveTask()�Ŏg��
	static bool sDispFlag;
	static int sDumpLogIndex;

protected:
	static ::nn_common::DATARECODE dataRecode[];
	
private:
	static TASK_SAVE * sPriorityTbl[TASK_PRIORITY_MAX];	// �^�X�N �D�揇�ʃe�[�u��
	static TASK_SAVE * sPriorityTblBitmap[TASK_PRIORITY_MAX];// �^�X�N �D�揇�ʃe�[�u�� �쐬���g�p
	
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

