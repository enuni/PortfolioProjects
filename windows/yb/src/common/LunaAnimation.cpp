
/**
    @brief		�A�j���[�V�����Ǘ��N���X
    @author		�t���`
*/


//=============================================================================
// INCLUDE
//=============================================================================
////#include "LunaBase.h"

#include "LunaAnimation.h"
////#include "LunaMain.h"

#include <math.h>

//=============================================================================
// VALIABLE
//=============================================================================
ANIMATIONDATA *LunaAnimation::m_pAnimation;
int LunaAnimation::m_AnimMax;


//=============================================================================
/**
	�N���X�̏����������܂�

	@retval true		����
	@retval false		���s
*/
//=============================================================================
bool LunaAnimation::Initialize( void )
{
	////Luna::LogoutMainCategory( "LunaAnimation�N���X�̏�����" );
	////Luna::LogoutNextLine();

	//----------------------------------------------------------
	// �I�u�W�F�N�g�o�b�t�@����
	//----------------------------------------------------------
	////Luna::LogoutStrong( "�A�j���[�V�����p�������m��" );
	////Luna::LogoutNextLine();

	if ( m_AnimMax == 0 ) m_AnimMax = 65536;

	m_pAnimation = (ANIMATIONDATA*)MemoryAlloc( sizeof(ANIMATIONDATA) * m_AnimMax );
	if ( m_pAnimation == NULL )
	{
		////Luna::LogoutError( "�@�@�@�A�j���[�V�����p�������̊m�ۂɎ��s" );
		////Luna::LogoutNextLine();
		return false;
	}

	////Luna::LogoutNextLine();

	return true;
}

//=============================================================================
/**
	�N���X�̉�������܂�

*/
//=============================================================================
void LunaAnimation::Finalize( void )
{
	////Luna::LogoutMainCategory( "LunaAnimation�N���X�̉��" );
	////Luna::LogoutNextLine();

	for ( int i = 0; i < m_AnimMax; i++ )
	{
		LunaAnimation::Delete( (LANIMATION)i );
	}

	MemoryFree( m_pAnimation );

	////Luna::LogoutNextLine();
}

//=============================================================================
/**
	���g�p�A�j���[�V�����n���h�����擾���܂�

	@return		�A�j���[�V�����n���h��
*/
//=============================================================================
LANIMATION LunaAnimation::GetFreeHandle( void )
{
	for ( int i = 0; i < m_AnimMax; i++ )
	{
		if ( !m_pAnimation[i].IsUse )
		{
			m_pAnimation[i].IsUse = true;
			return (LANIMATION)i;
		}
	}

	return INVALID_ANIMATION;
}



//=============================================================================
/**
	�A�j���[�V�����f�[�^������������폜���܂�

	@param lAnim		[in] �A�j���[�V�����n���h��

	@retval true		����
	@retval false		���s
*/
//=============================================================================
bool LunaAnimation::Delete( LANIMATION lAnim )
{
	if ( lAnim == INVALID_ANIMATION ) return false;

	if ( m_pAnimation[lAnim].IsUse )
	{
		MemoryFree( m_pAnimation[lAnim].pKeys );
		MemoryFree( m_pAnimation[lAnim].pNewtonWork0 );
		MemoryFree( m_pAnimation[lAnim].pNewtonWork1 );
		MemoryClear( &m_pAnimation[lAnim], sizeof(ANIMATIONDATA) );

		return true;
	}

	return false;
}

//=============================================================================
/**
	�A�j���[�V�����f�[�^�̍ő吔��ݒ肵�܂�

	@param Max		[in] �ݒ肷��ő吔

*/
//=============================================================================
void LunaAnimation::SetMax( int Max )
{
	m_AnimMax = Max;
}

