// CTRL ��u��
// ctrl ��u��
// Ctrl ��u��
// yb ��u��
// main ��u��

#ifndef __CTRL_H__
#define __CTRL_H__

#include "task_save.h"
#include "master.h"
//---------------------------------------------------------------------------
// �O���錾
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {

namespace e {
namespace ctrl {

enum STATE {
	STATE_NONE,
	STATE_EDIT,
	
	STATE_MAX
};

enum LOADING {
	LOADING_AYA,
	LOADING_MARISA,
	LOADING_REIMU,
	LOADING_TENKO,
	LOADING_00,		// �얲2
	LOADING_01,		// ���~���A&�t����1
	LOADING_SANAE,	// ���c
	LOADING_FLAN,	// �t�����i�P�́j
	LOADING_02,		// ���~���A&�t����2
	
	LOADING_MAX
};

//���[�f�B���O��ʏ��
struct LOADING_SCREEN_PARAMS {
	int text_x;
	int text_y;
	int text_size;
	DWORD text_color;
	char* text;

	int bar_x;
	int bar_y;
	int bar_size;
	DWORD bar_color;

	int window_x;
	int window_y;
	int window_w;
	int window_h;
	DWORD window_color;
	u16 window_a;
};

} // namespece ctrl
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CCtrl : public TASK_SAVE {
	
public:
	CCtrl( TASK_SAVE * oya);
	virtual ~CCtrl();
	void task();
	void disp();
	
	void start();
	void changeWindowSize( int w, int h);
	
	void saveMain();

	void changeWindowSizeAfter();
	void initializeEffectCtrl();

	void loadImage( int mode);
	void releaseImage( int mode);
	void saveDataInit();
	void saveDataCorrect();
	
	void loadLoadingImg();
	void releaseLoadingImg();
	void dispLoadingScreen(int& frameCnt, const nn::yb::e::ctrl::LOADING_SCREEN_PARAMS& params) ;//���[�f�B���O��ʕ`��
	void fullscreenTask();

	int state;
	int oldState;
	int waitCnt;
	
	MASTER * master;

	nn::yb::edit::CEdit * edit;
	nn::yb::edit::CEditMenu * editMenu;

	nn::yb::common::CYbSaveManager * saveManager;
	nn::yb::main::CSaveData * saveData;
	char saveDataFileName[32];

	nn::yb::stage::CCamera * camera;

	nn::yb::main::CMenu * menu;
	
	nn::yb::common::CYbSaveManager * saveManagerEdit;

	char * holeData[256];
	int holeDataSize[256];
	int holeDataN;
	
	int loading_image_id;//�����_���œǂݍ��ރ��[�h�摜ID

	double dispFps;

	int fullscreenGraph;
	int displayW;
	int displayH;
	int drawDstW;
	int drawDstH;
	int drawGlobalW;
	int drawGlobalH;
}; // CCtrl
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __CTRL_H__
