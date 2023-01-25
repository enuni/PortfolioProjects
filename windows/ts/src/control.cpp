// control を置換
// Control を置換
// ts を置換
// common を置換

#include <string>
#include <map>
#include <time.h>

#include "nn_common.h"
#include "nn_common2.h"
#include "disp.h"
#include "touch.h"
#include "script.h"

#include "ts_save_manager.h"
#include "ts_common.h"

#include "quest.h"
#include "enemy.h"
#include "battle.h"
#include "quest_control.h"
#include "title.h"
#include "debug_menu.h"

#include "control.h"

extern nn::dxlib::CScript * script;
extern nn::dxlib::script::GlobalCommonVar * common_var;
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::common::CControl::dataRecode[] = {
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::common::CControl::CControl(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CControl Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::common::CControl::CControl(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CControl Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::common::CControl::CControl() : TASK_SAVE() {

	::nn_commonLog( 1, "CControl Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::constructCommon() {
	
	this->state = nn::ts::e::control::STATE_NONE;
	this->oldState = nn::ts::e::control::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;
	
	this->questCtrl = NULL;

	memset( &this->gm, 0, sizeof(this->gm));

	this->enemyName2Id["ルーミア"] = nn::ts::e::common::ENEMY_ID_RUMIA;
	this->enemyName2Id["妖精青"] = nn::ts::e::common::ENEMY_ID_YOSEIAO;
	this->enemyName2Id["妖精緑"] = nn::ts::e::common::ENEMY_ID_YOSEIMIDORI;
	this->enemyName2Id["お空"] = nn::ts::e::common::ENEMY_ID_OKUU;

	this->enemyAttackName2Id["マシンガンダウン"] = nn::ts::e::enemy::ATTACK_CATEGORY_MACHINGUN_DOWN;
	this->enemyAttackName2Id["反射レーザー2"] = nn::ts::e::enemy::ATTACK_CATEGORY_LASER_2;
	this->enemyAttackName2Id["反射レーザー4"] = nn::ts::e::enemy::ATTACK_CATEGORY_LASER_4;

}
//---------------------------------------------------------------------------
nn::ts::common::CControl::~CControl() {

	::nn_commonLog( 1, "CControl Delete\n");

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::start() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::startCommon() {
	
	this->saveManager = new nn::ts::common::CTsSaveManager();

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CControl::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	::TOUCH_WORK * touch = &master->touchWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::control::STATE_NONE:
			break;
		case nn::ts::e::control::STATE_TITLE:
			break;
		case nn::ts::e::control::STATE_QUEST:
			if(this->questCtrl->lm.quest->lm.battle->state == nn::ts::e::battle::STATE_GAMEOVER) {
				if(touch->touchPoint & TOUCH_TOUCH) {
					this->questCtrl->resetQuest();
				}
			}
#ifdef _DEBUG
			if(key->trigger & nn::common::e::PAD_START) {
				nn::ts::debug::CDebugMenu * debug_menu = new nn::ts::debug::CDebugMenu( this);
				debug_menu->m.link.font = this->font;
				debug_menu->m.link.commandCBFunc = nn::ts::common::CControl::debugMenuCommandCB;
				debug_menu->m.link.commnadCBFuncArg = this;
				debug_menu->start();
				this->state = nn::ts::e::control::STATE_DEBUG_MENU;
				this->debugMenu = debug_menu;
			}
#endif //_DEBUG
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::disp() {

	switch(this->state) {
		case nn::ts::e::control::STATE_NONE:
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::common::CControl::getClassId() {
	return -1;		// ToDo
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::common::CControl::getDataRecode() {
	return nn::ts::common::CControl::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::dump() {

	time_t timer;
    struct tm *local;
	char file_name[256] = {0};

    timer = time(NULL);
    local = localtime(&timer);

	sprintf_s( file_name, sizeof(file_name), "%04d%02d%02d%02d%02d%02d.txt",
		local->tm_year + 1900,	local->tm_mon + 1,	local->tm_mday,
		local->tm_hour,			local->tm_min,		local->tm_sec);
	
	nn_commonLog( 0, "---------- nn::ts::quest::CControl::dump() start ----------\n");

	nn::common::logInit( nn::ts::e::common::LOG_DST_INDEX_CLASS_DUMP, file_name);
	::TASK_SAVE::setDumpLogIndex( nn::ts::e::common::LOG_DST_INDEX_CLASS_DUMP);

	LOG_CLASS_DUMP( "---------- ::TASK_SAVE::sRoot->dump() start ----------\n");
	::TASK_SAVE::sRoot->dump();
	LOG_CLASS_DUMP( "---------- ::TASK_SAVE::sRoot->dump() end ----------\n");

	this->questCtrl->dump();

	::TASK_SAVE::setDumpLogIndex( -1);
	nn::common::logEnd( nn::ts::e::common::LOG_DST_INDEX_CLASS_DUMP);

	nn_commonLog( 0, "---------- nn::ts::quest::CControl::dump() end   ----------\n");

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::startTitle() {

	nn::ts::title::CTitle * title = new nn::ts::title::CTitle( this, this->saveManager);
	title->lm.ctrl = this;
	title->start();
	this->title = title;

	this->state = nn::ts::e::control::STATE_TITLE;

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::startQuest() {
	
	bool load_flag = false;

#ifdef _DEBUG
	if(this->saveManager->isLoad( nn::ts::common::gCharactersSaveDataDebugQuest)) {
		script->startScript( "resume_select.teco", 0, common_var);
		script->scriptEndCbFunc = nn::ts::common::CControl::resumeSelectEndCB;
		script->scriptEndCbFuncArg = this;
		load_flag = true;
		this->state = nn::ts::e::control::STATE_WAIT;
	}
#endif

	if(load_flag == false) {
		this->startQuestBegining();
	}

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::startQuestResume() {
	
	nn::ts::quest::CQuestControl * quest_ctrl =
		(nn::ts::quest::CQuestControl *)this->saveManager->load(
			nn::ts::common::gCharactersSaveDataDebugQuest, true);
	
	quest_ctrl->lm.ctrl = this;
	quest_ctrl->lm.quest->lm.ctrl = this;
	quest_ctrl->lm.quest->lm.battle->lm.ctrl = this;
	quest_ctrl->lm.quest->setupCamera();
	this->questCtrl = quest_ctrl;
	::TASK_SAVE::addTask( this, quest_ctrl);
	
	this->state = nn::ts::e::control::STATE_QUEST;

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::startQuestBegining() {

	nn::ts::quest::CQuestControl * quest_ctrl =
		new nn::ts::quest::CQuestControl( this, this->saveManager);
	quest_ctrl->lm.ctrl = this;
	quest_ctrl->start();
	this->questCtrl = quest_ctrl;

	this->state = nn::ts::e::control::STATE_QUEST;

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::debugMenuCommandCB( void * argControl, int command) {

	nn::ts::common::CControl * control = (nn::ts::common::CControl *)argControl;

	control->debugMenuCommand( command);

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::debugMenuCommand( int command) {

	switch(command) {
	case nn::ts::e::common::DEBUG_COMMAND_SAVE:
		{
			this->saveManager->save( "quest01.data", false, 0);
			this->endDebugMenu();
		}
		break;
	case nn::ts::e::common::DEBUG_COMMAND_LOAD:
		{
			nn::ts::quest::CQuestControl * quest_ctrl = (nn::ts::quest::CQuestControl *)this->saveManager->load( "quest01.data", false);
			quest_ctrl->lm.ctrl = this;
			this->questCtrl->taskDelete();
			this->questCtrl = quest_ctrl;
			::TASK_SAVE::addTask( this, quest_ctrl);
			this->endDebugMenu();
		}
		break;
	case nn::ts::e::common::DEBUG_COMMAND_UNDO:
		{
			this->gm.dispSkipFlag = true;
			this->gm.autoFlag = true;
			this->gm.autoEndTurn = this->questCtrl->lm.quest->gm.status.turn - 1;
			this->questCtrl->resetQuest();
		}
		break;
	case nn::ts::e::common::DEBUG_COMMAND_REDO:
		{
			if(this->questCtrl->lm.quest->gm.status.turn <= this->questCtrl->gm.log.lastTurn) {
				this->questCtrl->lm.quest->lm.battle->shot(
					this->questCtrl->gm.log.te[this->questCtrl->lm.quest->gm.status.turn].angle
				);
				this->gm.dispSkipFlag = true;
				this->gm.autoFlag = true;
				this->gm.autoEndTurn = this->questCtrl->lm.quest->gm.status.turn + 1;
			}
		}
		break;
	case nn::ts::e::common::DEBUG_COMMAND_BATTLE_NEXT:
		{
			for( int i = 0; i < this->questCtrl->lm.quest->lm.battle->gm.enemyN; i++) {

				nn::ts::quest::CEnemy * enmey = this->questCtrl->lm.quest->lm.battle->gm.enemys[i];
				enmey->gm.status.damageHp = 0;
				enmey->updateIfDead();

			}
			this->endDebugMenu();
		}
		break;
	case nn::ts::e::common::DEBUG_COMMAND_QUEST_RESTART:
		{
			this->questCtrl->resetQuest();
			this->endDebugMenu();
		}
		break;
	case nn::ts::e::common::DEBUG_COMMAND_END:
		{
			this->endDebugMenu();
		}
		break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::endDebugMenu() {
	
	this->debugMenu->taskDelete();
	this->debugMenu = NULL;
	this->state = nn::ts::e::control::STATE_QUEST;

}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::resumeSelectEndCB( void * argControl) {
	
	nn::ts::common::CControl * control = (nn::ts::common::CControl *)argControl;

	control->resumeSelectEnd();
}
//---------------------------------------------------------------------------
void nn::ts::common::CControl::resumeSelectEnd() {

	int select = common_var->ans;

	if(select == 0) {
		// 再開する
		this->startQuestResume();
	}
	else {
		// 最初から
		this->startQuestBegining();
	}

}
//---------------------------------------------------------------------------

