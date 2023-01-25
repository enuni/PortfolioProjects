#ifndef __SLEEP_H__
#define __SLEEP_H__

#include "type.h"

//������
void sleepInit( void);
//���t���[���Ăяo��
void sleepTask( void);
//�X���[�v�ɓ���O�̃R�[���o�b�N��o�^����
void sleepAddSleeStartCallBack( void (* func)( void *), void * arg);
//�X���[�v���畜�A�����R�[���o�b�N�֐���o�^����
void sleepAddSleepEndCallBack( void (* func)( void *), void * arg);
//�N�� �X���[�v LCD-OFF���畜�A�� �}�C�N���I���ɂ������Ԃ��Z�b�g
//�i�X���[�v LCD-OFF���畜�A���̓R�[���o�b�N SleepTask()����Ă΂��j
void sleepSetSleepEndMilliSec( void);
//�N�� �X���[�v LCD-OFF���畜�A�� �}�C�N���I���ɂ��Ă���o�߂����~���b��Ԃ�
u64 sleepGetSleepEndMilliSec( void);
//�X���[�v�ɓ��邩�ǂ����̃t���O���Z�b�g���� TRUE = ����Ȃ� FALSE = ����
void sleepSetSleepWaitFlag( BOOL flag);
//�X���[�v���[�h���ʐM�� LCDOFF��Ԃ��ǂ���
BOOL sleepIsModeNet( void);

void sleepBackLightOffAsync( void);
void sleepBackLightOnAsync( void);

#endif //__SLEEP_H__

