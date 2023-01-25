// battle_bg を置換
// BattleBg を置換
// ts を置換
// quest を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "ts_common.h"
#include "disp_3d_bg.h"
#include "ts_save_manager.h"

#include "battle_bg.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::ts::quest::CBattleBg::dataRecode[] = {
	DATA_LOCAL( nn::ts::quest::CBattleBg, state),
	DATA_LOCAL( nn::ts::quest::CBattleBg, m.cm),
	DATA_LOCAL( nn::ts::quest::CBattleBg, m.lm.directEndCBFuncId),
	DATA_PTR( nn::ts::quest::CBattleBg, m.lm.directEndCBFuncOwner),
	DATA_PTR( nn::ts::quest::CBattleBg, m.lm.disp3dBg),
	DATA_BASE(::TASK_SAVE::dataRecode),
	DATA_END,
};
//---------------------------------------------------------------------------
nn::ts::quest::CBattleBg::CBattleBg(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CBattleBg Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBattleBg::CBattleBg(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CBattleBg Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::ts::quest::CBattleBg::CBattleBg() : TASK_SAVE() {

	::nn_commonLog( 1, "CBattleBg Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::constructCommon() {
	
	this->state = nn::ts::e::battle_bg::STATE_NONE;
	this->oldState = nn::ts::e::battle_bg::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;
	
	memset( &this->m, 0, sizeof( this->m));

	strcpy_s( this->name, sizeof( this->name), "CBattleBg");
	
}
//---------------------------------------------------------------------------
nn::ts::quest::CBattleBg::~CBattleBg() {

	::nn_commonLog( 1, "CBattleBg Delete\n");

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::start() {
	
	this->startCommon();
	
	nn::dxlib::disp_3d_bg::CDisp3dBg * disp_3d_bg = new nn::dxlib::disp_3d_bg::CDisp3dBg( this, this->saveManager);

	disp_3d_bg->m.cm.scene.now = this->m.cm.initScene;

	for( int i = 0; i < this->m.cm.initPolygonN; i++) {

		disp_3d_bg->m.cm.polygons[i].handle = this->m.cm.initPolygonHandles[i];
		disp_3d_bg->m.cm.polygons[i].now = this->m.cm.initPolygons[i];

	}
	disp_3d_bg->m.cm.polygonN = this->m.cm.initPolygonN;
	
	disp_3d_bg->m.lm.directEndCBFuncId = nn::ts::e::common::CB_FUNC_ID_NEXT_DIRECT_BATTLE_BG;
	disp_3d_bg->m.lm.directEndCBFuncOwner = this;

	disp_3d_bg->start();

	this->m.lm.disp3dBg = disp_3d_bg;

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::startCommon() {

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CBattleBg::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::ts::e::battle_bg::STATE_NONE:
			{
				nn::dxlib::disp_3d_bg::CDisp3dBg * disp_3d_bg = this->m.lm.disp3dBg;
				if((key->trigger & nn::common::e::PAD_UP) ||
					(key->pulse & nn::common::e::PAD_UP))
				{
					disp_3d_bg->m.cm.scene.now.cameraPositionZ += 1.0f;
				}
				else if((key->trigger & nn::common::e::PAD_DOWN) ||
					(key->pulse & nn::common::e::PAD_DOWN))
				{
					disp_3d_bg->m.cm.scene.now.cameraPositionZ -= 1.0f;
				}
#ifdef _DEBUG
				printfDx( "size = %f\n", disp_3d_bg->m.cm.polygons[0].now.size);
				printfDx( "uv_rate = %f\n", disp_3d_bg->m.cm.polygons[0].now.uvRate);
				printfDx( "fog_start = %f\n", disp_3d_bg->m.cm.scene.now.fogStart);
				printfDx( "fog_end = %f\n", disp_3d_bg->m.cm.scene.now.fogEnd);
#endif // _DEBUG
			}
			break;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::disp() {

	switch(this->state) {
		case nn::ts::e::battle_bg::STATE_NONE:
			break;
	}

}
//---------------------------------------------------------------------------
int nn::ts::quest::CBattleBg::getClassId() {
	return nn::ts::e::ts_save_manager::CLASS_ID_BATTLE_BG;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::ts::quest::CBattleBg::getDataRecode() {
	return nn::ts::quest::CBattleBg::dataRecode;
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::nextDirectCB( void * argBattleBg) {

	 nn::ts::quest::CBattleBg * battle_bg = (nn::ts::quest::CBattleBg *)argBattleBg;

	 battle_bg->nextDirect();

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::nextDirect() {

	if(this->m.cm.dmIndex >= this->m.cm.dmN) {
		if(this->m.lm.directEndCBFuncId != 0) {
			void (* func)( void *) = (void (*)( void *))nn::common::util::CCallBackManager::get( this->m.lm.directEndCBFuncId);
			if(func != NULL) {
				func( this->m.lm.directEndCBFuncOwner);
			}
		}
		if(this->m.lm.directEndCBFunc != NULL) {
			this->m.lm.directEndCBFunc( this->m.lm.directEndCBFuncArg);
		}
	}
	else {
		nn::dxlib::disp_3d_bg::CDisp3dBg * disp_3d_bg = this->m.lm.disp3dBg;
		disp_3d_bg->initDirect();
		nn::ts::quest::BATTLE_BG_DIRECT_MEMBER * dm = &this->m.cm.dm[this->m.cm.dmIndex];
		if(dm->sceneFlag) {
			disp_3d_bg->setDirectScene( &dm->scene);
		}
		for( int i = 0; i < dm->polygonN; i++) {

			disp_3d_bg->setDirectPolygon( dm->polygonIndexTbl[i], &dm->polygons[i]);

		}
		disp_3d_bg->startDirect( dm->acc, dm->frame);
		this->m.cm.dmIndex++;
	}

}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::startDirect() {

	int i = 0;
	
	if(this->m.cm.battleIndex == 0) {
		this->m.cm.dm[i].sceneFlag			= false;
		this->m.cm.dm[i].polygonN			= 1;
		this->m.cm.dm[i].polygons[0]		= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v		= -0.005f * 40;
		this->m.cm.dm[i].polygonIndexTbl[0] = 0;
		this->m.cm.dm[i].frame				= 40;
		this->m.cm.dm[i].acc				= 66;
		i++;
	
		this->m.cm.dm[i].sceneFlag			= false;
		this->m.cm.dm[i].polygonN			= 1;
		this->m.cm.dm[i].polygons[0]		= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v		= this->m.cm.dm[i - 1].polygons[0].v + -0.04f * 45;
		this->m.cm.dm[i].polygonIndexTbl[0]	= 0;
		this->m.cm.dm[i].frame				= 45;
		this->m.cm.dm[i].acc				= 33;
		i++;

		this->m.cm.dm[i].sceneFlag				= true;
		this->m.cm.dm[i].scene					= this->m.cm.initScene;
		this->m.cm.dm[i].scene.cameraPositionY	= 200.0f;
		this->m.cm.dm[i].scene.cameraPositionZ	= -100.0f;
		this->m.cm.dm[i].polygons[0]			= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v			= this->m.cm.dm[i - 1].polygons[0].v + -0.04f * 90;
		this->m.cm.dm[i].polygonIndexTbl[0]		= 0;
		this->m.cm.dm[i].polygons[1]			= this->m.cm.initPolygons[1];
		this->m.cm.dm[i].polygons[1].a			= 255.0f;
		this->m.cm.dm[i].polygonIndexTbl[1]		= 1;
		this->m.cm.dm[i].polygonN				= 2;
		this->m.cm.dm[i].frame					= 90;
		this->m.cm.dm[i].acc					= 33;
		i++;

		this->m.cm.dm[i].sceneFlag			= false;
		this->m.cm.dm[i].polygonN			= 1;
		this->m.cm.dm[i].polygons[0]		= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v		= this->m.cm.dm[i - 1].polygons[0].v + -0.01f * 30;
		this->m.cm.dm[i].polygonIndexTbl[0]	= 0;
		this->m.cm.dm[i].frame				= 30;
		this->m.cm.dm[i].acc				= 33;
		i++;

	}
	else if(this->m.cm.battleIndex == 1) {
		// 減速
		this->m.cm.dm[i].sceneFlag			= false;
		this->m.cm.dm[i].polygonN			= 1;
		this->m.cm.dm[i].polygons[0]		= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v		= this->m.lm.disp3dBg->m.cm.polygons[0].now.v + -0.005f * 40;
		this->m.cm.dm[i].polygonIndexTbl[0] = 0;
		this->m.cm.dm[i].frame				= 40;
		this->m.cm.dm[i].acc				= 66;
		i++;

		// 水平飛行
		this->m.cm.dm[i].sceneFlag			= false;
		this->m.cm.dm[i].polygonN			= 1;
		this->m.cm.dm[i].polygons[0]		= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v		= this->m.cm.dm[i - 1].polygons[0].v + -0.04f * 45;
		this->m.cm.dm[i].polygonIndexTbl[0]	= 0;
		this->m.cm.dm[i].frame				= 45;
		this->m.cm.dm[i].acc				= 33;
		i++;

		// 上昇
		this->m.cm.dm[i].sceneFlag				= true;
		this->m.cm.dm[i].scene					= this->m.lm.disp3dBg->m.cm.scene.now;
		this->m.cm.dm[i].scene.cameraPositionY	= 400.0f;
		this->m.cm.dm[i].polygons[0]			= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v			= this->m.cm.dm[i - 1].polygons[0].v + -0.04f * 90;
		this->m.cm.dm[i].polygonIndexTbl[0]		= 0;
		this->m.cm.dm[i].polygons[1]			= this->m.cm.initPolygons[2];
		this->m.cm.dm[i].polygons[1].a			= 255.0f;
		this->m.cm.dm[i].polygonIndexTbl[1]		= 2;
		this->m.cm.dm[i].polygonN				= 2;
		this->m.cm.dm[i].frame					= 90;
		this->m.cm.dm[i].acc					= 33;
		i++;

		// 通常飛行
		this->m.cm.dm[i].sceneFlag			= false;
		this->m.cm.dm[i].polygonN			= 1;
		this->m.cm.dm[i].polygons[0]		= this->m.cm.initPolygons[0];
		this->m.cm.dm[i].polygons[0].v		= this->m.cm.dm[i - 1].polygons[0].v + -0.01f * 30;
		this->m.cm.dm[i].polygonIndexTbl[0]	= 0;
		this->m.cm.dm[i].frame				= 30;
		this->m.cm.dm[i].acc				= 33;
		i++;
	}

	this->m.cm.dmN = i;
	this->m.cm.dmIndex = 0;
	this->m.cm.battleIndex++;
	
	this->nextDirect();
}
//---------------------------------------------------------------------------
void nn::ts::quest::CBattleBg::dump() {

	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBattleBg::dump() start ----------\n");
	LOG_CLASS_DUMP( "this = 0x%08x\n", this);
	LOG_CLASS_DUMP( "this->state = %d\n", this->state);
	LOG_CLASS_DUMP( "this->oldState = %d\n", this->oldState);
	LOG_CLASS_DUMP( "this->waitCnt = %d\n", this->waitCnt);
	LOG_CLASS_DUMP( "this->frameCnt = %u\n", this->frameCnt);
	LOG_CLASS_DUMP( "this->m.cm.dmN = %d\n", this->m.cm.dmN);
	for( int i = 0; i < this->m.cm.dmN; i++) {

		nn::ts::quest::BATTLE_BG_DIRECT_MEMBER * dm = &this->m.cm.dm[i];
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.fogStart = %f\n", i, dm->scene.fogStart);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.fogEnd = %f\n", i, dm->scene.fogEnd);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.cameraPositionX = %f\n", i, dm->scene.cameraPositionX);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.cameraPositionY = %f\n", i, dm->scene.cameraPositionY);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.cameraPositionZ = %f\n", i, dm->scene.cameraPositionZ);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.cameraTargetX = %f\n", i, dm->scene.cameraTargetX);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.cameraTargetY = %f\n", i, dm->scene.cameraTargetY);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].scene.cameraTargetZ = %f\n", i, dm->scene.cameraTargetZ);

		for( int j = 0; j < nn::dxlib::e::disp_3d_bg::POLYGON_MAX; j++) {
			
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].size = %f\n", i, j, dm->polygons[j].size);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].r = %f\n", i, j, dm->polygons[j].r);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].g = %f\n", i, j, dm->polygons[j].g);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].b = %f\n", i, j, dm->polygons[j].b);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].a = %f\n", i, j, dm->polygons[j].a);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].y = %f\n", i, j, dm->polygons[j].y);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].u = %f\n", i, j, dm->polygons[j].u);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].v = %f\n", i, j, dm->polygons[j].v);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].varShiftU = %f\n", i, j, dm->polygons[j].varShiftU);
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygons[%d].varShiftV = %f\n", i, j, dm->polygons[j].varShiftV);

		}
		
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygonN = %f\n", dm->polygonN);
		for( int j = 0; j < dm->polygonN; j++) {
			
			LOG_CLASS_DUMP( "this->m.cm.dm[%d].polygonIndexTbl[%d] = %d\n", i, j, dm->polygonIndexTbl[j]);

		}
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].dm->sceneFlag = %d\n", i, dm->sceneFlag);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].dm->frame = %d\n", i, dm->frame);
		LOG_CLASS_DUMP( "this->m.cm.dm[%d].dm->acc = %d\n", i, dm->acc);

	}

	LOG_CLASS_DUMP( "this->m.cm.dmIndex = %d\n", this->m.cm.dmIndex);
	LOG_CLASS_DUMP( "this->m.cm.initScene.fogStart = %f\n", this->m.cm.initScene.fogStart);
	LOG_CLASS_DUMP( "this->m.cm.initScene.fogEnd = %f\n", this->m.cm.initScene.fogEnd);
	LOG_CLASS_DUMP( "this->m.cm.initScene.cameraPositionX = %f\n", this->m.cm.initScene.cameraPositionX);
	LOG_CLASS_DUMP( "this->m.cm.initScene.cameraPositionY = %f\n", this->m.cm.initScene.cameraPositionY);
	LOG_CLASS_DUMP( "this->m.cm.initScene.cameraPositionZ = %f\n", this->m.cm.initScene.cameraPositionZ);
	LOG_CLASS_DUMP( "this->m.cm.initScene.cameraTargetX = %f\n", this->m.cm.initScene.cameraTargetX);
	LOG_CLASS_DUMP( "this->m.cm.initScene.cameraTargetY = %f\n", this->m.cm.initScene.cameraTargetY);
	LOG_CLASS_DUMP( "this->m.cm.initScene.cameraTargetZ = %f\n", this->m.cm.initScene.cameraTargetZ);

	LOG_CLASS_DUMP( "this->m.cm.initPolygonN = %d\n", this->m.cm.initPolygonN);
	for( int i = 0; i < this->m.cm.initPolygonN; i++) {
		
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].size = %f\n", i, this->m.cm.initPolygons[i].size);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].uvRate = %f\n", i, this->m.cm.initPolygons[i].uvRate);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].r = %f\n", i, this->m.cm.initPolygons[i].r);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].g = %f\n", i, this->m.cm.initPolygons[i].g);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].b = %f\n", i, this->m.cm.initPolygons[i].b);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].a = %f\n", i, this->m.cm.initPolygons[i].a);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].y = %f\n", i, this->m.cm.initPolygons[i].y);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].u = %f\n", i, this->m.cm.initPolygons[i].u);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].v = %f\n", i, this->m.cm.initPolygons[i].v);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].varShiftU = %f\n", i, this->m.cm.initPolygons[i].varShiftU);
		LOG_CLASS_DUMP( "this->m.cm.initPolygons[%d].varShiftV = %f\n", i, this->m.cm.initPolygons[i].varShiftV);
		LOG_CLASS_DUMP( "this->m.cm.initPolygonHandles[%d] = %d\n", i, this->m.cm.initPolygonHandles[i]);

	}
	LOG_CLASS_DUMP( "this->m.cm.battleIndex = %d\n", this->m.cm.battleIndex);

	LOG_CLASS_DUMP( "---------- nn::ts::quest::CBattleBg::dump() end   ----------\n");

}
//---------------------------------------------------------------------------