//=============================================================================
/**
	�w�莞�Ԃ���ł��߂��L�[�f�[�^���擾���܂�

	@param pData		[in] �L�[�t���[���f�[�^
	@param fTime		[in] ��������
	@param KeyMax		[in] �L�[�ő吔

	@return			�L�[�t���[���f�[�^�|�C���^
*/
//=============================================================================
void *LunaAnimation::GetKeyData( void *pData, float fTime, int KeyMax )
{
	KEYFRAMEDATA *pKeys = (KEYFRAMEDATA *)pData;

	int Left	= 0;
	int Right	= KeyMax - 1;

	if ( fTime <= pKeys[Left ].fTime ) return &pKeys[Left ];
	if ( fTime >= pKeys[Right].fTime ) return &pKeys[Right];

	while ( Left <= Right )
	{
		int Middle = Left + (Right - Left) / 2;

		if ( fTime < pKeys[Middle+0].fTime )
		{
			Right = Middle - 1;
		}
		else if ( fTime > pKeys[Middle+1].fTime )
		{
			Left = Middle + 1;
		}
		else
		{
			return &pKeys[Middle];
		}
	}

	return NULL;
}

//=============================================================================
/**
	�A�j���[�V�����f�[�^�𐶐����܂�

	@param KeyMax		[in] �L�[�ő吔

	@return			�A�j���[�V�����n���h��
*/
//=============================================================================
LANIMATION LunaAnimation::Create( int KeyMax )
{
	LANIMATION lAnim = INVALID_ANIMATION;

	lAnim = LunaAnimation::GetFreeHandle();
	if ( lAnim == INVALID_ANIMATION )
	{
		return INVALID_ANIMATION;
	}

	// �������m��
	m_pAnimation[lAnim].KeyMax = KeyMax;
	m_pAnimation[lAnim].pKeys = (KEYFRAMEDATA*)MemoryAlloc( sizeof(KEYFRAMEDATA) * KeyMax );
	m_pAnimation[lAnim].pNewtonWork0 = (float*)MemoryAlloc( sizeof(float) * KeyMax );
	m_pAnimation[lAnim].pNewtonWork1 = (float*)MemoryAlloc( sizeof(float) * KeyMax );

	// �O��Ƀ����N���X�g��ڑ�
	for ( int i = 0; i < KeyMax - 1; i++ )
	{
		m_pAnimation[lAnim].pKeys[i].pNext = &m_pAnimation[lAnim].pKeys[i+1];
	}
	for ( int i = KeyMax - 1; i >= 1; i-- )
	{
		m_pAnimation[lAnim].pKeys[i].pPrev = &m_pAnimation[lAnim].pKeys[i-1];
	}

	return lAnim;
}

//=============================================================================
/**
	�A�j���[�V�����f�[�^�����������������܂�

	@param lAnim		[in] �A�j���[�V�����n���h��

*/
//=============================================================================
void LunaAnimation::Release( LANIMATION lAnim )
{
	if ( lAnim == INVALID_ANIMATION ) return;

	if ( m_pAnimation[lAnim].IsUse )
	{
		if ( LunaAnimation::Delete( lAnim ) )
		{
		}
	}
}

//=============================================================================
/**
	�L�[�t���[���f�[�^�ɃL�[��ݒ肵�܂�

	@param lAnim		[in] �A�j���[�V�����n���h��
	@param KeyNo		[in] �ݒ��L�[�ԍ�
	@param fTime		[in] �ݒ莞��
	@param fParam		[in] �ݒ�p�����[�^
	@param IsIn			[in] ���̓p�����[�^�[�w��L��
	@param IsOut		[in] �o�̓p�����[�^�[�w��L��
	@param fIn			[in] ���̓p�����[�^�[
	@param fOut			[in] �o�̓p�����[�^�[

*/
//=============================================================================
void LunaAnimation::SetKey( LANIMATION lAnim, int KeyNo, float fTime, float fParam, bool IsIn, bool IsOut, float fIn, float fOut )
{
	if ( lAnim == INVALID_ANIMATION ) return;

	// �L�[�ԍ���ݒ�
	KeyNo %= m_pAnimation[lAnim].KeyMax;

	// �L�[�ݒ�
	m_pAnimation[lAnim].pKeys[KeyNo].fTime  = fTime;
	m_pAnimation[lAnim].pKeys[KeyNo].fParam = fParam;
	m_pAnimation[lAnim].pKeys[KeyNo].IsIn   = IsIn;
	m_pAnimation[lAnim].pKeys[KeyNo].IsOut  = IsOut;
	m_pAnimation[lAnim].pKeys[KeyNo].fIn    = fIn;
	m_pAnimation[lAnim].pKeys[KeyNo].fOut   = fOut;

	// �J�n�|�I������
	m_pAnimation[lAnim].fTimeFirst = m_pAnimation[lAnim].pKeys[0].fTime;
	m_pAnimation[lAnim].fTimeLast = m_pAnimation[lAnim].pKeys[m_pAnimation[lAnim].KeyMax-1].fTime;
}

