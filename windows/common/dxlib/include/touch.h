#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "type.h"

//#define __PRC__			//�}�`�F��������

#ifndef __TOUCH_WORK__
#define __TOUCH_WORK__

typedef struct _touch_work {
	s16 touchPoint;	//�^�b�`�C�x���g �ꏊ�����m
	s16 touchDisp;	//�^�b�`�C�x���g �ꏊ���s���m�ł�
	s16 first_x;	//���݂̃^�b�`�C�x���g�ōŏ��Ƀ^�b�`�����ꏊ��x���W
	s16 first_y;	//���݂̃^�b�`�C�x���g�ōŏ��Ƀ^�b�`�����ꏊ��y���W
	s16 release_x;	//���݂̃^�b�`�C�x���g�ŕ������ꏊ��x���W
	s16 release_y;	//���݂̃^�b�`�C�x���g�ŕ������ꏊ��y���W
	s16 x;	//���݃^�b�`�y�����^�b�`���Ă���ꏊ��x���W
	s16 y;	//���݃^�b�`�y�����^�b�`���Ă���ꏊ��y���W
	
	s16 slideStartX;
	s16 slideStartY;
} TOUCH_WORK;

#endif //__TOUCH_WORK__

#define TOUCH_TOUCH					0x0001	//�^�b�`�����u��
#define TOUCH_TOUCH_LONG			0x0002	//������
#define TOUCH_TOUCH_LONG_LONG		0x0004	//��������
#define TOUCH_DOUBLE_TOUCH			0x0008	//�_�u���N���b�N
#define TOUCH_PRESS					0x0010	//�^�b�`��
#define TOUCH_PULSE					0x0020	//�A���^�b�`����莞�Ԃ��Ƃɔ���
#define TOUCH_RELEASE				0x0040	//�������u��
#define TOUCH_RELEASE_LONG			0x0080	//�����Ă���o��
#define TOUCH_RELEASE_LONG_LONG		0x0100	//�����Ă��璴�o��

#define TOUCH_SLIDE_UP				0x0200	//��փX���C�h
#define TOUCH_SLIDE_DOWN			0x0400	//���փX���C�h
//---------------------------------------------------------------------------

void touchInit( void);
void touchTask( TOUCH_WORK * touchWork);
void touchSetTouchLongNum( u8 setNum);

#ifdef __PRC__
//�}�`�F���̏����� �J�n ����heapAlloc()���Ă� ���������m��
//���� �p�^�[���̃��X�g�� .cpp �̐ÓI�Ȃ��̂������N���� �����I�Ƀt�@�C�����\��
void touchStartPrc( void);
//�I������ StartPrc() �Ŋm�ۂ��� ���������J������
void touchEndPrc( void);
//touchTask()�� �}�`�F���p�̃^�b�`�̗������L�^����t���O��ݒ�
void touchSetPrcAppendPointFlag( BOOL flag);
//�L�^�����^�b�`�������� kind �Ŏw�肵���C�ӂ̃p�^�[���̒��� �ł��߂������� resultCharArray �ɂ���� kind = (0xffffffffU)�őS�Ẵp�^�[��
//�L���ȕ������F���ł����� TRUE ��Ԃ�
BOOL touchGetPrcBlock( char * resultCharArray, u32 kind);

//�^�b�`����������
void touchClearPrc( void);

//�}�`�F���̓��͔͈͂�ݒ肷��
void touchSetPrcArea( int x1, int y1, int x2, int y2);
//�}�`�F���ɓ��͂���Ă���_�̌���Ԃ�
int touchGetPrcPointN( void);
//�}�`�F���ɓ��͂���Ă��� i �Ԗڂ� x���W
int touchGetPrcPointX( int i);
//�}�`�F���ɓ��͂���Ă��� i �Ԗڂ� y���W
int touchGetPrcPointY( int i);
#endif //__PRC__
//---------------------------------------------------------------------------

#endif //__TOUCH_H__



