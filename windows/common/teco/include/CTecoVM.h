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

	// �R�}���h�Ăяo���̖߂�l
	typedef enum
	{
		RET_COMMAND_OK_GO_NEXT,	// �����F���̖��߂̎��s�Ɉڂ�
		RET_COMMAND_OK_STOP,	// �����F���s����U��~�������߂�
		RET_COMMAND_ERROR_STOP,	// ���s�F��~����

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
			// ���[�J���ϐ�
			return (int *)&m_pVarLocal[ address ^ CTecoData::LOCAL_VARIABLE_FLAG ];
		}

		// �O���[�o���ϐ�
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

	// �w�肳�ꂽ�R�}���h�̎��̃R�}���h�̃A�h���X�𓾂�
	CTecoData::SCommand *GetNextCommand( CTecoData::SCommand *pCommand );
	// �w�肳�ꂽ�R�}���h���V�[�P���X�̍Ō�̃R�}���h�����ׂ�
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

	BYTE	*m_pVarGlobal;	// �O���[�o���ϐ��̈�
	BYTE	*m_pVarLocal;	// ���[�J���ϐ��̈�
	int		*m_pArg;		// �����̈�


	bool	m_bRunning;

	CTecoData::SInfo	*m_pCurSInfo;
	CTecoData::SCommand	*m_pCurSCommand;	// ���ݎ��s���̖��߂̃A�h���X
	CTecoData::SCommand	*m_pNextSCommand;	// ���Ɏ��s���閽�߂̃A�h���X

	CTecoData::SCommand	*m_pNextOfSeg;		// ���݂̃Z�O�����g�́u���v�A�܂�Z�O�����g�͈̔͊O�̃A�h���X

	int endMode;

//	int		m_ExecutedCommandNum;	// ���s�����R�}���h�̐�

//	bool	m_bFirstExec;	// ���݂̃R�}���h�̏�����s��

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
