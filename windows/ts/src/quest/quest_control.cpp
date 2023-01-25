// quest_control を置換
// QuestControl を置換
// ts を置換
// quest を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "quest.h"
#include "ts_save_manager.h"

#include "quest_control.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CQuestControl::dataRecode[] = {
	DATA_LOCAL( nn::ts::quest::CQuestControl, state),
	DATA_LOCAL( nn::ts::quest::CQuestControl, oldState),
	DATA_LOCAL( nn::ts::quest::CQuestControl, waitCnt),
	DATA_LOCAL( nn::ts::quest::CQuestControl, frameCnt),
	DATA_LOCAL( nn::ts::quest::CQuestControl, gm),
	DATA_PTR( nn::ts::quest::CQuestControl, lm.quest),
	DATA_BASE(::TASK_SAVE::dataRecode),
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::quest::CQuestControl::CQuestControl(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CQuestControl Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CQuestControl::CQuestControl(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CQuestControl Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CQuestControl::CQuestControl() : TASK_SAVE() {

	::nn_commonLog( 1, "CQuestControl Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::constructCommon() {
	
	this->state = nn::ts::e::quest_control::STATE_NONE;
	this->oldState = nn::ts::e::quest_control::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;
	
	strcpy_s( this->name, sizeof( this->name), "CQuestControl");

}
//---------------------------------------------------------------------------
nn::ts::quest::CQuestControl::~CQuestControl() {

	::nn_commonLog( 1, "CQuestControl Delete\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::startCommon() {

	this->createQuest();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CQuestControl::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::quest_control::STATE_NONE:
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::disp() {

	switch(this->state) {
		case nn::ts::e::quest_control::STATE_NONE:
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::quest::CQuestControl::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_QUEST_CONTROL;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CQuestControl::getDataRecode() {
	return nn::ts::quest::CQuestControl::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::addTe( int angle, bool spell) {
	
	this->gm.log.lastTurn = this->lm.quest->gm.status.turn;

	this->gm.log.te[this->gm.log.lastTurn].angle = angle;
	this->gm.log.te[this->gm.log.lastTurn].spell = spell;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::dump() {
	
	nn_commonLog( 1, "---------- nn::ts::quest::CQuestControl::dump() start ----------\n");

	this->lm.quest->dump();

	nn_commonLog( 1, "---------- nn::ts::quest::CQuestControl::dump() end   ----------\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::createQuest() {

	nn::ts::quest::CQuest * quest = new nn::ts::quest::CQuest( this, this->saveManager);
	quest->lm.questCtrl = this;
	quest->lm.ctrl = this->lm.ctrl;
	quest->start();

	this->lm.quest = quest;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CQuestControl::resetQuest() {

	this->lm.quest->taskDelete();

	this->createQuest();

}
//---------------------------------------------------------------------------

