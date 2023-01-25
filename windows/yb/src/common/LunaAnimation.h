
/**
    @brief		アニメーション管理クラス
    @author		葉迩倭
*/

#ifndef ___LUNAANIMATION_H___
#define ___LUNAANIMATION_H___


//=============================================================================
// INCLUDE
//=============================================================================

#include <windows.h>

#define MemoryClear(PD,S)		::FillMemory((PD),(S),0x00)													///< メモリクリア
#define MemoryFill(PD,PS,S)		::FillMemory((PD),(S),(PS))													///< メモリ塗りつぶし
#define MemoryMove(PD,PS,S)		::MoveMemory((PD),(PS),(S))													///< メモリ移動
#define MemoryCopy(PD,PS,S)		::CopyMemory((PD),(PS),(S))													///< メモリコピー

#define MemoryAlloc(size)		(::VirtualAlloc( NULL, ((size)+4095)&~4095, MEM_COMMIT, PAGE_READWRITE ))					///< VirtuakAllocメモリ確保マクロ
#define MemoryFree(pointer)		if ( (pointer) != NULL ) { ::VirtualFree( pointer, 0, MEM_RELEASE ); (pointer) = NULL; }	///< VirtualAllocメモリ解放マクロ

typedef enum { INVALID_ANIMATION	= -1 } LANIMATION;			///< アニメーションハンドル

#define toF(val)							(static_cast<float>(val))										///< floatへのキャスト
#define toI(val)							(static_cast<Sint32>(val))										///< intへのキャスト
#define REV(val)							(1.0f/toF(val))													///< 逆数算出マクロ


/**
	@brief	キーフレームデータ

	LunaAnimationクラスで使用される、アニメーションの<BR>
	キーフレームの情報を格納するための構造体です
*/
struct KEYFRAMEDATA
{
	float fTime;							///< キー時間
	float fParam;							///< キー値
	bool IsIn;								///< スプライン時のIN係数を定数にするかどうか
	bool IsOut;								///< スプライン時のOUT係数を定数にするかどうか
	float fIn;								///< IN係数の定数
	float fOut;								///< OUT係数の定数

	KEYFRAMEDATA *pPrev;					///< 前方キーフレームデータポインタ
	KEYFRAMEDATA *pNext;					///< 後方キーフレームデータポインタ
};

/**
	@brief	基本オブジェクト

	各オブジェクトの基本となる構造体です
*/
struct OBJECTBASE
{
	bool IsUse;							///< 使用フラグ
	int RefCount;					///< 参照カウンタ
};
/**
	@brief	サウンドデータ

	LunaAnimationクラスで使用される、アニメーションの<BR>
	データを管理するための構造体です
*/
struct ANIMATIONDATA : public OBJECTBASE
{
	float *pNewtonWork0;					///< ニュートン補間用ワーク
	float *pNewtonWork1;					///< ニュートン補間用ワーク
	KEYFRAMEDATA *pKeys;					///< キーフレームデータポインタ
	int KeyMax;					///< キーフレーム数
	float fTimeFirst;						///< 開始時間
	float fTimeLast;						///< 終了時間
};

//=============================================================================
/**
	@brief	アニメーション管理クラス

	アニメーションを管理するためのクラスです。
	線形補間からＴＣＢスプラインまで、
	色々な補間をサポートしています。
	いくつかのキーフレームを指定するだけで
	時間ごとの補間を自動的に行うようになっています。
*/
//=============================================================================
/**
	@brief	アニメーションタイプ定義

	LunaAnimationクラスで使用されるアニメーションの方法を表す定義。
*/
enum eAnimationType
{
	ANIM_TYPE_STEP,						///< ステップ補間
	ANIM_TYPE_LINEAR,					///< 線形補間
	ANIM_TYPE_NEWTON,					///< ニュートン補間
	ANIM_TYPE_TCB,						///< ＴＣＢスプライン補間
};


