#ifndef __CTecoData_H__
#define __CTecoData_H__

#include "windows.h"

namespace nn
{
namespace common
{
namespace teco
{

//#define TECOLIB_DEBUG

class CTecoData
{
public:

	typedef struct
	{
		int id;

		struct
		{
			unsigned int size	: 31;	// �Z�O�����g�̃T�C�Y
			unsigned int isSeq	: 1;	// �V�[�P���X�f�[�^��
		};

		union
		{
			int offset;
			void *pRef;
		};

	} SInfo;

	// �w�b�_
	typedef struct
	{
		// ���ʌ�
		char string[ 4 ];	// "Teco"

		// �o�[�W����
		union
		{
			int version;

			struct
			{
				short majorVersion;
				short minorVersion;
			};
		};

		// ���s���ɕK�v�ȃ������T�C�Y
		short localVarSize;	// ���[�J���ϐ��p
		short argSize;		// �����p

		int segNum;
		union
		{
			int segOffset;
			SInfo *paSegInfo;
		};

	} STecoHeader;

	// �R�}���h
	typedef struct
	{
		unsigned short commandId;	// �R�}���hID

		union
		{
			struct	// �ʏ�̃R�}���h�Ŏg�p
			{
				char argNum;	// �����̐�
				char varNum;	// �������̕ϐ��̐�
			};

			short size;	// �v�Z���Ŏg�p  �v�Z���̃T�C�Y
		};

	} SCommand;

	enum
	{
		LOCAL_VARIABLE_FLAG = 0x80000000,

		// �����R�}���h�t���O
		INTERNAL_COMMAND_FLAG = 0x8000,

		// �����R�}���hID
		INTERNAL_COMMAND_END = 0,				// end ����  �����g�p
		INTERNAL_COMMAND_JUMP,					// jump ����
		INTERNAL_COMMAND_JUMP_IF_TRUE,			// jump if true ����
		INTERNAL_COMMAND_JUMP_IF_FALSE,			// jump if false ����
		INTERNAL_COMMAND_EXPRESSION = 0x3fff,	// �v�Z��
	};


#pragma warning( disable : 4480 )	// ��W���̊g���@�\���g�p����Ă��܂�: enum '' �̊�ɂȂ�^���w�肵�Ă��܂�

	// ���Z�q
	typedef enum : char
	{
		EXP_OPE_CONDITION,		// ? :
		EXP_OPE_LOGICAL_OR,		// ||
		EXP_OPE_LOGICAL_AND,	// &&
		EXP_OPE_BIT_OR,			// |
		EXP_OPE_BIT_XOR,		// ^
		EXP_OPE_BIT_AND,		// &
		EXP_OPE_COMP_EQ,		// ==
		EXP_OPE_COMP_NE,		// !=
		EXP_OPE_COMP_L,			// <
		EXP_OPE_COMP_G,			// >
		EXP_OPE_COMP_LE,		// <=
		EXP_OPE_COMP_GE,		// >=
		EXP_OPE_BIT_LSHIFT,		// <<
		EXP_OPE_BIT_RSHIFT,		// >>
		EXP_OPE_PLUS,			// +
		EXP_OPE_MINUS,			// -
		EXP_OPE_MUL,			// *
		EXP_OPE_DIV,			// /
		EXP_OPE_MOD,			// %
		EXP_OPE_CAST_INT,		// (int)
		EXP_OPE_CAST_FLOAT,		// (float)
		EXP_OPE_UNARY_MINUS,	// �P��-
		EXP_OPE_BIT_NOT,		// ~
		EXP_OPE_LOGICAL_NOT,	// !
		EXP_OPE_SUBST,				// =
		EXP_OPE_SUBST_PLUS,			// +=
		EXP_OPE_SUBST_MINUS,		// -=
		EXP_OPE_SUBST_MUL,			// *=
		EXP_OPE_SUBST_DIV,			// /=
		EXP_OPE_SUBST_MOD,			// %=
		EXP_OPE_SUBST_BIT_LSHIFT,	// <<=
		EXP_OPE_SUBST_BIT_RSHIFT,	// >>=
		EXP_OPE_SUBST_BIT_AND,		// &=
		EXP_OPE_SUBST_BIT_XOR,		// ^=
		EXP_OPE_SUBST_BIT_OR,		// |=

		// float ��
		EXP_OPE_COMP_L_F,		// <
		EXP_OPE_COMP_G_F,		// >
		EXP_OPE_COMP_LE_F,		// <=
		EXP_OPE_COMP_GE_F,		// >=
		EXP_OPE_PLUS_F,			// +
		EXP_OPE_MINUS_F,		// -
		EXP_OPE_MUL_F,			// *
		EXP_OPE_DIV_F,			// /
		EXP_OPE_UNARY_MINUS_F,	// �P��-
		EXP_OPE_SUBST_F,			// =
		EXP_OPE_SUBST_PLUS_F,		// +=
		EXP_OPE_SUBST_MINUS_F,		// -=
		EXP_OPE_SUBST_MUL_F,		// *=
		EXP_OPE_SUBST_DIV_F,		// /=

	} eExpOpecode;

