
/**
    @brief		�A�j���[�V�����Ǘ��N���X
    @author		�t���`
*/

#ifndef ___LUNAANIMATION_H___
#define ___LUNAANIMATION_H___


//=============================================================================
// INCLUDE
//=============================================================================

#include <windows.h>

#define MemoryClear(PD,S)		::FillMemory((PD),(S),0x00)													///< �������N���A
#define MemoryFill(PD,PS,S)		::FillMemory((PD),(S),(PS))													///< �������h��Ԃ�
#define MemoryMove(PD,PS,S)		::MoveMemory((PD),(PS),(S))													///< �������ړ�
#define MemoryCopy(PD,PS,S)		::CopyMemory((PD),(PS),(S))													///< �������R�s�[

#define MemoryAlloc(size)		(::VirtualAlloc( NULL, ((size)+4095)&~4095, MEM_COMMIT, PAGE_READWRITE ))					///< VirtuakAlloc�������m�ۃ}�N��
#define MemoryFree(pointer)		if ( (pointer) != NULL ) { ::VirtualFree( pointer, 0, MEM_RELEASE ); (pointer) = NULL; }	///< VirtualAlloc����������}�N��

typedef enum { INVALID_ANIMATION	= -1 } LANIMATION;			///< �A�j���[�V�����n���h��

#define toF(val)							(static_cast<float>(val))										///< float�ւ̃L���X�g
#define toI(val)							(static_cast<Sint32>(val))										///< int�ւ̃L���X�g
#define REV(val)							(1.0f/toF(val))													///< �t���Z�o�}�N��


/**
	@brief	�L�[�t���[���f�[�^

	LunaAnimation�N���X�Ŏg�p�����A�A�j���[�V������<BR>
	�L�[�t���[���̏����i�[���邽�߂̍\���̂ł�
*/
struct KEYFRAMEDATA
{
	float fTime;							///< �L�[����
	float fParam;							///< �L�[�l
	bool IsIn;								///< �X�v���C������IN�W����萔�ɂ��邩�ǂ���
	bool IsOut;								///< �X�v���C������OUT�W����萔�ɂ��邩�ǂ���
	float fIn;								///< IN�W���̒萔
	float fOut;								///< OUT�W���̒萔

	KEYFRAMEDATA *pPrev;					///< �O���L�[�t���[���f�[�^�|�C���^
	KEYFRAMEDATA *pNext;					///< ����L�[�t���[���f�[�^�|�C���^
};

/**
	@brief	��{�I�u�W�F�N�g

	�e�I�u�W�F�N�g�̊�{�ƂȂ�\���̂ł�
*/
struct OBJECTBASE
{
	bool IsUse;							///< �g�p�t���O
	int RefCount;					///< �Q�ƃJ�E���^
};
/**
	@brief	�T�E���h�f�[�^

	LunaAnimation�N���X�Ŏg�p�����A�A�j���[�V������<BR>
	�f�[�^���Ǘ����邽�߂̍\���̂ł�
*/
struct ANIMATIONDATA : public OBJECTBASE
{
	float *pNewtonWork0;					///< �j���[�g����ԗp���[�N
	float *pNewtonWork1;					///< �j���[�g����ԗp���[�N
	KEYFRAMEDATA *pKeys;					///< �L�[�t���[���f�[�^�|�C���^
	int KeyMax;					///< �L�[�t���[����
	float fTimeFirst;						///< �J�n����
	float fTimeLast;						///< �I������
};

//=============================================================================
/**
	@brief	�A�j���[�V�����Ǘ��N���X

	�A�j���[�V�������Ǘ����邽�߂̃N���X�ł��B
	���`��Ԃ���s�b�a�X�v���C���܂ŁA
	�F�X�ȕ�Ԃ��T�|�[�g���Ă��܂��B
	�������̃L�[�t���[�����w�肷�邾����
	���Ԃ��Ƃ̕�Ԃ������I�ɍs���悤�ɂȂ��Ă��܂��B
*/
//=============================================================================
/**
	@brief	�A�j���[�V�����^�C�v��`

	LunaAnimation�N���X�Ŏg�p�����A�j���[�V�����̕��@��\����`�B
*/
enum eAnimationType
{
	ANIM_TYPE_STEP,						///< �X�e�b�v���
	ANIM_TYPE_LINEAR,					///< ���`���
	ANIM_TYPE_NEWTON,					///< �j���[�g�����
	ANIM_TYPE_TCB,						///< �s�b�a�X�v���C�����
};


class LunaAnimation
{
protected:
	static ANIMATIONDATA *m_pAnimation;													///< �A�j���[�V�����f�[�^�|�C���^
	static int m_AnimMax;															///< �A�j���[�V�����f�[�^�ő吔

protected:
	static LANIMATION GetFreeHandle( void );											///< ���g�p�n���h���̎擾
	static bool Delete( LANIMATION lAnim );												///< �폜
	static void *GetKeyData( void *pData, float fTime, int KeyMax );					///< �L�[�f�[�^�擾
	static void Hermite( float fTime, float *pH1, float *pH2, float *pH3, float *pH4 );	///< �G���~�[�g�֐�
	static float InComing( void *pData0, void *pData1 );								///< ��Ԑ�[�v�Z
	static float OutGoing( void *pData0, void *pData1 );								///< ��ԏI�[�v�Z

public:
	static bool Initialize( void );														///< ������
	static void Finalize( void );														///< ���

