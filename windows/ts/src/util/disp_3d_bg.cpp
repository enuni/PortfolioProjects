// disp_3d_bg を置換
// Disp3dBg を置換
// dxlib を置換
// disp_3d_bg を置換

#include "nn_common.h"
#include "nn_common2.h"

#include "anim.h"

#include "disp_3d_bg.h"
//---------------------------------------------------------------------------
::nn_common::DATARECODE nn::dxlib::disp_3d_bg::CDisp3dBg::dataRecode[] = {
	DATA_LOCAL( nn::dxlib::disp_3d_bg::CDisp3dBg, state),
	DATA_LOCAL( nn::dxlib::disp_3d_bg::CDisp3dBg, m.cm),
	DATA_LOCAL( nn::dxlib::disp_3d_bg::CDisp3dBg, m.lm.directEndCBFuncId),
	DATA_PTR( nn::dxlib::disp_3d_bg::CDisp3dBg, m.lm.directEndCBFuncOwner),
	DATA_BASE(::TASK_SAVE::dataRecode),
	DATA_END,
};
//---------------------------------------------------------------------------
nn::dxlib::disp_3d_bg::CDisp3dBg::CDisp3dBg(TASK_SAVE * oya, nn_common::SaveManager * m) : TASK_SAVE( oya, m) {

	::nn_commonLog( 1, "CDisp3dBg Create(TASK_SAVE * oya, nn_common::SaveManager * m)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::dxlib::disp_3d_bg::CDisp3dBg::CDisp3dBg(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CDisp3dBg Create(TASK_SAVE * oya)\n");
	
	this->constructCommon();

}
//---------------------------------------------------------------------------
nn::dxlib::disp_3d_bg::CDisp3dBg::CDisp3dBg() : TASK_SAVE() {

	::nn_commonLog( 1, "CDisp3dBg Create() %x\n", this);

	this->constructCommon();

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::constructCommon() {
	
	this->state = nn::dxlib::e::disp_3d_bg::STATE_NONE;
	this->oldState = nn::dxlib::e::disp_3d_bg::STATE_NONE;
	this->waitCnt = 0;
	this->frameCnt = 0;

	memset( &this->m, 0, sizeof( this->m));

	strcpy_s( this->name, sizeof( this->name), "CDisp3dBg");
	
}
//---------------------------------------------------------------------------
nn::dxlib::disp_3d_bg::CDisp3dBg::~CDisp3dBg() {

	::nn_commonLog( 1, "CDisp3dBg Delete\n");

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::setMember(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::setMemberLoad(  void * owner) {
	
	this->setMemberCommon( owner);

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::setMemberCommon(  void * owner) {

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::start() {
	
	this->startCommon();

	this->state = nn::dxlib::e::disp_3d_bg::STATE_IDLE;

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::startLoad() {
	
	this->startCommon();

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::startCommon() {

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CDisp3dBg::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;
	
	this->frameCnt++;
	
	switch(this->state) {
		case nn::dxlib::e::disp_3d_bg::STATE_IDLE:
			{
				for( int i = 0; i < this->m.cm.polygonN; i++) {

					this->m.cm.polygons[i].shiftU += this->m.cm.polygons[i].now.varShiftU;
					if(this->m.cm.polygons[i].shiftU < 1.0f) {
						this->m.cm.polygons[i].shiftU += 1.0f;
					}
					if(this->m.cm.polygons[i].shiftU > 1.0f) {
						this->m.cm.polygons[i].shiftV -= 1.0f;
					}
					this->m.cm.polygons[i].shiftV += this->m.cm.polygons[i].now.varShiftV;
					if(this->m.cm.polygons[i].shiftV < 1.0f) {
						this->m.cm.polygons[i].shiftV += 1.0f;
					}
					if(this->m.cm.polygons[i].shiftV > 1.0f) {
						this->m.cm.polygons[i].shiftV -= 1.0f;
					}

				}
			}
			break;
	}
	
	switch(this->state) {
		case nn::dxlib::e::disp_3d_bg::STATE_NONE:
			break;
		case nn::dxlib::e::disp_3d_bg::STATE_IDLE:
			break;
		case nn::dxlib::e::disp_3d_bg::STATE_DIRECT:
			{
				this->m.cm.frame++;
				if(this->m.cm.frame >= this->m.cm.frameN) {
					for( int i = 0; i < this->m.cm.polygonN; i++) {
						
						this->m.cm.polygons[i].now = this->m.cm.polygons[i].target;

					}
					this->m.cm.scene.now = this->m.cm.scene.target;

					this->state = nn::dxlib::e::disp_3d_bg::STATE_IDLE;

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
			}
	}

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::disp() {

	switch(this->state) {
		case nn::dxlib::e::disp_3d_bg::STATE_NONE:
			break;
		case nn::dxlib::e::disp_3d_bg::STATE_IDLE:
		case nn::dxlib::e::disp_3d_bg::STATE_DIRECT:
			{
				nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER scene_var;

				if(this->state == nn::dxlib::e::disp_3d_bg::STATE_DIRECT) {
					scene_var.fogStart			= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.fogStart,			this->m.cm.scene.target.fogStart,			this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					scene_var.fogEnd			= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.fogEnd,				this->m.cm.scene.target.fogEnd,			this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					scene_var.cameraPositionX	= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.cameraPositionX,	this->m.cm.scene.target.cameraPositionX,	this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					scene_var.cameraPositionY	= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.cameraPositionY,	this->m.cm.scene.target.cameraPositionY,	this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					scene_var.cameraPositionZ	= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.cameraPositionZ,	this->m.cm.scene.target.cameraPositionZ,	this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					scene_var.cameraTargetX		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.cameraTargetX,		this->m.cm.scene.target.cameraTargetX,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					scene_var.cameraTargetY		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.cameraTargetY,		this->m.cm.scene.target.cameraTargetY,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					scene_var.cameraTargetZ		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.scene.now.cameraTargetZ,		this->m.cm.scene.target.cameraTargetZ,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
				}
				else {
					scene_var = this->m.cm.scene.now;
				}
				
				SetCameraPositionAndTarget_UpVecY(
					VGet( scene_var.cameraPositionX,	scene_var.cameraPositionY,	scene_var.cameraPositionZ),
					VGet( scene_var.cameraTargetX,		scene_var.cameraTargetY,	scene_var.cameraTargetZ));

				SetFogStartEnd( scene_var.fogStart, scene_var.fogEnd);

				for( int i = 0; i < this->m.cm.polygonN; i++) {

					nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER var;

					if(this->state == nn::dxlib::e::disp_3d_bg::STATE_DIRECT) {
						var.size	= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.size,		this->m.cm.polygons[i].target.size,	this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.y		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.y,		this->m.cm.polygons[i].target.y,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.uvRate	= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.uvRate,	this->m.cm.polygons[i].target.uvRate,	this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.r		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.r,		this->m.cm.polygons[i].target.r,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.g		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.g,		this->m.cm.polygons[i].target.g,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.b		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.b,		this->m.cm.polygons[i].target.b,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.a		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.a,		this->m.cm.polygons[i].target.a,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.u		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.u,		this->m.cm.polygons[i].target.u,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
						var.v		= nn::dxlib::CAnim::utilCalcMove( this->m.cm.polygons[i].now.v,		this->m.cm.polygons[i].target.v,		this->m.cm.acc, this->m.cm.frameN, this->m.cm.frame);
					}
					else {
						var = this->m.cm.polygons[i].now;
					}

					VERTEX3D Vertex[ 4 ] ;
					WORD Index[ 6 ] ;
					// ４頂点分のデータをセット
					Vertex[ 0 ].pos  = VGet( - var.size / 2.0f, var.y, var.size / 2.0f);
					Vertex[ 0 ].norm = VGet(   0.0f,   0.0f, -1.0f );
					Vertex[ 0 ].dif  = GetColorU8( (int)var.r, (int)var.g, (int)var.b, (int)var.a);
					Vertex[ 0 ].spc  = GetColorU8(   0,  0,  0,  0 );
					Vertex[ 0 ].u    = (0.5f - 0.5f * var.uvRate) + var.u + this->m.cm.polygons[i].shiftU;
					Vertex[ 0 ].v    = (0.5f - 0.5f * var.uvRate) + var.v + this->m.cm.polygons[i].shiftV;
					Vertex[ 0 ].su   = 0.0f;
					Vertex[ 0 ].sv   = 0.0f;
		
					Vertex[ 1 ].pos  = VGet( var.size / 2.0f, var.y, var.size / 2.0f);
					Vertex[ 1 ].norm = VGet(   0.0f,   0.0f, -1.0f );
					Vertex[ 1 ].dif  = GetColorU8( (int)var.r, (int)var.g, (int)var.b, (int)var.a);
					Vertex[ 1 ].spc  = GetColorU8(   0,  0,  0,  0 );
					Vertex[ 1 ].u    = (0.5f + 0.5f * var.uvRate) + var.u + this->m.cm.polygons[i].shiftU;
					Vertex[ 1 ].v    = (0.5f - 0.5f * var.uvRate) + var.v + this->m.cm.polygons[i].shiftV;
					Vertex[ 1 ].su   = 0.0f;
					Vertex[ 1 ].sv   = 0.0f;
		
					Vertex[ 2 ].pos  = VGet( - var.size / 2.0f, var.y, - var.size / 2.0f);
					Vertex[ 2 ].norm = VGet(   0.0f,   0.0f, -1.0f );
					Vertex[ 2 ].dif  = GetColorU8( (int)var.r, (int)var.g, (int)var.b, (int)var.a);
					Vertex[ 2 ].spc  = GetColorU8(   0,  0,  0,  0 );
					Vertex[ 2 ].u    = (0.5f - 0.5f * var.uvRate) + var.u + this->m.cm.polygons[i].shiftU;
					Vertex[ 2 ].v    = (0.5f + 0.5f * var.uvRate) + var.v + this->m.cm.polygons[i].shiftV;
					Vertex[ 2 ].su   = 0.0f;
					Vertex[ 2 ].sv   = 0.0f;
		
					Vertex[ 3 ].pos  = VGet( var.size / 2.0f, var.y, - var.size / 2.0f);
					Vertex[ 3 ].norm = VGet(   0.0f,   0.0f, -1.0f );
					Vertex[ 3 ].dif  = GetColorU8( (int)var.r, (int)var.g, (int)var.b, (int)var.a);
					Vertex[ 3 ].spc  = GetColorU8(   0,  0,  0,  0 );
					Vertex[ 3 ].u    = (0.5f + 0.5f * var.uvRate) + var.u + this->m.cm.polygons[i].shiftU;
					Vertex[ 3 ].v    = (0.5f + 0.5f * var.uvRate) + var.v + this->m.cm.polygons[i].shiftV;
					Vertex[ 3 ].su   = 0.0f;
					Vertex[ 3 ].sv   = 0.0f;
		
					// ２ポリゴン分のインデックスデータをセット
					Index[ 0 ] = 0;
					Index[ 1 ] = 1;
					Index[ 2 ] = 2;
					Index[ 3 ] = 3;
					Index[ 4 ] = 2;
					Index[ 5 ] = 1;
		
					// ２ポリゴンの描画
					DrawPolygonIndexed3D( Vertex, 4, Index, 2, this->m.cm.polygons[i].handle, TRUE);

				}
			}
			break;
	}

}
//---------------------------------------------------------------------------
int nn::dxlib::disp_3d_bg::CDisp3dBg::getClassId() {
	return nn_common::e::save_manager::CLASS_ID_COMMON_DISP_3D_BG;
}
//---------------------------------------------------------------------------
nn_common::DATARECODE * nn::dxlib::disp_3d_bg::CDisp3dBg::getDataRecode() {
	return nn::dxlib::disp_3d_bg::CDisp3dBg::dataRecode;
}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::initDirect() {
	this->m.cm.scene.target = this->m.cm.scene.now;
	for( int i = 0; i < this->m.cm.polygonN; i++) {

		this->m.cm.polygons[i].target = this->m.cm.polygons[i].now;

	}
}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::setDirectScene( nn::dxlib::disp_3d_bg::SCENE_VAR_MEMBER * target) {

	this->m.cm.scene.target = *target;

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::setDirectPolygon( int index, nn::dxlib::disp_3d_bg::POLYGON_VAR_MEMBER * target) {

	this->m.cm.polygons[index].target = *target;

}
//---------------------------------------------------------------------------
void nn::dxlib::disp_3d_bg::CDisp3dBg::startDirect( int acc, int frame) {

	this->m.cm.acc = acc;
	this->m.cm.frameN = frame;
	this->m.cm.frame = 0;
	this->state = nn::dxlib::e::disp_3d_bg::STATE_DIRECT;

}
//---------------------------------------------------------------------------

