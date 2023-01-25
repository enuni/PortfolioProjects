
#include "yb_save_manager.h"
#include "save_data.h"

//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::yb::main::CSaveData::dataRecode[] = {
	DATA_LOCAL( nn::yb::main::CSaveData, editWindowSizeIndex),
	DATA_LOCAL( nn::yb::main::CSaveData, editEditDataIndex),
	DATA_LOCAL( nn::yb::main::CSaveData, menuStartIndex),
	DATA_LOCAL( nn::yb::main::CSaveData, score),
	DATA_LOCAL( nn::yb::main::CSaveData, breakData),
	DATA_LOCAL( nn::yb::main::CSaveData, stageOpen),
	DATA_LOCAL( nn::yb::main::CSaveData, configData),
	DATA_LOCAL( nn::yb::main::CSaveData, version),
	DATA_LOCAL( nn::yb::main::CSaveData, editEditDataPage),
	DATA_LOCAL( nn::yb::main::CSaveData, breakData2),
	DATA_LOCAL( nn::yb::main::CSaveData, editSpell),
	DATA_LOCAL( nn::yb::main::CSaveData, scoreStandard),
	DATA_LOCAL( nn::yb::main::CSaveData, stageOpenStandard),
	DATA_LOCAL( nn::yb::main::CSaveData, gameStageModeSelect),
	DATA_LOCAL( nn::yb::main::CSaveData, gameStageSelect),
	DATA_LOCAL( nn::yb::main::CSaveData, breakData3),
	DATA_END,
};
//---------------------------------------------------------------------------
::nn::yb::main::CSaveData::CSaveData() : nn_common::SaveObjBase() {
	
	printf( "CDataHole() Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
::nn::yb::main::CSaveData::CSaveData( ::nn_common::SaveManager * m) : nn_common::SaveObjBase( m) {
	
	printf( "CDataHole( SaveManager) Create\n");
	this->constructorCommon();

}
//---------------------------------------------------------------------------
void ::nn::yb::main::CSaveData::constructorCommon() {

	this->editWindowSizeIndex = 0;
	this->editEditDataIndex = 0;

	this->menuStartIndex = 0;

	memset( &this->score, 0, sizeof( nn::yb::main::SCORE));
	memset( &this->breakData, 0, sizeof( nn::yb::main::BREAK_DATA));
	memset( &this->configData, 0, sizeof( nn::yb::main::CONFIG_DATA));

	this->stageOpen = 0;

	this->editEditDataPage = 0;

	this->version = 0;

	this->editSpell = 0;
	memset( &this->scoreStandard, 0, sizeof( nn::yb::main::SCORE));
	this->stageOpenStandard = 0;
	this->gameStageModeSelect = 0;
	memset( this->gameStageSelect, 0, sizeof(this->gameStageSelect));

}
//---------------------------------------------------------------------------
::nn_common::DATARECODE * ::nn::yb::main::CSaveData::getDataRecode() {

	return ::nn::yb::main::CSaveData::dataRecode;

}
//---------------------------------------------------------------------------
int ::nn::yb::main::CSaveData::getClassId() {

	return nn::yb::e::yb_save_manager::CLASS_ID_SAVE_DATA;

}
//---------------------------------------------------------------------------
