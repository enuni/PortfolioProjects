
#ifndef __TASK_H__
#define __TASK_H__

#define TASK_MAX (256)			//�^�X�N�̐�
#define TASK_PRIORITY_MAX (256)			//�^�X�N�̃v���C�I���e�B�ő�l
#define TASK_DEFAULT_PRIORITY (128)			//�^�X�N�̃f�t�H���g�v���C�I���e�B

#define TASK_FLAGS_DELETE	(0x00000002)			//�f���[�g

typedef struct _task TASK;
struct _task {
	TASK * parent;			//�e
	TASK * childTop;			//���j
	TASK * childBottom;			//����
	TASK * brotherUp;			//�Z
	TASK * brotherDown;			//��
	TASK * next;			//�ėp�̃����N
	void (* func)(TASK *);			//�����ւ̃|�C���^
	void (* destructor)(TASK *);			//�f�X�g���N�^
	
	int flags;			//�t���Obit��
	int priority;			//���s�D�揇��
	
	void * work;			//�^�X�N�ŗL�̃��[�N
};

//---------------------------------------------------------------------------

void taskInit( void);
TASK * taskCreate( void (* func)(TASK *), TASK * oya);
void taskDelete( TASK * task);
void taskExec(void);
void taskDump( TASK * task);

//---------------------------------------------------------------------------
#endif	//__TASK_H__