	// �I�y�����h�̎��
	typedef enum : char
	{
		EXP_OPERANDTYPE_NONE = 0,
		EXP_OPERANDTYPE_IMMEDIATE,
		EXP_OPERANDTYPE_VARIABLE,
		EXP_OPERANDTYPE_OPE,

	} eExpOperandType;

	// �v�Z��
	typedef struct
	{
		eExpOpecode ope;

		eExpOperandType operandType[ 3 ];
	} sExp;

	// �����R�}���h �ujump if true ���߁v�Ɓujump if false ���߁v�̈����^
	typedef struct
	{
		int offset;	// �W�����v��̃I�t�Z�b�g

		int condition;	// �^�U�l
	} sInternalCommandArg_JumpIf;


	//////////////////////////////////////////////////

	STecoHeader	*m_pSTecoHeader;

	CTecoData()
		: m_pSTecoHeader( NULL )
	{
	}

	virtual ~CTecoData()
	{
	}


	STecoHeader *GetHeader( void )
	{
		return m_pSTecoHeader;
	}

	bool CheckHeader( void )
	{
		if( *(unsigned int *)m_pSTecoHeader == 'oceT' )
			return true;

		return false;
	}

	// �I�t�Z�b�g���A�h���X �ϊ�
	void *OffsetToAddress( int offset )
	{
		return (void *)( (BYTE *)m_pSTecoHeader + offset );
	}

	// �A�h���X����
	void SolveAddress( void );

	virtual bool Set( void *pData )
	{
		m_pSTecoHeader = (STecoHeader *)pData;

		if( CheckHeader() == false )
		{
			m_pSTecoHeader = NULL;
			return false;
		}

		// �A�h���X����
		SolveAddress();

		return true;
	}

	void *Get( void )
	{
		return (void *)m_pSTecoHeader;
	}


	int GetNeedLocalVarSize( void )
	{
		return m_pSTecoHeader->localVarSize;
	}

	int GetNeedArgSize( void )
	{
		return m_pSTecoHeader->argSize;
	}

	int GetNeedMemorySize( void )
	{
		return GetNeedLocalVarSize() + GetNeedArgSize();
	}


	int GetSegNum( void )
	{
		return m_pSTecoHeader->segNum;
	}

	SInfo *GetSeg( int id );

	SInfo *GetSegByIndex( int index )
	{
		return &m_pSTecoHeader->paSegInfo[ index ];
	}


protected:
	// �񕪒T��
	SInfo *BinarySearch( SInfo *paSInfo, int maxIndex, int targetId );
};

}	// namespace TecoLib
}	// namespace common
}	// namespace nn


#endif
