#ifndef __TECO__
#pragma once

// �Q�[���ŗL�̕ϐ��͂��̃t�@�C�����Q�[����_script_var.h�ȂǂŎ����Ă���
// namespace���X�V����
// script_var.h��include�̃R�����g�A�E�g���O�����̃t�@�C����include����(�p�X�̓Q�[���̃f�B���N�g���ɍ����ēK�X�C��)
// 0x00:�̊J�n�A�h���X��0x400:�ɂ���(���̃t�@�C����GlobalCommonVar�T�C�Y��)
// GlobalCommonVar�����o�[��// �O���ϐ���$ans�Ȃǂ̖��O���Q�[���ŗL�ŕK�v�ȕϐ��ɕς���
// script���ʂ�header�t�@�C���ŃQ�[����_script_var.h��include����
// �Q�[�����ŋ��ʂƃQ�[���ŗL��GlobalCommonVar���̃��������m�ۂ��āA���̐擪�A�h���X��startScript�œn��
// C�����GlobalCommonVar�����o�[�ŃA�N�Z�X�A�X�N���v�g�����// �O���ϐ���$ans�Ȃǂ̖��O�ŃA�N�Z�X
// �ϐ��ǉ���GlobalCommonVar�����o�[��// �O���ϐ��ȉ��ɖ��O��ǉ�

namespace nn {
namespace dxlib {
namespace script {
	
// �i�X�N���v�g�ɂƂ��Ắj�O���ϐ�
typedef struct
{
	int ans;
	int dummy[255];

} GlobalCommonVar;
	
}	// script
}	// dxlib
}	// nn
#endif //__TECO__

//#include "..\..\..\common\dxlib\include\script_var.h"

#ifndef WIN32
///////////////////////////////////////////////////////////////////////////////
// �O���ϐ�

0x00:
	int $ans

///////////////////////////////////////////////////////////////////////////////
#endif //WIN32

