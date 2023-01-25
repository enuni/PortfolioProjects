#ifndef __TECO__
#pragma once

// ゲーム固有の変数はこのファイルをゲーム名_script_var.hなどで持っていく
// namespaceを更新する
// script_var.hのincludeのコメントアウトを外しこのファイルをincludeする(パスはゲームのディレクトリに合せて適宜修正)
// 0x00:の開始アドレスを0x400:にする(このファイルのGlobalCommonVarサイズ分)
// GlobalCommonVarメンバーと// 外部変数の$ansなどの名前をゲーム固有で必要な変数に変える
// script共通のheaderファイルでゲーム名_script_var.hをincludeする
// ゲーム内で共通とゲーム固有のGlobalCommonVar分のメモリを確保して、その先頭アドレスをstartScriptで渡す
// CからはGlobalCommonVarメンバーでアクセス、スクリプトからは// 外部変数の$ansなどの名前でアクセス
// 変数追加はGlobalCommonVarメンバーと// 外部変数以下に名前を追加

namespace nn {
namespace dxlib {
namespace script {
	
// （スクリプトにとっての）外部変数
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
// 外部変数

0x00:
	int $ans

///////////////////////////////////////////////////////////////////////////////
#endif //WIN32

