///////////////////////////////////////////////////////////////////////////////
//
//	共通ヘッダ
//
///////////////////////////////////////////////////////////////////////////////

#define __TECO__ 0

#include "..\..\..\common\dxlib\include\script_cmd.h"
#include "..\src\image_data\obj_main_3d_data.h"
#include "..\src\image_data\layout_main_3d_data.h"
// コンバータの設定
// 0:false 1:true
#setting "NeedVariableDeclaration"		1		// 変数宣言が必要か(default:false)
#setting "DefaultVariableType"			0		// デフォルト型 0:int 1:uint 2:float 3:str 4:label (default:int)

#setting "NoHeader"						0		// ヘッダを出力しない(default:false)
#setting "ResumableSegment"				0		// セグメントの再開が可能か(default:true)
#setting "MixableCodeData"				0		// セグメントにコードとデータが混在するのを許可(default:false)
#setting "DeleteEmptySegment"			1		// 空のセグメントを削除するか(default:true)
#setting "MacroLoopMax"					2000	// マクロループの繰り返し回数の上限(default:2000)

///////////////////////////////////////////////////////////////////////////////
// マクロ関数

///////////////////////////////////////////////////////////////////////////////

// セグメント開始
Seg 0

