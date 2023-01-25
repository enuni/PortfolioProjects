// config ��u��
// Config ��u��
// yb ��u��
// main ��u��

#include "nn_common.h"
#include "nn_common2.h"

#include "font.h"

#include "key_common.h"
#include "disp_common.h"
#include "sound_nns.h"

#include "sound_data.sadl"

#include "enum.h"
#include "yb_common.h"

#include "ctrl.h"
#include "save_data.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"
#include "animation_data.h"

#include "effect/effect_ctrl.h"

#include "config.h"
//---------------------------------------------------------------------------
nn::yb::main::CConfig::CConfig(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CConfig Create\n");
	this->state = nn::yb::e::config::STATE_NONE;
	this->oldState = nn::yb::e::config::STATE_NONE;
	this->waitCnt = 0;

	this->saveData = NULL;

	this->cursorX = 0;
	this->cursorY = 0;
	this->oldOriginalPadState = 0;
	this->itemN = 0;
	memset( this->configPad, 0, sizeof( this->configPad));
	memset( this->buttonNoTbl, 0, sizeof( this->buttonNoTbl));

	this->cursorYMain = 0;
	this->cursorXMain = 0;

	this->ctrl = NULL;
	this->endCb = NULL;
	this->endCbArg = NULL;

	memset( &this->oldConfig, 0, sizeof( this->oldConfig));

}
//---------------------------------------------------------------------------
nn::yb::main::CConfig::~CConfig() {

	::nn_commonLog( 1, "CConfig Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::main::CConfig::start() {

	::KEY_WORK * key = &this->master->keyWork;

	// �Z���N�g�{�^���͊���U��Ȃ�(�L�[�{�[�h��p)
	//this->itemN = 8;
	this->itemN = nn::yb::e::config::PAD_CONFIG_MAX;

	memcpy( this->configPad, this->saveData->configData.pad, sizeof( this->configPad));

	this->renewButtonNoTbl();
	
	this->oldOriginalPadState = key->originalPadState;

	memcpy( &this->oldConfig, &this->saveData->configData, sizeof( this->oldConfig));

	this->state = nn::yb::e::config::STATE_MAIN;

}
//---------------------------------------------------------------------------
void nn::yb::main::CConfig::taskMain() {

	::KEY_WORK * key = &this->master->keyWork;
	nn::yb::main::CONFIG_DATA * data = &this->saveData->configData;

	if(key_commonCursorMoveUpDown( key, nn::yb::e::config::MAIN_ITEM_MAX - 1, &this->cursorYMain)) {
		sound_nnsPlaySe( SE_U);
	}
	else if(key->trigger & nn::common::e::PAD_WIN_KEY_LEFT ||
		key->hold & nn::common::e::PAD_WIN_KEY_LEFT
	) {
		switch(this->cursorYMain) {
			case nn::yb::e::config::MAIN_ITEM_BGM_VOL:
				sound_nnsPlaySe( SE_DEDO);
				if(data->bgmVol > 0) {
					data->bgmVol--;
				}
				sound_nnsSetVolumeBgm( data->bgmVol);
				break;
			case nn::yb::e::config::MAIN_ITEM_SE_VOL:
				sound_nnsPlaySe( SE_DEDO);
				if(data->seVol > 0) {
					data->seVol--;
				}
				sound_nnsSetVolumeSe( data->seVol);
				break;
			case nn::yb::e::config::MAIN_ITEM_DISP_SIZE:
				sound_nnsPlaySe( SE_DEDO);
				if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
					data->dispSize = 1 - data->dispSize;
				}
				break;
			case nn::yb::e::config::MAIN_ITEM_DISP_MASK:
				sound_nnsPlaySe( SE_DEDO);
				if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
					data->dispMask = 1 - data->dispMask;
				}
				break;
			case nn::yb::e::config::MAIN_ITEM_SAVE_CANCEL:
				sound_nnsPlaySe( SE_U);
				if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
					this->cursorXMain = 1 - this->cursorXMain;
				}
				break;
		}
	}
	else if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT ||
		key->hold & nn::common::e::PAD_WIN_KEY_RIGHT
	) {
		switch(this->cursorYMain) {
			case nn::yb::e::config::MAIN_ITEM_BGM_VOL:
				sound_nnsPlaySe( SE_DEDO);
				if(data->bgmVol < 127) {
					data->bgmVol++;
				}
				sound_nnsSetVolumeBgm( data->bgmVol);
				break;
			case nn::yb::e::config::MAIN_ITEM_SE_VOL:
				sound_nnsPlaySe( SE_DEDO);
				if(data->seVol < 127) {
					data->seVol++;
				}
				sound_nnsSetVolumeSe( data->seVol);
				break;
			case nn::yb::e::config::MAIN_ITEM_DISP_SIZE:
				sound_nnsPlaySe( SE_DEDO);
				if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
					data->dispSize = 1 - data->dispSize;
				}
				break;
			case nn::yb::e::config::MAIN_ITEM_DISP_MASK:
				sound_nnsPlaySe( SE_DEDO);
				if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
					data->dispMask = 1 - data->dispMask;
				}
				break;
			case nn::yb::e::config::MAIN_ITEM_SAVE_CANCEL:
				sound_nnsPlaySe( SE_U);
				if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
					this->cursorXMain = 1 - this->cursorXMain;
				}
				break;
		}
	}
	else if(key->trigger & nn::common::e::PAD_WIN_BUTTON_A) {
		if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_PAD) {
			sound_nnsPlaySe( SE_DEDON);
			this->state = nn::yb::e::config::STATE_PAD;
		}
		else if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_SAVE_CANCEL) {
			if(this->cursorXMain == 0) {
				sound_nnsPlaySe( SE_DEDON);
				this->ctrl->saveMain();
			}
			else {
				sound_nnsPlaySe( SE_DEDHIN);
				memcpy( &this->saveData->configData, &this->oldConfig, sizeof( this->saveData->configData));
			}
			// �ݒ�̔��f
			if(data->dispSize != this->oldConfig.dispSize) {
				if(data->dispSize == nn::yb::e::config::SET_DISP_SIZE_ITEM_WINDOW) {
					this->ctrl->releaseImage( 0);
					nn::yb::main::CEffectCtrl::uninitialize();
					int result = ChangeWindowMode( TRUE);
					if(result >= 0) {
						this->ctrl->changeWindowSizeAfter();
					}
					this->ctrl->loadImage( 0);
					
					this->ctrl->initializeEffectCtrl();
				}
				else {
					this->ctrl->releaseImage( 0);
					nn::yb::main::CEffectCtrl::uninitialize();
					int result = ChangeWindowMode( FALSE);
					if(result >= 0) {
						this->ctrl->changeWindowSizeAfter();
					}
					this->ctrl->loadImage( 0);

					this->ctrl->initializeEffectCtrl();			
				}
			}

			if(data->dispMask != this->oldConfig.dispMask) {
				if(data->dispMask == nn::yb::e::config::SET_DISP_MASK_ITEM_ENABLE) {
					disp_commonStartMask();
				}
				else {
					disp_commonEndMask();
				}
			}

			if(this->endCb != NULL) {
				this->endCb( this->endCbArg);
			}
			this->taskDelete();
		}
	}


}
//---------------------------------------------------------------------------
void nn::yb::main::CConfig::taskPad() {
	
	::KEY_WORK * key = &this->master->keyWork;
	
	int change_trigger = key->originalPadState & (key->originalPadState ^ this->oldOriginalPadState);
	this->oldOriginalPadState = key->originalPadState;

	if(::key_commonCursorMoveUpDown( key, this->itemN, &this->cursorY)) {
		sound_nnsPlaySe( SE_U);
	}
	else if(key->pulse & nn::common::e::PAD_WIN_KEY_LEFT) {
		if(this->cursorY == this->itemN) {
			sound_nnsPlaySe( SE_U);
			this->cursorX = 1 - this->cursorX;
		}
	}
	else if(key->pulse & nn::common::e::PAD_WIN_KEY_RIGHT) {
		if(this->cursorY == this->itemN) {
			sound_nnsPlaySe( SE_U);
			this->cursorX = 1 - this->cursorX;
		}
	}
	else if(change_trigger != 0 && this->cursorY < this->itemN) {
		int button_no = this->cursorY + 4 + 1;
		int bit_first = this->bsf( (unsigned int)change_trigger);
		if(4 <= bit_first && bit_first < 32) {
			int target_index = bit_first;
			if(button_no != this->configPad[target_index]) {
				for( int i = 4; i < 32; i++) {

					if(this->configPad[i] == button_no) {
						sound_nnsPlaySe( SE_DEDON);
						this->configPad[i] = this->configPad[target_index];
					}

				}
				this->configPad[target_index] = button_no;
			}
			this->renewButtonNoTbl();
			this->cursorY++;
		}
	}
	else if(key->trigger & 0xfffffff0) {
		if(this->cursorY == this->itemN) {
			if(this->cursorX == 0) {
				sound_nnsPlaySe( SE_DEDON);
				memcpy( key->configPad, this->configPad, sizeof( key->configPad));
				memcpy( this->saveData->configData.pad, this->configPad, sizeof( this->saveData->configData.pad));
				this->ctrl->saveMain();
				this->state = nn::yb::e::config::STATE_MAIN;
			}
			else {
				sound_nnsPlaySe( SE_DEDHIN);
				this->state = nn::yb::e::config::STATE_MAIN;
			}
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CConfig::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CConfig::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}
	
	this->disp();

	::KEY_WORK * key = &this->master->keyWork;
	
	switch(this->state) {
		case nn::yb::e::config::STATE_NONE:
			break;
		case nn::yb::e::config::STATE_MAIN:
			this->taskMain();
			break;
		case nn::yb::e::config::STATE_PAD:
			this->taskPad();
			break;
	}

}
//---------------------------------------------------------------------------
char * gTextTblPadConfig[] = {
	"����",
	"�L�����Z��",
	"�}�b�v",
	"�_�b�V��",
	"�k",
	"�q",
	"�|�[�Y",
	"SELECT",
};

char * gTextPadConfigGuideA[] = {
	"����{�^��",
	"����{�^���ɐݒ肵�����Q�[���p�b�h��",
	"�{�^���������܂��B",
};
char * gTextPadConfigGuideB[] = {
	"�L�����Z���{�^��",
	"�L�����Z���{�^���ɐݒ肵�����Q�[���p",
	"�b�h�̃{�^���������܂��B",
};
char * gTextPadConfigGuideY[] = {
	"�_�b�V���{�^��",
	"�_�b�V���{�^���ɐݒ肵�����Q�[���p�b",
	"�h�̃{�^���������܂��B",
};
char * gTextPadConfigGuideX[] = {
	"�}�b�v�{�^��",
	"�}�b�v�{�^���ɐݒ肵�����Q�[���p�b�h",
	"�̃{�^���������܂��B",
};
char * gTextPadConfigGuideL[] = {
	"L�{�^��",
	"L�{�^���ɐݒ肵�����Q�[���p�b�h�̃{�^",
	"���������܂��B",
};
char * gTextPadConfigGuideR[] = {
	"R�{�^��",
	"R�{�^���ɐݒ肵�����Q�[���p�b�h�̃{�^",
	"���������܂��B",
};
char * gTextPadConfigGuideSTART[] = {
	"�|�[�Y�{�^��",
	"�|�[�Y�{�^���ɐݒ肵�����Q�[���p�b�h",
	"�̃{�^���������܂��B",
};
char * gTextPadConfigGuideSave[] = {
	"�ۑ�",
	"�ύX�������e��K�p�E�ۑ����āA�R���t",
	"�B�O�ɖ߂�܂���B",
	"�ۑ����Ȃ��ƁA�܊p�ݒ肵�����e������",
	"�߂����Ⴂ�܂���B",
};
char * gTextPadConfigGuideCancel[] = {
	"�L�����Z��",
	"�ύX�������e���L�����Z�����āA�R���t",
	"�B�O�ɖ߂�܂���B",
	"�ݒ���Ԉ���Ă��܂����̂Ȃ�A������",
	"�ł���B",
};

char * gTextTblSaveCancel[] = {
	"�ۑ�", "�L�����Z��",
};
char * gTextTblMainItemName[nn::yb::e::config::MAIN_ITEM_MAX] = {
	"BGM����",
	"SE����",
	"�\���T�C�Y",
	"�}�X�N�`��",
	"�Q�[���p�b�h�ݒ�",
	"",
};

char * gTextTBlMainDispSizeImteName[nn::yb::e::config::SET_DISP_SIZE_ITEM_MAX] = {
	"�E�B���h�E",
	"�t��",
};

char * gTextTBlMainDispMaskImteName[nn::yb::e::config::SET_DISP_MASK_ITEM_MAX] = {
	"����",
	"�Ȃ�",
};
char * gTextGuideBgm[] = {
	"BGM����",
	"���E�L�[��BGM�̉��ʂ𒲐߂��܂���B",
};
char * gTextGuideSe[] = {
	"SE����",
	"���E�L�[�Ō��ʉ��̉��ʂ𒲐߂��܂���B",
};
char * gTextGuideWindowSize[] = {
	"�E�B���h�E�T�C�Y",
	"�Q�[����ʂ��E�B���h�E��640x480�ŕ\��",
	"���܂���B",
};
char * gTextGuideFullSize[] = {
	"�t���T�C�Y",
	"�Q�[����ʂ���ʈ�t�ɕ\�����܂���B",
	"�𑜓x�̏c���䂪4:3�łȂ���Ԃł́A�\",
	"���̏c���䂪����Ă��܂��܂��B�c�O�B",
};
char * gTextGuideWIndowMaskEnable[] = {
	"�}�X�N�`�悠��",
	"���̃Q�[���ł́A�Q�[���̈ꕔ�Ń}�X�N",
	"�`����g�p���Ă��܂��B",
	"�R���s���[�^�ɂ���Ă͏����������鎖",
	"������܂��̂ł��̏ꍇ�A��������Ȃ�",
	"�ɂ��Ă݂Ă��������ˁB",
};
char * gTextGuideWIndowMaskDisable[] = {
	"�}�X�N�`��Ȃ�",
	"�S�̓I�A�������̓J�b�v�C�����ɏ�����",
	"�����铙�̏ꍇ�A�Ƃ肠����������B��",
	"�P���邩������܂����B",
};
char * gTextGuideKeyConfig[] = {
	"�Q�[���p�b�h�ݒ�",
	"����{�^���ŁA�Q�[���p�b�h�̃{�^����",
	"�蓖�Đݒ�ɍs���܂���B",
	"����{�^���̓L�[�{�[�h�ł́A�w�L�[�A",
	"��������Enter�L�[�ł���B",
};
char * gTextGuideSave[] = {
	"�ۑ�",
	"�ύX�������e��K�p�E�ۑ����āA�Q�[��",
	"�ɖ߂�܂���B",
	"�ۑ����Ȃ��ƁA�܊p�ݒ肵�����e������",
	"�߂����Ⴂ�܂���B",
};
char * gTextGuideCancel[] = {
	"�L�����Z��",
	"�ύX�������e���L�����Z�����āA�Q�[��",
	"�ɖ߂�܂���B",
	"�ݒ���Ԉ���Ă��܂����̂Ȃ�A������",
	"�ł���B",
};
void nn::yb::main::CConfig::disp() {

	switch(this->state) {
		case nn::yb::e::config::STATE_NONE:
			break;
		case nn::yb::e::config::STATE_MAIN:
			{
				disp_commonObjSimple( LAYOUT_ID_MENU_CONFIG_WINDOW);

				int cursor_x_main[nn::yb::e::config::MAIN_ITEM_MAX] = {
					0,
					0,
					this->saveData->configData.dispSize,
					this->saveData->configData.dispMask,
					0
				};
				LAYOUT * layout_text = disp_commonGetLayoutMain3d( LAYOUT_ID_MENU_CONFIG_TEXT_TOP_LEFT);
				for( int i = 0; i < nn::yb::e::config::MAIN_ITEM_MAX; i++) {

					GXRgb color = RGB_BLACK;
					if(i == this->cursorYMain) {
						color = RGB_RED;
					}
					if(i != nn::yb::e::config::MAIN_ITEM_SAVE_CANCEL) {
						fontPutsType( layout_text->x, layout_text->y + layout_text->h * i,
							nn::yb::e::global::FONT_TYPE_SIZE_18, color, gTextTblMainItemName[i]
						);
					}

					switch(i) {
						case nn::yb::e::config::MAIN_ITEM_BGM_VOL:
							fontPrintfType( layout_text->x + 150, layout_text->y + layout_text->h * i,
								nn::yb::e::global::FONT_TYPE_SIZE_18, color,
								"%2d", this->saveData->configData.bgmVol
							);
							break;
						case nn::yb::e::config::MAIN_ITEM_SE_VOL:
							fontPrintfType( layout_text->x + 150, layout_text->y + layout_text->h * i,
								nn::yb::e::global::FONT_TYPE_SIZE_18, color,
								"%2d", this->saveData->configData.seVol
							);
							break;
						case nn::yb::e::config::MAIN_ITEM_DISP_SIZE:
						case nn::yb::e::config::MAIN_ITEM_DISP_MASK:
							{
								char ** text_tbl = NULL;
								if(i == nn::yb::e::config::MAIN_ITEM_DISP_SIZE) {
									text_tbl = gTextTBlMainDispSizeImteName;
								}
								if(i == nn::yb::e::config::MAIN_ITEM_DISP_MASK) {
									text_tbl = gTextTBlMainDispMaskImteName;
								}
								for( int j = 0; j < 2; j++) {

									GXRgb color_window_size = RGB_BLACK;
									if(j == cursor_x_main[i]) {
										if(i == this->cursorYMain) {
											color_window_size = RGB_RED;
										}
										else {
											color_window_size = GX_RGB( 28, 12, 0);
										}
									}

									fontPutsType( layout_text->x + 150 + 100 * j, layout_text->y + layout_text->h * i,
										nn::yb::e::global::FONT_TYPE_SIZE_18, color_window_size,
										text_tbl[j]
									);

								}
							}
							break;
						case nn::yb::e::config::MAIN_ITEM_SAVE_CANCEL:
							{
								for( int j = 0; j < 2; j++) {

									GXRgb color_save_cancel = RGB_BLACK;
									if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_SAVE_CANCEL &&
										j == this->cursorXMain
									) {
										color_save_cancel = RGB_RED;
									}
									fontPutsType( layout_text->x + 100 * j, layout_text->y + layout_text->h * i,
										nn::yb::e::global::FONT_TYPE_SIZE_18, color_save_cancel,
										gTextTblSaveCancel[j]
									);
									
								}
							}
							break;
					}

				}

				// ���b�Z�[�W�{�b�N�X
				LAYOUT * layout_msg_box = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX);
				nn::yb::common::yb_commonDispMsgBox( layout_msg_box->x, layout_msg_box->y,
					layout_msg_box->z, layout_msg_box->w, layout_msg_box->h,
					OBJ_ID_TITLE_MSG_BOX_0, RGB_WHITE, 24
				);
				disp_commonObjSimpleColor( LAYOUT_ID_TITLE_MSG_BOX_BALLOON, RGB_WHITE, 24);	// �����o��

				LAYOUT * layout_msg_box_text = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX_TEXT);

				char ** disp_text_array;
				int disp_text_line_n;
				if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_BGM_VOL) {
					disp_text_array = gTextGuideBgm;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideBgm);
				}
				else if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_SE_VOL) {
					disp_text_array = gTextGuideSe;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideSe);
				}
				else if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_DISP_SIZE) {
					if(cursor_x_main[this->cursorYMain] == 0) {
						disp_text_array = gTextGuideWindowSize;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideWindowSize);
					}
					else {
						disp_text_array = gTextGuideFullSize;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideFullSize);
					}
				}
				else if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_DISP_MASK) {
					if(cursor_x_main[this->cursorYMain] == 0) {
						disp_text_array = gTextGuideWIndowMaskEnable;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideWIndowMaskEnable);
					}
					else {
						disp_text_array = gTextGuideWIndowMaskDisable;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideWIndowMaskDisable);
					}
				}
				else if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_PAD) {
					disp_text_array = gTextGuideKeyConfig;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideKeyConfig);
				}
				else if(this->cursorYMain == nn::yb::e::config::MAIN_ITEM_SAVE_CANCEL) {
					if(this->cursorXMain == 0) {
						disp_text_array = gTextGuideSave;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideSave);
					}
					else {
						disp_text_array = gTextGuideCancel;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextGuideCancel);
					}
				}

				for( int i = 0; i < disp_text_line_n; i++) {

					fontPutsType( layout_msg_box_text->x, layout_msg_box_text->y + layout_msg_box_text->h * i,
						nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, disp_text_array[i]
					);

				}
			}
			break;
		case nn::yb::e::config::STATE_PAD:
			{
				disp_commonObjSimple( LAYOUT_ID_MENU_CONFIG_WINDOW);

				LAYOUT * layout_text = disp_commonGetLayoutMain3d( LAYOUT_ID_MENU_CONFIG_TEXT_TOP_LEFT);
				int i;
				for( i = 0; i < this->itemN; i++) {

					// 0:A�{�^��
					// 1:B�{�^��
					// 2:X�{�^��
					// 3:Y�{�^��
					// 4:L�{�^��
					// 5:R�{�^��
					// 6:Start
					// 7:Select
					GXRgb color = RGB_BLACK;
					if(i == this->cursorY) {
						color = RGB_RED;
					}
					fontPutsType( layout_text->x, layout_text->y + 20 * i, nn::yb::e::global::FONT_TYPE_SIZE_18, color, 
						gTextTblPadConfig[i]
					);
					int button_no = this->buttonNoTbl[i + 4] - 4 + 1;
					fontPrintfType( layout_text->x + 250, layout_text->y + 20 * i, nn::yb::e::global::FONT_TYPE_SIZE_18, color,
						":%2d", button_no
					);

				}

				int y = i;
				for( i = 0; i < 2; i ++) {

					GXRgb color = RGB_BLACK;
					if(this->cursorY == this->itemN && i == this->cursorX) {
						color = RGB_RED;
					}
					fontPutsType( layout_text->x + 100 * i, layout_text->y + 20 * y,
						nn::yb::e::global::FONT_TYPE_SIZE_18, color,
						gTextTblSaveCancel[i]
					);

				}

				// ���b�Z�[�W�{�b�N�X
				LAYOUT * layout_msg_box = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX);
				nn::yb::common::yb_commonDispMsgBox( layout_msg_box->x, layout_msg_box->y,
					layout_msg_box->z, layout_msg_box->w, layout_msg_box->h,
					OBJ_ID_TITLE_MSG_BOX_0, RGB_WHITE, 24
				);
				disp_commonObjSimpleColor( LAYOUT_ID_TITLE_MSG_BOX_BALLOON, RGB_WHITE, 24);	// �����o��

				LAYOUT * layout_msg_box_text = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX_TEXT);

				char ** disp_text_array;
				int disp_text_line_n;
				if(this->cursorY == nn::yb::e::config::PAD_CONFIG_A) {
					disp_text_array = gTextPadConfigGuideA;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideA);
				}
				else if(this->cursorY == nn::yb::e::config::PAD_CONFIG_B) {
					disp_text_array = gTextPadConfigGuideB;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideB);
				}
				else if(this->cursorY == nn::yb::e::config::PAD_CONFIG_X) {
					disp_text_array = gTextPadConfigGuideX;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideX);
				}
				else if(this->cursorY == nn::yb::e::config::PAD_CONFIG_Y) {
					disp_text_array = gTextPadConfigGuideY;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideY);
				}
				else if(this->cursorY == nn::yb::e::config::PAD_CONFIG_L) {
					disp_text_array = gTextPadConfigGuideL;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideL);
				}
				else if(this->cursorY == nn::yb::e::config::PAD_CONFIG_R) {
					disp_text_array = gTextPadConfigGuideR;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideR);
				}
				else if(this->cursorY == nn::yb::e::config::PAD_CONFIG_START) {
					disp_text_array = gTextPadConfigGuideSTART;
					disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideSTART);
				}
				else if(this->cursorY == nn::yb::e::config::PAD_CONFIG_MAX) {
					if(this->cursorX == 0) {
						disp_text_array = gTextPadConfigGuideSave;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideSave);
					}
					else {
						disp_text_array = gTextPadConfigGuideCancel;
						disp_text_line_n = NN_COMMON_GET_ARRAY_SIZE( gTextPadConfigGuideCancel);
					}
				}

				for( i = 0; i < disp_text_line_n; i++) {

					fontPutsType( layout_msg_box_text->x, layout_msg_box_text->y + layout_msg_box_text->h * i,
						nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, disp_text_array[i]
					);

				}
			}
			break;
	}

}

//---------------------------------------------------------------------------

#include <cassert>

#ifdef _MSC_VER
#include <intrin.h>
#endif

int nn::yb::main::CConfig::bsf( unsigned int mask) {

    assert(mask);
#ifdef _MSC_VER
    // VC++�̏ꍇ
    unsigned long ret;
    _BitScanForward( &ret, mask);
    // ���������0���ƃT�[�`���s���Ӗ�����̂����c�B
    // �܂��A���������g�����͂��Ă͂Ȃ�Ȃ��Ƃ������ƂŁc�B
    return (int)ret;
#elif defined (__x86_64 || i386)
    int ret;
    __asm__("bsfl %1,%0" : "=r"(ret) : "r"(mask));
    return ret;
#else
    // gcc�̏ꍇ
    return __builtin_ctzl(mask);
#endif

}
//---------------------------------------------------------------------------
void nn::yb::main::CConfig::renewButtonNoTbl() {

	//�e�[�u���̍X�V
	for( int i = 0; i < 32; i++) {

		for( int j = 0; j < 32; j++) {

			if(this->configPad[j] == i + 1) {
				this->buttonNoTbl[i] = j;
			}

		}

	}

}
//---------------------------------------------------------------------------