//=============================================================================
/**
	�G���~�[�g��Ԋ֐����������܂�

	@param fTime	[in] ����
	@param pH1		[out] �p�����[�^�P
	@param pH2		[out] �p�����[�^�Q
	@param pH3		[out] �p�����[�^�R
	@param pH4		[out] �p�����[�^�S

*/
//=============================================================================
void LunaAnimation::Hermite( float fTime, float *pH1, float *pH2, float *pH3, float *pH4 )
{
   float fTime2 = fTime * fTime;
   float fTime3 = fTime * fTime2;

   *pH2 = (3.0f * fTime2) - fTime3 - fTime3;
   *pH1 = 1.0f - *pH2;
   *pH4 = fTime3 - fTime2;
   *pH3 = *pH4 - fTime2 + fTime;
}

//=============================================================================
/**
	�O���̃L�[�̏o�͐�̃p�����[�^�����߂܂�

	@param pData0	[in] �O���L�[�f�[�^
	@param pData1	[in] ����L�[�f�[�^

	@return		�Z�o���ꂽ�p�����[�^�[
*/
//=============================================================================
float LunaAnimation::InComing( void *pData0, void *pData1 )
{
	KEYFRAMEDATA *pKey0 = (KEYFRAMEDATA *)pData0;
	KEYFRAMEDATA *pKey1 = (KEYFRAMEDATA *)pData1;

	float fParam = pKey1->fParam - pKey0->fParam;

	if ( pKey1->pNext != NULL )
	{
		float fTime = (pKey1->fTime - pKey0->fTime) / (pKey1->pNext->fTime - pKey0->fTime);
		return (fTime * ((pKey1->pNext->fParam - pKey1->fParam) + fParam));
	}
	else
	{
		return fParam;
	}
}

//=============================================================================
/**
	����̃L�[�̏o�͐�̃p�����[�^�����߂܂�

	@param pData0	[in] �O���L�[�f�[�^
	@param pData1	[in] ����L�[�f�[�^

	@return		�Z�o���ꂽ�p�����[�^�[
*/
//=============================================================================
float LunaAnimation::OutGoing( void *pData0, void *pData1 )
{
	KEYFRAMEDATA *pKey0 = (KEYFRAMEDATA *)pData0;
	KEYFRAMEDATA *pKey1 = (KEYFRAMEDATA *)pData1;

	float fParam = pKey1->fParam - pKey0->fParam;

	if ( pKey0->pPrev != NULL )
	{
		float fTime = (pKey1->fTime - pKey0->fTime) / (pKey1->fTime - pKey0->pPrev->fTime);
		return (fTime * ((pKey0->fParam - pKey0->pPrev->fParam) + fParam));
	}
	else
	{
		return fParam;
	}
}

