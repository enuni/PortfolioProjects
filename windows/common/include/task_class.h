
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
	
	TASK_CLASS * parent;			//�e
	TASK_CLASS * childTop;			//���j
	TASK_CLASS * childBottom;			//����
	TASK_CLASS * brotherUp;			//�Z
	TASK_CLASS * brotherDown;			//��
	TASK_CLASS * next;			//�ėp�̃����N
	TASK_EXEC exec;			//task()���I�[�o�[���C�h�����Ɏg�����߂̃����o�֐��|�C���^
	
	int flags;			//�t���Obit��
	int priority;			//���s�D�揇��

	virtual void task();
	static void init();
	static void addTask(TASK_CLASS *parent, TASK_CLASS *task);
	void constructCommon();
	
	static void taskExec(void);
	void taskDelete(void);
	void dump();
	
	static ::TASK_CLASS * sRoot;					// �S�Ẵ^�X�N�̐�c
	
private:
	static TASK_CLASS * sPriorityTbl[TASK_PRIORITY_MAX];	// �^�X�N �D�揇�ʃe�[�u��
	static TASK_CLASS * sPriorityTblBitmap[TASK_PRIORITY_MAX];// �^�X�N �D�揇�ʃe�[�u�� �쐬���g�p
	
	static void task_delete( TASK_CLASS * task);
	static void check_delete( TASK_CLASS * task);
	static void set_priority_tbl( TASK_CLASS * task);
	
};

#endif //__TASK_CLASS_H__