class LunaAnimation
{
protected:
	static ANIMATIONDATA *m_pAnimation;													///< アニメーションデータポインタ
	static int m_AnimMax;															///< アニメーションデータ最大数

protected:
	static LANIMATION GetFreeHandle( void );											///< 未使用ハンドルの取得
	static bool Delete( LANIMATION lAnim );												///< 削除
	static void *GetKeyData( void *pData, float fTime, int KeyMax );					///< キーデータ取得
	static void Hermite( float fTime, float *pH1, float *pH2, float *pH3, float *pH4 );	///< エルミート関数
	static float InComing( void *pData0, void *pData1 );								///< 補間先端計算
	static float OutGoing( void *pData0, void *pData1 );								///< 補間終端計算

public:
	static bool Initialize( void );														///< 初期化
	static void Finalize( void );														///< 解放

	static void SetMax( int Max );													///< 最大数設定
	static LANIMATION Create( int KeyMax );											///< アニメーションオブジェクト生成
	static void Release( LANIMATION lAnim );											///< オブジェクト解放
	static void SetKey( LANIMATION lAnim, int KeyNo, float fTime, float fParam, bool IsIn = false, bool IsOut = false, float fIn = 0.0f, float fOut = 0.0f );		///< アニメーションキー背って尾
	static float GetParameter( LANIMATION lAnim, float fTime, eAnimationType Type, float fDefault );	///< パラメーター取得
	static void Optimize( LANIMATION lAnim );											///< アニメーションデータ最適化

	static float FlatSpeed( float fStart, float fEnd, float fAll, float fNow );			///< 等速運動を行います
	static float AddSpeed( float fStart, float fEnd, float fAll, float fNow );			///< 加速運動を行います
	static float SubSpeed( float fStart, float fEnd, float fAll, float fNow );			///< 減速運動を行います
	static float Bezier( float p1, float p2, float p3, float t );						///< Bezier補間を行います
	static float Neville( float p1, float p2, float p3, float t );						///< Neville補間を行います

	

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
		@brief 等速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		等速運動を行うときの補間値を取得します。
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Linear( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(Time) / toF(TimeAll);
		return (TypeA)(toF(Start) + toF(End - Start) * Rate);
	}

	/**
		@brief 加速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		加速運動を行うときの補間値を取得します。
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Add( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(Time) / toF(TimeAll);
		Rate *= Rate;
		return (TypeA)(toF(Start) + toF(End - Start) * Rate);
	}

	/**
		@brief 減速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		減速運動を行うときの補間値を取得します。
	*/
	template <typename TypeA, typename TypeB>
	static inline TypeA Sub( TypeA Start, TypeA End, TypeB TimeAll, TypeB Time )
	{
		float Rate = toF(TimeAll-Time) / toF(TimeAll);
		Rate *= Rate;
		return (TypeA)(toF(End) + toF(Start - End) * Rate);
	}
	/**
		@brief 加速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		加速運動を行うときの補間値を取得します。
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
		@brief 減速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		減速運動を行うときの補間値を取得します。
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
		@brief 加速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		加速運動を行うときの補間値を取得します。
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
		@brief 減速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		減速運動を行うときの補間値を取得します。
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
		@brief 加速→減速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		加速→減速運動を行うときの補間値を取得します。
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
		@brief 減速→加速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		加速→減速運動を行うときの補間値を取得します。
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
		@brief 加速→減速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		加速→減速運動を行うときの補間値を取得します。
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
		@brief 減速→加速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		減速→加速運動を行うときの補間値を取得します。
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
		@brief 加速→減速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		加速→減速運動を行うときの補間値を取得します。
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
		@brief 減速→加速運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		減速→加速運動を行うときの補間値を取得します。
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
		@brief sin運動
		@author 葉迩倭
		@param Start	[in] 開始値
		@param End		[in] 終了値
		@param TimeAll	[in] End到達時間
		@param Time		[in] 現在時間
		@return Timeにおける値
		@note
		sin波で運動を行うときの補間値を取得します。
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


