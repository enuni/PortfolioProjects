#ifndef __CTecoVM_H__
#define __CTecoVM_H__


#include "CTecoData.h"

namespace nn
{
namespace common
{
namespace teco
{

//#define TECOLIB_DEBUG

class CTecoVM
{
public:

	// コマンド呼び出しの戻り値
	typedef enum
	{
		RET_COMMAND_OK_GO_NEXT,	// 成功：次の命令の実行に移る
		RET_COMMAND_OK_STOP,	// 成功：実行を一旦停止し制御を戻す
		RET_COMMAND_ERROR_STOP,	// 失敗：停止する

	} eRetCommand;

	enum SCRIPT_END_MODE {
		SCRIPT_END_MODE_NONE,
		SCRIPT_END_MODE_EOF,
		SCRIPT_END_MODE_COMMAND_END,
		SCRIPT_END_MODE_NEXT_SEQ,
		SCRIPT_END_MODE_NEXT_SCRIPT,
		SCRIPT_END_MODE_NEXT_SCENE,
		SCRIPT_END_MODE_ERROR,

		SCRIPT_END_MODE_MAX
	};


	typedef union
	{
		int i;
		float f;

	} uValue;



	CTecoVM()
		: m_pTecoData( NULL )
		, m_pVarGlobal( NULL )
		, m_pVarLocal( NULL )
		, m_pArg( NULL )
		, m_pCurSInfo( NULL )
		, m_pCurSCommand( NULL )
		, m_pNextSCommand( NULL )
		, m_bRunning( false )
		, endMode( nn::common::teco::CTecoVM::SCRIPT_END_MODE_NONE )
	{
	}

	virtual ~CTecoVM()
	{
	}


	void SetTecoData( nn::common::teco::CTecoData *pTecoData )
	{
		m_pTecoData = pTecoData;
	}

	CTecoData *GetTecoData( void )
	{
		return m_pTecoData;
	}

/*
	void SetVariableMemory( void *pGlobal, void *pLocal )
	{
		m_pVarGlobal = reinterpret_cast<BYTE *>( pGlobal );
		m_pVarLocal = reinterpret_cast<BYTE *>( pLocal );
	}
*/
	void SetWorkMemory( void *pGlobal, void *pLocal, void *pArg )
	{
		m_pVarGlobal = reinterpret_cast<BYTE *>( pGlobal );
		m_pVarLocal = reinterpret_cast<BYTE *>( pLocal );
		m_pArg = reinterpret_cast<int *>( pArg );
	}


	int *GetVariableAddress( int address )
	{
		if( address & CTecoData::LOCAL_VARIABLE_FLAG )
		{
			// ローカル変数
			return (int *)&m_pVarLocal[ address ^ CTecoData::LOCAL_VARIABLE_FLAG ];
		}

		// グローバル変数
		return (int *)&m_pVarGlobal[ address ];
	}

	int GetVariableValue( int address )
	{
		return *GetVariableAddress( address );
	}


	bool IsRunning( void )
	{
		return m_bRunning;
	}

	bool StartSeq( int id )
	{
		return _StartSeq( GetTecoData()->GetSeg( id ) );
	}

	bool StartSeqByIndex( int index )
	{
		return _StartSeq( GetTecoData()->GetSegByIndex( index ) );
	}

	// 指定されたコマンドの次のコマンドのアドレスを得る
	CTecoData::SCommand *GetNextCommand( CTecoData::SCommand *pCommand );
	// 指定されたコマンドがシーケンスの最後のコマンドか調べる
	bool IsLastCommand( CTecoData::SCommand *pCommand );
	void GoNextCommand( void );
	virtual void Proc( void );
	eRetCommand ProcCommand( CTecoData::SCommand *pSCommand );
	eRetCommand CallInternalCommand( CTecoData::SCommand *pSCommand, int *paArg );
	eRetCommand ProcExpression( CTecoData::SCommand *pSCommand );
	int *_ProcExpression( CTecoData::sExp *pExp, uValue *pValue );

	CTecoData::SInfo *GetSInfo( void )
	{
		return m_pCurSInfo;
	}

	CTecoData::SCommand *GetSCommand( void )
	{
		return m_pCurSCommand;
	}

	virtual eRetCommand CallCommand( CTecoData::SCommand *pSCommand, int *paArg ) = 0;


protected:
	bool _StartSeq( CTecoData::SInfo *pSInfo );
public:


	CTecoData *m_pTecoData;

	BYTE	*m_pVarGlobal;	// グローバル変数領域
	BYTE	*m_pVarLocal;	// ローカル変数領域
	int		*m_pArg;		// 引数領域


	bool	m_bRunning;

	CTecoData::SInfo	*m_pCurSInfo;
	CTecoData::SCommand	*m_pCurSCommand;	// 現在実行中の命令のアドレス
	CTecoData::SCommand	*m_pNextSCommand;	// 次に実行する命令のアドレス

	CTecoData::SCommand	*m_pNextOfSeg;		// 現在のセグメントの「次」、つまりセグメントの範囲外のアドレス

	int endMode;

//	int		m_ExecutedCommandNum;	// 実行したコマンドの数

//	bool	m_bFirstExec;	// 現在のコマンドの初回実行か

/*
	enum
	{
		ARG_MAX = 64,
	};

	int		m_arg[ ARG_MAX ];
*/
};


}	// namespace TecoLib
}	// common
}	// namespace nn


#endif