//=============================================================================
/**
	�w��^�C�v�̃A�j���[�V�����Ŏw�莞�Ԃɂ�����l���擾���܂�

	@param lAnim		[in] �A�j���[�V�����n���h��
	@param fTime		[in] ����
	@param Type			[in] ��ԃ^�C�v
	@param fDefault		[in] �f�t�H���g�l

	@return		��Ԍ��ʒl
*/
//=============================================================================
float LunaAnimation::GetParameter( LANIMATION lAnim, float fTime, eAnimationType Type, float fDefault )
{
	if ( lAnim == INVALID_ANIMATION ) return fDefault;

	ANIMATIONDATA *pAnim = &m_pAnimation[lAnim];

	//--------------------------------------------------
	// ���Ԃ̓N�����v����
	//--------------------------------------------------
	if ( fTime < 0.0f )
	{
		fTime = 0.0f;
	}
	else if ( fTime > m_pAnimation[lAnim].fTimeLast )
	{
		fTime = m_pAnimation[lAnim].fTimeLast;
	}

	//--------------------------------------------------
	// �w�莞�Ԃ̃L�[�擾
	//--------------------------------------------------
	KEYFRAMEDATA *pKey0 = (KEYFRAMEDATA*)GetKeyData( pAnim->pKeys, fTime, pAnim->KeyMax );
	KEYFRAMEDATA *pKey1 = pKey0->pNext;

	if ( pKey1 == NULL ) return pKey0->fParam;

	//--------------------------------------------------
	// ��Ԋ����擾
	//--------------------------------------------------
	float fLerpValue = 0;
	if ( pKey0->fParam != pKey1->fParam )
	{
		fLerpValue = (fTime - pKey0->fTime) / (pKey1->fTime - pKey0->fTime);
	}

	//--------------------------------------------------
	// �L�[���
	//--------------------------------------------------
	switch ( Type )
	{
	// Step
	case ANIM_TYPE_STEP:
		{
			return pKey0->fParam;
		}
		break;
	// Linear
	case ANIM_TYPE_LINEAR:
		{
			return (pKey0->fParam + ((pKey1->fParam - pKey0->fParam) * fLerpValue));
		}
		break;
	// Newton
	case ANIM_TYPE_NEWTON:
		{
			float f = pAnim->pNewtonWork0[pAnim->KeyMax - 1];
			for ( int i = pAnim->KeyMax - 2; i >= 0; i-- )
			{
				f = f * (fTime - pAnim->pKeys[i].fTime) + pAnim->pNewtonWork0[i];
			}

			return f;
		}
		break;
	// TCB (Kochanek-Bartels) 
	case ANIM_TYPE_TCB:
		{
			float fIn, fOut;
			float h1, h2, h3, h4;

			// �|�C���g�Z�o
			if ( pKey0->IsOut ) fOut = pKey0->fOut;
			else                fOut = OutGoing( pKey0, pKey1 );
			if ( pKey1->IsIn  ) fIn  = pKey1->fIn;
			else                fIn  = InComing( pKey0, pKey1 );

			// �G���~�[�g
			Hermite( fLerpValue, &h1, &h2, &h3, &h4 );

			return ((h1 * pKey0->fParam) + (h2 * pKey1->fParam) + (h3 * fOut) + (h4 * fIn));
		}
		break;
	}

	return fDefault;
}

//=============================================================================
/**
	��ԏ����������p�ɍœK�������܂�

	@param lAnim	[in] �A�j���[�V�����n���h��
*/
//=============================================================================
void LunaAnimation::Optimize( LANIMATION lAnim )
{
	if ( lAnim == INVALID_ANIMATION ) return;

	ANIMATIONDATA *pAnim = &m_pAnimation[lAnim]; 

	//--------------------------------------------------------
	// �j���[�g����ԗp�W���Z�o
	//--------------------------------------------------------
	for ( int i = 0; i < (int)pAnim->KeyMax; i++ )
	{
		pAnim->pNewtonWork1[i] = pAnim->pKeys[i].fParam;
		for ( int j = i - 1; j >= 0; j-- )
		{
			float fTemp0 = pAnim->pNewtonWork1[j+1] - pAnim->pNewtonWork1[j];
			float fTime1 = pAnim->pKeys[i].fTime - pAnim->pKeys[j].fTime;
			pAnim->pNewtonWork1[j] = fTemp0 / fTime1;
		}
		pAnim->pNewtonWork0[i] = pAnim->pNewtonWork1[0];
	}
}

