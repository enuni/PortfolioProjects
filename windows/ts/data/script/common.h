///////////////////////////////////////////////////////////////////////////////
//
//	���ʃw�b�_
//
///////////////////////////////////////////////////////////////////////////////

#define __TECO__ 0

#include "..\..\..\common\dxlib\include\script_cmd.h"
#include "..\src\image_data\obj_main_3d_data.h"
#include "..\src\image_data\layout_main_3d_data.h"
// �R���o�[�^�̐ݒ�
// 0:false 1:true
#setting "NeedVariableDeclaration"		1		// �ϐ��錾���K�v��(default:false)
#setting "DefaultVariableType"			0		// �f�t�H���g�^ 0:int 1:uint 2:float 3:str 4:label (default:int)

#setting "NoHeader"						0		// �w�b�_���o�͂��Ȃ�(default:false)
#setting "ResumableSegment"				0		// �Z�O�����g�̍ĊJ���\��(default:true)
#setting "MixableCodeData"				0		// �Z�O�����g�ɃR�[�h�ƃf�[�^�����݂���̂�����(default:false)
#setting "DeleteEmptySegment"			1		// ��̃Z�O�����g���폜���邩(default:true)
#setting "MacroLoopMax"					2000	// �}�N�����[�v�̌J��Ԃ��񐔂̏��(default:2000)

///////////////////////////////////////////////////////////////////////////////
// �}�N���֐�

///////////////////////////////////////////////////////////////////////////////

// �Z�O�����g�J�n
Seg 0

