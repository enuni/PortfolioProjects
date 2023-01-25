#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__

#include "save_obj_base.h"
#include "master.h"

//---------------------------------------------------------------------------
// 前方宣言
#include "class_list.h"
//---------------------------------------------------------------------------
// enum
namespace nn {
namespace yb {
namespace e {
namespace save_data {

} // namespece save_data
} // namespece e
//---------------------------------------------------------------------------
namespace main {

typedef struct _score_ranking {
	int total;
	int score[8];
	char name[16];
} SCORE_RANKING;

typedef struct _score_stage {
	int total;						// このステージのトータルベスト
	int score[8];					// トータルベストの各ホールのスコア
	int hole[8];					// 各ホールのベストスコア
	SCORE_RANKING ranking[10];		// このステージのトータルベストでのランキング
} SCORE_STAGE;

typedef struct _score {
	SCORE_STAGE bestScore[8];
} SCORE;

typedef struct _breadk_data {

	bool breakFlag;
	int stageNo;
	int holeNo;
	int score[8];
	int totalScore;
	int lp;
	int hp;

} BREAK_DATA;

typedef struct _config_data {

	int bgmVol;
	int seVol;
	int dispSize;
	int dispMask;
	int pad[MASTER_CONFIG_PAD_N];

} CONFIG_DATA;

typedef struct _breadk_data2 {
	
	int spell;

} BREAK_DATA2;

typedef struct _breadk_data3 {
	
	int stageMode;

} BREAK_DATA3;

class CSaveData : public nn_common::SaveObjBase {

public:
	CSaveData();
	CSaveData( nn_common::SaveManager * m);
	virtual nn_common::DATARECODE * getDataRecode();
	virtual int getClassId();
	void constructorCommon();
	
	static ::nn_common::DATARECODE dataRecode[];

	int editWindowSizeIndex;
	int editEditDataIndex;

	int menuStartIndex;

	nn::yb::main::SCORE score;

	nn::yb::main::BREAK_DATA breakData;

	int stageOpen;

	nn::yb::main::CONFIG_DATA configData;

	int editEditDataPage;

	int version;

	nn::yb::main::BREAK_DATA2 breakData2;

	// Ver.2
	int editSpell;
	nn::yb::main::SCORE scoreStandard;
	int stageOpenStandard;
	int gameStageModeSelect;
	int gameStageSelect[2];		// 0:chaos 1:standard
	nn::yb::main::BREAK_DATA3 breakData3;

};

} // namespace save_data
//---------------------------------------------------------------------------
} // namespace yb
} // namespace nn
#endif //__SAVE_DATA_H__