//=============================================================================
/**
	�����^���p�̕�Ԋ֐��ł�

	@param start	[in] �n�_
	@param end		[in] �I�_
	@param all		[in] �n�_����I�_�܂łɂ����鎞��
	@param now		[in] ���݂̎���

	@return		now�ɂ������Ԃ��ꂽ�l
*/
//=============================================================================
float LunaAnimation::FlatSpeed( float fStart, float fEnd, float fAll, float fNow )
{
	float fTime = fNow / fAll;
	float fRate = fTime;

	return fStart + (fRate * (fEnd - fStart));
}

//=============================================================================
/**
	�����^���p�̕�Ԋ֐��ł�

	@param start	[in] �n�_
	@param end		[in] �I�_
	@param all		[in] �n�_����I�_�܂łɂ����鎞��
	@param now		[in] ���݂̎���

	@return		now�ɂ������Ԃ��ꂽ�l
*/
//=============================================================================
float LunaAnimation::AddSpeed( float fStart, float fEnd, float fAll, float fNow )
{
	float fTime = fNow / fAll;
	float fRate = fTime * fTime;

	return fStart + (fRate * (fEnd - fStart));
}

//=============================================================================
/**
	�����^���p�̕�Ԋ֐��ł�

	@param start	[in] �n�_
	@param end		[in] �I�_
	@param all		[in] �n�_����I�_�܂łɂ����鎞��
	@param now		[in] ���݂̎���

	@return		now�ɂ������Ԃ��ꂽ�l
*/
//=============================================================================
float LunaAnimation::SubSpeed( float fStart, float fEnd, float fAll, float fNow )
{
	float fTime = fNow / fAll;
	float fRate = 1.0f - (fTime * fTime);

	return fEnd + (fRate * (fStart - fEnd));
}

//=============================================================================
/**
	�x�W�F�̕�Ԋ֐��ł�

	@param p1	[in] �n�_
	@param p2	[in] ���_
	@param p3	[in] �I�_
	@param t	[in] ���݂̎���(0�`1)

	@return		t�ɂ�����l
*/
//=============================================================================
float LunaAnimation::Bezier( float p1, float p2, float p3, float t )
{
	return ( ((1-t)*(1-t)*p1) + (2*(1-t)*t*p2) + (t*t*p3) );
}

//=============================================================================
/**
	Neville�̕�Ԋ֐��ł�

	@param p1	[in] �n�_
	@param p2	[in] ���_
	@param p3	[in] �I�_
	@param t	[in] ���݂̎���(0�`1)

	@return		t�ɂ�����l
*/
//=============================================================================
float LunaAnimation::Neville( float p1, float p2, float p3, float t )
{
	t *= 2;
	p1 = p2 + ( p2 - p1 ) * ( t - 1 );
	p2 = p3 + ( p3 - p2 ) * ( t - 2 );
	return p2 + ( p2 - p1 ) * ( t - 2 ) * 0.5f;
}



//Tween
//�A�j���[�V�������b�p(�����𑵂���)

float LunaAnimation::EaseInBounce ( float fStart, float fEnd, float fAll, float fNow ) 
{
	return LunaAnimation::easeInBounce( fNow, fStart, (fEnd - fStart), fAll);
}
float LunaAnimation::EaseOutBounce ( float fStart, float fEnd, float fAll, float fNow ) 
{
	return LunaAnimation::easeOutBounce( fNow, fStart, (fEnd - fStart), fAll);
}

float LunaAnimation::EaseInOutBounce ( float fStart, float fEnd, float fAll, float fNow ) 
{
	return LunaAnimation::easeInOutBounce( fNow, fStart, (fEnd - fStart), fAll);
}

float LunaAnimation::EaseOutInBounce ( float fStart, float fEnd, float fAll, float fNow ) 
{
	return LunaAnimation::easeOutInBounce( fNow, fStart, (fEnd - fStart), fAll);
}

float LunaAnimation::EaseOutElastic ( float fStart, float fEnd, float fAll, float fNow ) 
{
	return LunaAnimation::easeOutElastic( fNow, fStart, (fEnd - fStart), fAll);
}



