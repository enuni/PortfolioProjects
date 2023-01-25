// ending ��u��
// Ending ��u��
// yb ��u��
// main ��u��

#include "nn_common.h"

#include "font.h"
#include "disp_common.h"
#include "direct_common.h"

#include "enum.h"
#include "yb_common.h"

#include "sound_data.sadl"
#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"

#include "ending.h"
//---------------------------------------------------------------------------
char * gTextTblStaff[] = {
	"�������{�E��DX All Clear",
	"Congratulations!",
	"",
	"",
	"",
	"",
	"",
	"�������{�E��DX Staff",
	"",
	"�f�B���N�V����",
	"�x�[�X�v���O�����@�@���ʂɂƒ��� �K�V",
	"",
	"�v���O����",
	"�G�t�F�N�g�f�U�C���@nullpo",
	"",
	"���C���O���t�B�b�N",
	"�h�b�g",
	"�p�b�P�[�W�f�U�C���@�Ԃ��̔L",
	"",
	"�O���t�B�b�N�@�@�@�@��X����",
	"�@�@�@�@�@�@�@�@�@�@����",
	"�@�@�@�@�@�@�@�@�@�@�^���N",
	"",
	"�T�E���h�@�@�@�@�@�@KEDDY(clear sky recordings)",
	"�@�@�@�@�@�@�@�@�@�@momo(clear sky recordings)",
	"",
	"���y�f��",
	"�u�ǓƂ̃O�����vO.S.T.",
	"�@�X�N���[���g�[���Y�l",
	"",
	"- Pledge of Scarlet -(�S�������ׂ̈̃Z�v�e�b�g)",
	"�@�G�������h�N���Z���g�@�������l",
	"",
	"�_���Y�� �`smash battle.(off ver.)(�����Y�z��)",
	"�@mi_ke:l�l",
	"",
	"�����B�̃f�B�X�R(�����B�̕����� �`Magus(�������z))",
	"�@���ʒ����t�@�[�W�@mocchie�l",
	"",
	"���CGirl�t�����h�[��(UN�I�[�G���͔ޏ��Ȃ̂�?)",
	"�@�Q�����@���_���@Livi�l",
	"",
	"���̗��s�A�����W(���̗��s�@�`Be�@of�@good�@cheer!)",
	"�@RaggedFriend�@�T�C�����g�}���l",
	"",
	"�@�N���V�b�N���ȃT�E���h���C�u�����[�l",
	"",
	"���ʉ��f��",
	"�@�U�E�}�b�`���C�J�@�Y�l",
	"",
	"�@AquesTalk",
	"",
	"",
	"",
	"",
	"",
	"�@�@�@�@�@�@�@�@�@����@n2NSoft�@�@�@�@�@�@�@�@�@",
	"�@�@�@�@�@�@�@�@�@����@agrippa�@�@�@�@�@�@�@�@�@",
};
//---------------------------------------------------------------------------
nn::yb::main::CEnding::CEnding(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CEnding Create\n");
	this->state = nn::yb::e::ending::STATE_NONE;
	this->oldState = nn::yb::e::ending::STATE_NONE;
	this->waitCnt = 0;

	this->frameCnt = 0;
	this->lineCnt = 0;

	this->endCb = NULL;
	this->endCbArg = NULL;

	this->allTotalScore = 0;
	this->yukkuriAlpha = 0;

}
//---------------------------------------------------------------------------
nn::yb::main::CEnding::~CEnding() {

	::nn_commonLog( 1, "CEnding Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::main::CEnding::start() {

	// �g�[�^��150�ňȉ��Ŗ߂�
	// �g�[�^��151�`250�ňȉ��Ŗ߂肩����
	// �g�[�^��251�`�ȏ�ŕω��Ȃ�
	int temp = this->allTotalScore - 150;
	if(temp < 0){
		temp = 0;
	}
	if(temp >= 100){
		temp = 100;
	}
	this->yukkuriAlpha = (int)(((double)temp * 31.0 / 100.0) + 0.999);

	direct_commonCreateTaskBlackFadeInAllSimple( NULL, nn::yb::main::CEnding::nextCb, this);
	this->state = nn::yb::e::ending::STATE_START;

}
//---------------------------------------------------------------------------
void nn::yb::main::CEnding::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CEnding::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	switch(this->state) {
		case nn::yb::e::ending::STATE_NONE:
			break;
		case nn::yb::e::ending::STATE_START:
			break;
		case nn::yb::e::ending::STATE_STAFF_ROLL:
			if(this->frameCnt >= 60) {
				if(this->lineCnt < NN_COMMON_GET_ARRAY_SIZE( gTextTblStaff)) {
					nn::yb::main::CStaffRoll* sr = new nn::yb::main::CStaffRoll( this);
					sr->text = gTextTblStaff[this->lineCnt];
					sr->y = 510;
					if(this->lineCnt == NN_COMMON_GET_ARRAY_SIZE( gTextTblStaff) - 1) {
						sr->endCb = nn::yb::main::CEnding::nextCb;
						sr->endCbArg = this;
					}
					this->lineCnt++;
					this->frameCnt = 0;
				}
			}
			else {
				this->frameCnt++;
			}
			break;
		case nn::yb::e::ending::STATE_FIN:
			{
				bool flag = false;
				
				if((key->trigger & nn::common::e::PAD_WIN_BUTTON_A) ||
					(key->trigger & nn::common::e::PAD_WIN_BUTTON_START)
				) {
					flag = true;
				}
				if(this->frameCnt >= 60 * 60) {
					flag = true;
				}
				else {
					this->frameCnt++;
				}
				if(flag) {
					this->frameCnt = 0;
					direct_commonCreateTaskBlackFadeOutAllSimple( NULL, nn::yb::main::CEnding::nextCb, this);
					this->state = nn::yb::e::ending::STATE_END;
				}
			}
			break;
		case nn::yb::e::ending::STATE_END:
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
char * gTextEnding[] = {
	"���肪�Ƃ��B",
	"�ǂ����Ė߂�Ȃ��̂�I�H�@�����B",
};
//---------------------------------------------------------------------------
void nn::yb::main::CEnding::disp() {

	switch(this->state) {
		case nn::yb::e::ending::STATE_NONE:
		case nn::yb::e::ending::STATE_START:
		case nn::yb::e::ending::STATE_STAFF_ROLL:
			{
#if 0
				// �������Ɨ얲�̃��[�t�B���O�� �킸��킵�����Ȃ̂ŕۗ�
				int correct_alpha = (this->frameCnt % 60) * 15 / 59 - 8;
				int disp_yukkuri_alpha = this->yukkuriAlpha - correct_alpha;
				if(disp_yukkuri_alpha < 0) {
					disp_yukkuri_alpha = 0;
				}
				if(disp_yukkuri_alpha > 31) {
					disp_yukkuri_alpha = 31;
				}
				disp_commonObjSimpleColor( LAYOUT_ID_TITLE_REIMU, RGB_WHITE, 31 - disp_yukkuri_alpha);
				disp_commonObjSimpleColor( LAYOUT_ID_TITLE_REIMU, RGB_WHITE, 31);
#endif
				/*
				if(this->yukkuriAlpha == 0) {
					// �얲
					disp_commonObjSimple( LAYOUT_ID_TITLE_REIMU);
				}
				else {
					// �������
					disp_commonObjSimple( LAYOUT_ID_TITLE_YUKKURI_ENDING_1);
				}
				*/
			}
			break;
		case nn::yb::e::ending::STATE_FIN:
		case nn::yb::e::ending::STATE_END:
			{
				/*
				if(this->yukkuriAlpha == 0) {
					// �얲
					disp_commonObjSimple( LAYOUT_ID_TITLE_REIMU);

					// ���b�Z�[�W�{�b�N�X
					LAYOUT * layout_msg_box = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX_REIMU);
					nn::yb::common::yb_commonDispMsgBox( layout_msg_box->x, layout_msg_box->y,
						layout_msg_box->z, layout_msg_box->w, layout_msg_box->h,
						OBJ_ID_TITLE_MSG_BOX_0, RGB_WHITE, 24
					);
					disp_commonObjSimpleColor( LAYOUT_ID_TITLE_MSG_BOX_BALLOON_REIMU, RGB_WHITE, 24);	// �����o��
					LAYOUT * layout_msg_box_text = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX_TEXT_REIMU);
					fontPutsType( layout_msg_box_text->x, layout_msg_box_text->y, 
						nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, gTextEnding[0]
					);
				}
				else {
					// �������
					disp_commonObjSimple( LAYOUT_ID_TITLE_YUKKURI_ENDING_2);

					// ���b�Z�[�W�{�b�N�X
					LAYOUT * layout_msg_box = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX_YUKKURI);
					nn::yb::common::yb_commonDispMsgBox( layout_msg_box->x, layout_msg_box->y,
						layout_msg_box->z, layout_msg_box->w, layout_msg_box->h,
						OBJ_ID_TITLE_MSG_BOX_0, RGB_WHITE, 24
					);
					disp_commonObjSimpleColor( LAYOUT_ID_TITLE_MSG_BOX_BALLOON_YUKKURI, RGB_WHITE, 24);	// �����o��
					LAYOUT * layout_msg_box_text = disp_commonGetLayoutMain3d( LAYOUT_ID_TITLE_MSG_BOX_TEXT_YUKKURI);
					fontPutsType( layout_msg_box_text->x, layout_msg_box_text->y, 
						nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_BLACK, gTextEnding[1]
					);
				}
				*/
				fontPutsType( 320 - 7, 240 - 5, 
					nn::yb::e::global::FONT_TYPE_SIZE_10, RGB_WHITE, "END"
				);
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CEnding::nextCb( void * endingArg) {

	nn::yb::main::CEnding * ending = (nn::yb::main::CEnding *)endingArg;

	ending->next();

}
//---------------------------------------------------------------------------
void nn::yb::main::CEnding::next() {

	if(this->state == nn::yb::e::ending::STATE_START) {
		nn::yb::common::yb_commonPlaySeq( BGM_ENDING);
		this->state = nn::yb::e::ending::STATE_STAFF_ROLL;
	}
	else if(this->state == nn::yb::e::ending::STATE_STAFF_ROLL) {
		this->frameCnt = 0;
		this->state = nn::yb::e::ending::STATE_FIN;
	}
	else if(this->state == nn::yb::e::ending::STATE_END) {
		if(this->endCb != NULL) {
			this->endCb( this->endCbArg);
		}
		this->taskDelete();
	}

}
//---------------------------------------------------------------------------
nn::yb::main::CStaffRoll::CStaffRoll(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CStaffRoll Create\n");
	this->state = nn::yb::e::staff_roll::STATE_NONE;
	this->oldState = nn::yb::e::staff_roll::STATE_NONE;
	this->waitCnt = 0;

	this->text = NULL;
	this->y = 0;
	this->endCb = NULL;
	this->endCbArg = NULL;
	this->cnt = 0;

}
//---------------------------------------------------------------------------
nn::yb::main::CStaffRoll::~CStaffRoll() {

	::nn_commonLog( 1, "CStaffRoll Delete\n");

}
//---------------------------------------------------------------------------
void nn::yb::main::CStaffRoll::start() {

}
//---------------------------------------------------------------------------
void nn::yb::main::CStaffRoll::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CStaffRoll::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	switch(this->state) {
		case nn::yb::e::staff_roll::STATE_NONE:
			{
				if(this->y <= -30) {
					if(this->endCb != NULL) {
						this->endCb( this->endCbArg);
					}
					this->taskDelete();
				}
				else {
					if(this->cnt >= 1) {
						this->cnt = 0;
						this->y--;
					}
					else {
						this->cnt++;
					}
				}
			}
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::main::CStaffRoll::disp() {

	switch(this->state) {
		case nn::yb::e::staff_roll::STATE_NONE:
			{
				fontPutsType( 100, this->y, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, this->text);
			}
			break;
	}

}
//---------------------------------------------------------------------------

