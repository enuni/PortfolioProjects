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
			unsigned int size	: 31;	// セグメントのサイズ
			unsigned int isSeq	: 1;	// シーケンスデータか
		};

		union
		{
			int offset;
			void *pRef;
		};

	} SInfo;

	// ヘッダ
	typedef struct
	{
		// 識別語
		char string[ 4 ];	// "Teco"

		// バージョン
		union
		{
			int version;

			struct
			{
				short majorVersion;
				short minorVersion;
			};
		};

		// 実行時に必要なメモリサイズ
		short localVarSize;	// ローカル変数用
		short argSize;		// 引数用

		int segNum;
		union
		{
			int segOffset;
			SInfo *paSegInfo;
		};

	} STecoHeader;

	// コマンド
	typedef struct
	{
		unsigned short commandId;	// コマンドID

		union
		{
			struct	// 通常のコマンドで使用
			{
				char argNum;	// 引数の数
				char varNum;	// 引数中の変数の数
			};

			short size;	// 計算式で使用  計算式のサイズ
		};

	} SCommand;

	enum
	{
		LOCAL_VARIABLE_FLAG = 0x80000000,

		// 内部コマンドフラグ
		INTERNAL_COMMAND_FLAG = 0x8000,

		// 内部コマンドID
		INTERNAL_COMMAND_END = 0,				// end 命令  ←未使用
		INTERNAL_COMMAND_JUMP,					// jump 命令
		INTERNAL_COMMAND_JUMP_IF_TRUE,			// jump if true 命令
		INTERNAL_COMMAND_JUMP_IF_FALSE,			// jump if false 命令
		INTERNAL_COMMAND_EXPRESSION = 0x3fff,	// 計算式
	};


#pragma warning( disable : 4480 )	// 非標準の拡張機能が使用されています: enum '' の基になる型を指定しています

	// 演算子
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
		EXP_OPE_UNARY_MINUS,	// 単項-
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

		// float 版
		EXP_OPE_COMP_L_F,		// <
		EXP_OPE_COMP_G_F,		// >
		EXP_OPE_COMP_LE_F,		// <=
		EXP_OPE_COMP_GE_F,		// >=
		EXP_OPE_PLUS_F,			// +
		EXP_OPE_MINUS_F,		// -
		EXP_OPE_MUL_F,			// *
		EXP_OPE_DIV_F,			// /
		EXP_OPE_UNARY_MINUS_F,	// 単項-
		EXP_OPE_SUBST_F,			// =
		EXP_OPE_SUBST_PLUS_F,		// +=
		EXP_OPE_SUBST_MINUS_F,		// -=
		EXP_OPE_SUBST_MUL_F,		// *=
		EXP_OPE_SUBST_DIV_F,		// /=

	} eExpOpecode;

	// オペランドの種類
	typedef enum : char
	{
		EXP_OPERANDTYPE_NONE = 0,
		EXP_OPERANDTYPE_IMMEDIATE,
		EXP_OPERANDTYPE_VARIABLE,
		EXP_OPERANDTYPE_OPE,

	} eExpOperandType;

	// 計算式
	typedef struct
	{
		eExpOpecode ope;

		eExpOperandType operandType[ 3 ];
	} sExp;

	// 内部コマンド 「jump if true 命令」と「jump if false 命令」の引数型
	typedef struct
	{
		int offset;	// ジャンプ先のオフセット

		int condition;	// 真偽値
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

	// オフセット→アドレス 変換
	void *OffsetToAddress( int offset )
	{
		return (void *)( (BYTE *)m_pSTecoHeader + offset );
	}

	// アドレス解決
	void SolveAddress( void );

	virtual bool Set( void *pData )
	{
		m_pSTecoHeader = (STecoHeader *)pData;

		if( CheckHeader() == false )
		{
			m_pSTecoHeader = NULL;
			return false;
		}

		// アドレス解決
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
	// 二分探索
	SInfo *BinarySearch( SInfo *paSInfo, int maxIndex, int targetId );
};

}	// namespace TecoLib
}	// namespace common
}	// namespace nn


#endif