//��ԃA�j���[�V����(bound)
/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param b		Starting value.
 * @param c		Change needed in value.
 * @param d		Expected easing duration (in frames or seconds).
 * @return		The correct value.
 */
float LunaAnimation::easeInBounce (float time, float b, float c, float d) 
{
	return (c - easeOutBounce(d-time, 0.0, c, d) + b);
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param b		Starting value.
 * @param c		Change needed in value.
 * @param d		Expected easing duration (in frames or seconds).
 * @return		The correct value.
 */
 
float LunaAnimation::easeOutBounce(float time, float b, float c, float d)
{
	////time = time / d;
	////if (time < (1.0 / 2.75)) 
	////{
	////	return (c * ( 7.5625 * time * time) + b);
	////}
	////else if (time < (2.0 / 2.75)) 
	////{
	////	time -= (1.5 / 2.75);
	////	return (c * (7.5625 * time * time + 0.75) + b);
	////} 
	////else if (time < (2.5 / 2.75)) 
	////{
	////	time -= (2.25 / 2.75);
	////	return (c * (7.5625 * (time) * time + 0.9375) + b);
	////} 
	////else 
	////{
	////	time -= (2.625 / 2.75);
	////	return (c * (7.5625 * (time) * time + 0.984375) + b);
	////}
	if ((time/=d) < (1.0f/2.75f)) {
		return c*(7.5625f*time*time) + b;
	} else if (time < (2.0f/2.75f)) {
		return c*(7.5625f*(time-=(1.5f/2.75f))*time + 0.75f) + b;
	} else if (time < (2.5/2.75f)) {
		return c*(7.5625f*(time-=(2.25f/2.75f))*time + 0.9375f) + b;
	} else {
		return c*(7.5625f*(time-=(2.625f/2.75f))*time + 0.984375f) + b;
	}
}


/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param b		Starting value.
 * @param c		Change needed in value.
 * @param d		Expected easing duration (in frames or seconds).
 * @return		The correct value.
 */
float LunaAnimation::easeInOutBounce  (float t, float b, float c, float d)
{
	if (t < d/2.0f) return easeInBounce (t*2.0f, 0.0f, c, d) * 0.5f + b;
	else return easeOutBounce (t*2.0f-d, 0.0f, c, d) * 0.5f + c*0.5f + b;
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param b		Starting value.
 * @param c		Change needed in value.
 * @param d		Expected easing duration (in frames or seconds).
 * @return		The correct value.
 */
float LunaAnimation::easeOutInBounce  (float t, float b, float c, float d)
{
	if (t < d/2.0f) return easeOutBounce (t*2.0f, b, c/2.0f, d);
	return easeInBounce((t*2.0f)-d, b+c/2.0f, c/2.0f, d);
}


/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param b		Starting value.
 * @param c		Change needed in value.
 * @param d		Expected easing duration (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
float LunaAnimation::easeOutElastic  (float t, float b, float c, float d) 
{
	if (t == 0.0f) 
		return b;
	
	t = t / d;
	if (t == 1.0f)
	{
		
		return c;
	}
	
//	 p = !Boolean(p_params) || isNaN(p_params.period) ? d * 0.3 : p_params.period;
	 float p = d * 0.3f;
	
	 float s = 0.0f;
	
//	 a = !Boolean(p_params) || isNaN(p_params.amplitude) ? 0.0 : p_params.amplitude;
	 float a = 0.0f;
	if (
	//	!Boolean(a) || 
		a < ::abs(c)
	)
	{
		a = c / 2.0f;
		s = p/4.0f;
	} else {
		////s = p/(2.0 * 3.14152) * math::asin ( c / a );
		s = p/(2.0f * 3.14152f) * ::asinf ( c / a );
	}
	
	float rad_angle = (t*d-s)*(2.0f * 3.14152f) / p;
	float ret = (a * ::pow(2.0f,-10.0f * t) * ::sin( rad_angle ) + c);
	
	return ret; 
}