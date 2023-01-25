// TITLE ��u��
// title ��u��
// Title ��u��
// ts ��u��
// title ��u��

#ifndef __TITLE_H__
#define __TITLE_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
//---------------------------------------------------------------------------
namespace nn {
namespace ts {

// enum
namespace e {
namespace title {

enum STATE {
	STATE_NONE,
	STATE_WARNING,		// �񎟑n�샏�[�j���O
	STATE_LOGO,			// agrippa n2NSoft
	STATE_LOGO_OUT,		// agrippa n2NSoft
	STATE_TITLE_IN,		// �^�C�g����ʃt�F�[�h�C��
	STATE_TITLE_DIRECT, // �^�C�g����ʉ��o
	STATE_TITLE_WAIT,	// �^�C�g����ʓ��͑҂�
	STATE_NEXT,			// �^�C�g����ʃt�F�[�h�A�E�g
	
	STATE_MAX
};

} // namespece title
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace title {

typedef struct {
	int layoutIdAgrippa;
	int layoutIdN2nsoft;
	int animeIdTitleTextIn;
} TITLE_DISP_MEMBER;

typedef struct _title_link_member {
	nn::ts::common::CControl * ctrl;
} TITLE_LINK_MEMBER;
	
class CTitle : public TASK_SAVE {
	
public:
	CTitle( TASK_SAVE * oya);
	CTitle( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CTitle();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	void taskWarningLogo();
	void taskTitle();
	static void decWaitCntCb( void * titleArg);
	void decWaitCnt();
	static void nextCb( void * titleArg);
	void next();
	void createMoney( int xStart, int xWidth, int yStart, int yHeight,
		void (* cb)(void *), void * cbArg
	);
	void startTitleTextIn();

	static void taskMoneyCB( void *particleArg, void *titleArg);
	static void taskMoney( void *particleArg);
	static void dispMoneyCB( void *particleArg, void *titleArg);
	static void dispMoney( void *particleArg);
	void startGame();

	int state;
	int oldState;
	int waitCnt;

	unsigned int stateFrameCnt;

	static TITLE_DISP_MEMBER dm;

	nn::ts::title::TITLE_LINK_MEMBER lm;
	
}; // CTitle
	 
} // namespece title

} // namespace ts
} // namespace nn
	

#endif // __TITLE_H__