	static void SetMax( int Max );													///< �ő吔�ݒ�
	static LANIMATION Create( int KeyMax );											///< �A�j���[�V�����I�u�W�F�N�g����
	static void Release( LANIMATION lAnim );											///< �I�u�W�F�N�g���
	static void SetKey( LANIMATION lAnim, int KeyNo, float fTime, float fParam, bool IsIn = false, bool IsOut = false, float fIn = 0.0f, float fOut = 0.0f );		///< �A�j���[�V�����L�[�w���Ĕ�
	static float GetParameter( LANIMATION lAnim, float fTime, eAnimationType Type, float fDefault );	///< �p�����[�^�[�擾
	static void Optimize( LANIMATION lAnim );											///< �A�j���[�V�����f�[�^�œK��

	static float FlatSpeed( float fStart, float fEnd, float fAll, float fNow );			///< �����^�����s���܂�
	static float AddSpeed( float fStart, float fEnd, float fAll, float fNow );			///< �����^�����s���܂�
	static float SubSpeed( float fStart, float fEnd, float fAll, float fNow );			///< �����^�����s���܂�
	static float Bezier( float p1, float p2, float p3, float t );						///< Bezier��Ԃ��s���܂�
	static float Neville( float p1, float p2, float p3, float t );						///< Neville��Ԃ��s���܂�

	

	//Tween
	static float LunaAnimation::EaseInBounce ( float fStart, float fEnd, float fAll, float fNow ) ;
	static float LunaAnimation::EaseOutBounce ( float fStart, float fEnd, float fAll, float fNow ) ;
	static float LunaAnimation::EaseInOutBounce ( float fStart, float fEnd, float fAll, float fNow ) ;
	static float LunaAnimation::EaseOutInBounce ( float fStart, float fEnd, float fAll, float fNow ) ;
	static float LunaAnimation::EaseOutElastic ( float fStart, float fEnd, float fAll, float fNow ) ;
	
	static float LunaAnimation::easeInBounce (float time, float b, float c, float d);
	static float LunaAnimation::easeOutBounce(float time, float b, float c, float d);
	static float LunaAnimation::easeInOutBounce  (float time, float b, float c, float d);
	static float LunaAnimation::easeOutInBounce  (float time, float b, float c, float d);
	static float LunaAnimation::easeOutElastic  (float time, float b, float c, float d) ;


	//added
		/**
		@brief �����^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Linear( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(Time) / toF(TimeAll);
		return (TypeA)(toF(Start) + toF(End - Start) * Rate);
	}

	/**
		@brief �����^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Add( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(Time) / toF(TimeAll);
		Rate *= Rate;
		return (TypeA)(toF(Start) + toF(End - Start) * Rate);
	}

	/**
		@brief �����^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Sub( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(TimeAll-Time) / toF(TimeAll);
		Rate *= Rate;
		return (TypeA)(toF(End) + toF(Start - End) * Rate);
	}
	/**
		@brief �����^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Add2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(Time) / toF(TimeAll);
		Rate *= Rate;
		Rate *= Rate;
		return (TypeA)(toF(Start) + toF(End - Start) * Rate);
	}

	/**
		@brief �����^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Sub2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(TimeAll-Time) / toF(TimeAll);
		Rate *= Rate;
		Rate *= Rate;
		return (TypeA)(toF(End) + toF(Start - End) * Rate);
	}

	/**
		@brief �����^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Add4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(Time) / toF(TimeAll);
		Rate *= Rate;
		Rate *= Rate;
		Rate *= Rate;
		return (TypeA)(toF(Start) + toF(End - Start) * Rate);
	}

	/**
		@brief �����^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Sub4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(TimeAll-Time) / toF(TimeAll);
		Rate *= Rate;
		Rate *= Rate;
		Rate *= Rate;
		return (TypeA)(toF(End) + toF(Start - End) * Rate);
	}

	/**
		@brief �����������^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����������^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA AddSub( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Add( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Sub( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

	/**
		@brief �����������^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����������^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA SubAdd( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Sub( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Add( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

	/**
		@brief �����������^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����������^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA AddSub2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Add2( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Sub2( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

	/**
		@brief �����������^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����������^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA SubAdd2( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Sub2( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Add2( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

	/**
		@brief �����������^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����������^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA AddSub4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Add4( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Sub4( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

	/**
		@brief �����������^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		�����������^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA SubAdd4( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		TypeA Middle = (End + Start) / (TypeA)2;
		TypeB MiddleTime = TimeAll / (TypeB)2;

		if ( Time < MiddleTime )
		{
			return Sub4( Start, Middle, MiddleTime, Time );
		}
		else
		{
			return Add4( Middle, End, TimeAll-MiddleTime, Time-MiddleTime );
		}
	}

	/**
		@brief sin�^��
		@author �t���`
		@param Start	[in] �J�n�l
		@param End		[in] �I���l
		@param TimeAll	[in] End���B����
		@param Time		[in] ���ݎ���
		@return Time�ɂ�����l
		@note
		sin�g�ŉ^�����s���Ƃ��̕�Ԓl���擾���܂��B
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Sin( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		Sint32 Angle = (Sint32)( Kernel::Math::DegToAngle(180) * Time / TimeAll);
		float Rate = Kernel::Math::FastSin( Angle );
		return (TypeA)(toF(Start) + toF(End - Start) * Rate);
	}


};


#endif // ___LUNAANIMATION_H___


