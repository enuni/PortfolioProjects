// ctrl を置換
// Ctrl を置換
// yb を置換
// main を置換

#include "nn_common.h"

#include "disp_common.h"
#include "image_ctrl.h"
#include "animation.h"
#include "anime.h"
#include "font.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"

#include "animation_data.inc"
#include "animation_data.h"

#include "sound_data.sadl"

#include "yb_common.h"
#include "yb_save_manager.h"
#include "save_data.h"
#include "config.h"

#include "edit.h"
#include "edit_menu.h"
#include "stage.h"
#include "camera.h"
#include "menu.h"

#include "ctrl.h"

#include "effect/effect_ctrl.h"

//---------------------------------------------------------------------------
nn::yb::main::CCtrl::CCtrl(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CCtrl Create\n");
	this->state = nn::yb::e::ctrl::STATE_NONE;
	this->oldState = nn::yb::e::ctrl::STATE_NONE;
	this->waitCnt = 0;

	memset( this->holeData, 0, sizeof( this->holeData));
	memset( this->holeDataSize, 0, sizeof( this->holeDataSize));
	this->holeDataN = 0;

	this->loading_image_id = 0;

	this->dispFps = 0.0;

	this->fullscreenGraph = 0;
	this->displayW = 0;
	this->displayH = 0;
	this->drawDstW = 0;
	this->drawDstH = 0;
	this->drawGlobalW = 0;
	this->drawGlobalH = 0;
}
//---------------------------------------------------------------------------
nn::yb::main::CCtrl::~CCtrl() {

	::nn_commonLog( 1, "CCtrl Delete\n");

	for( int i = 0; i < NN_COMMON_GET_ARRAY_SIZE( this->holeData); i++) {

		if(this->holeData[i] != NULL) {
			free( this->holeData[i]);
			printf( "this->holedata[%d] free\n", i);
		}

	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::start() {
	
	// セーブデータロード
	this->saveManagerEdit = new nn::yb::common::CYbSaveManager();
	this->saveManager = new nn::yb::common::CYbSaveManager();

	strcpy( this->saveDataFileName, "save_data.bin");
	bool save_data_flag = false;
	if(this->saveManager->isLoad( this->saveDataFileName)) {
		char * buf;
		int size;
		size = this->saveManager->open( &buf, this->saveDataFileName);
		if(nn::common::util::checkCrc( (u8 *)buf, size, nn::yb::e::global::CRC_KEY) == 0) {
			nn::yb::main::CSaveData * save_data = (nn::yb::main::CSaveData * )this->saveManager->load( this->saveDataFileName, true);
			// 消しても良いはず
			/*if(save_data->stageOpen == 0) {
				save_data->stageOpen = 1;
			}*/
			this->saveData = save_data;
			save_data_flag = true;
			::nn_commonLog( 1, "セーブデータが正常にロードされました。\n");
		}
		else {
			::nn_commonLog( 1, "セーブデータが壊れています。\n");
		}
	}

	if(!save_data_flag) {
		this->saveDataInit();
	}

	this->saveDataCorrect();

	// フルスクリーン時 アスペクト比固定対応
	this->displayW = GetSystemMetrics( SM_CXSCREEN);
	this->displayH = GetSystemMetrics( SM_CYSCREEN);
#if 1
#if 1
	this->drawGlobalW = 800;
	this->drawGlobalH = 600;
#elif 1
	this->drawGlobalW = 640;
	this->drawGlobalH = 480;
#endif
	if(this->displayW / 4 >= this->displayH / 3) {
		this->drawDstH = this->drawGlobalH;
		this->drawDstW = int(this->drawGlobalW * (double)this->drawGlobalW / this->drawGlobalH / ((double)this->displayW / this->displayH));
	}
	else {
		this->drawDstW = this->drawGlobalW;
		this->drawDstH = int(this->drawGlobalH * (double)this->drawGlobalH / this->drawGlobalW / ((double)this->displayH / this->displayW));
	}
#elif 1
	this->drawGlobalW = this->displayW;
	this->drawGlobalH = this->displayH;
	if(this->drawGlobalW / 4 >= this->drawGlobalH / 3) {
		this->drawDstH = this->displayH;
		this->drawDstW = this->drawDstH * 4 / 3;
	}
	else {
		this->drawDstW = this->displayW;
		this->drawDstH = this->drawDstW * 3 / 4;
	}
#endif

	// セーブデータの反映
	sound_nnsSetVolumeBgm( this->saveData->configData.bgmVol);
	sound_nnsSetVolumeSe( this->saveData->configData.seVol);
	memcpy( this->master->keyWork.configPad, this->saveData->configData.pad, sizeof( this->master->keyWork.configPad));
	if(this->saveData->configData.dispSize == 0) {
		int result = ChangeWindowMode( TRUE);
		if(result >= 0) {
			this->changeWindowSizeAfter();
		}
	}
	else {
		int result = ChangeWindowMode( FALSE);
		if(result >= 0) {
			this->changeWindowSizeAfter();
		}
	}

	// 画像初期設定
	image_ctrlSetTextureData( 0, texture_main_3d_data, palette_3d_main_3d_data, TEXTURE_ID_MAIN_3D_OFFSET, TEXTURE_MAIN_3D_ID_MAX);
	disp_commonSetObjData( 0, obj_main_3d_data, OBJ_ID_MAIN_3D_OFFSET, OBJ_MAIN_3D_ID_MAX);

	animationArcInit( gAnimationArcInfo, ANIMATION_ARC_ID_MAX);
	animationLoadArc( ANIMATION_ARC_ID_COMMON);
	nn::common::dxlib::CAnime::arcInit( gAnimationArcInfo, ANIMATION_ARC_ID_MAX);
	nn::common::dxlib::CAnime::loadArc( ANIMATION_ARC_ID_COMMON);

	disp_commonSetLayoutMain3dData( 0, layout_main_3d_data, LAYOUT_ID_MAIN_3D_OFFSET, LAYOUT_MAIN_3D_ID_MAX);

	/*
	// menu.cppに移行
	image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_TITLE);

	// BGM
	nn::yb::common::yb_commonPlaySeq( BGM_LOGO);
	*/

	// ホールデータのロード
	FILE * fp = fopen( "data.bin", "rb");
	
	fseek( fp, 0L, SEEK_END);
	long data_size = ftell( fp);
	fseek( fp, 0L, SEEK_SET);

	char * temp = (char *)malloc( data_size);
	fread( temp, data_size, 1, fp);
	fclose( fp);

	if(nn::common::util::checkCrc( (unsigned char *)temp, data_size, nn::yb::e::global::CRC_KEY) != 0) {
		master->gameContinueFlag = FALSE;
	}

	char * p = temp;
	this->holeDataN = *(int *)p;
	p += 4;
	char * data_top = temp + 4 + (4 + 4) * this->holeDataN;
	for( int i = 0; i < this->holeDataN; i++) {

		int offset = *(int *)p;
		p += 4;
		this->holeDataSize[i] = *(int *)p;
		p += 4;
		this->holeData[i] = (char *)malloc( this->holeDataSize[i]);
		memcpy( this->holeData[i], data_top + offset, this->holeDataSize[i]);

	}
	free( temp);

	this->camera = new nn::yb::stage::CCamera( this);

	nn::yb::main::CMenu * menu = new nn::yb::main::CMenu( this);
	menu->master = this->master;
	menu->ctrl = this;
	menu->saveData = this->saveData;
	menu->start();
	this->menu = menu;

#if 0
	nn::yb::main::CStage * stage = new nn::yb::main::CStage( NULL);
	stage->master = master;
	stage->start();
#endif

}

//---------------------------------------------------------------------------
int getLoadingTextureID(int r) {
	int tex_id_loading = TEXTURE_ID_BG_LOADING_AYA;
	switch (r)
	{
	case nn::yb::e::ctrl::LOADING_AYA:
		tex_id_loading = TEXTURE_ID_BG_LOADING_AYA;
		break;
	case nn::yb::e::ctrl::LOADING_MARISA:
		tex_id_loading = TEXTURE_ID_BG_LOADING_MARISA;
		break;
	case nn::yb::e::ctrl::LOADING_REIMU:
		tex_id_loading = TEXTURE_ID_BG_LOADING_REIMU;
		break;
	case nn::yb::e::ctrl::LOADING_TENKO:
		tex_id_loading = TEXTURE_ID_BG_LOADING_TENKO;
		break;
	case nn::yb::e::ctrl::LOADING_00:
		tex_id_loading = TEXTURE_ID_BG_LOADING_00;
		break;
	case nn::yb::e::ctrl::LOADING_01:
		tex_id_loading = TEXTURE_ID_BG_LOADING_01;
		break;
	case nn::yb::e::ctrl::LOADING_SANAE:
		tex_id_loading = TEXTURE_ID_BG_LOADING_SANAE;
		break;
	case nn::yb::e::ctrl::LOADING_FLAN:
		tex_id_loading = TEXTURE_ID_BG_LOADING_FLAN;
		break;
	case nn::yb::e::ctrl::LOADING_02:
		tex_id_loading = TEXTURE_ID_BG_LOADING_02;
		break;
	}
	return tex_id_loading;
}

//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::releaseImage( int mode) {

	(void)mode;

	this->releaseLoadingImg();
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_MAIN);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_HOLE);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_DUN_FUMAZIN);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_DUN_SPELL);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_DUN_SPELL_BELT);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_STARBIT);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_SPELL_CHAR);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_MASTERSPARK);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_MASTERSPARK_BASE);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_TITLE);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_KANAMEISHI);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_MENU);

	image_ctrlReleaseMain3dMask( TEXTURE_ID_MASK);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_GAME_CHAR);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_GUNGNIR);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_SPELL_BGCIRCLE_GUNGNIR);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_SPELLNAME);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_BG);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_EXPLOSION);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_COMMON_EFFECT);
	image_ctrlReleaseMain3dImagePaletteHeapVram( TEXTURE_ID_MAIN2);
	disp_commonEndMask();

}
//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::loadImage( int mode) {

	this->loadLoadingImg();
	if(mode == 0) {
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_MAIN);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_HOLE);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_DUN_FUMAZIN);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_DUN_SPELL);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_DUN_SPELL_BELT);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_STARBIT);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_SPELL_CHAR);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_GAME_CHAR);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_GUNGNIR);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_SPELL_BGCIRCLE_GUNGNIR);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_SPELLNAME);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_BG);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_MASTERSPARK);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_MASTERSPARK_BASE);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_TITLE);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_KANAMEISHI);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_MENU);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_EXPLOSION);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_COMMON_EFFECT);
		image_ctrlLoadMain3dImagePaletteHeapVram( TEXTURE_ID_MAIN2);

		if(this->saveData->configData.dispMask == nn::yb::e::config::SET_DISP_MASK_ITEM_ENABLE) {
			disp_commonStartMask();
			image_ctrlLoadMain3dMask( TEXTURE_ID_MASK);
		}
		else {
			disp_commonEndMask();
		}
	}
	else {
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_MAIN);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_HOLE);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_DUN_FUMAZIN);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_DUN_SPELL);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_DUN_SPELL_BELT);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_STARBIT);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_SPELL_CHAR);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_GAME_CHAR);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_GUNGNIR);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_SPELL_BGCIRCLE_GUNGNIR);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_SPELLNAME);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_BG);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_MASTERSPARK);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_MASTERSPARK_BASE);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_TITLE);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_KANAMEISHI);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_MENU);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_EXPLOSION);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_COMMON_EFFECT);
		image_ctrlLoadMain3dImagePaletteHeapVramAsync( TEXTURE_ID_MAIN2);

		if(this->saveData->configData.dispMask == nn::yb::e::config::SET_DISP_MASK_ITEM_ENABLE) {
			disp_commonStartMask();
			image_ctrlLoadMain3dMaskAsync( TEXTURE_ID_MASK);
		}
		else {
			disp_commonEndMask();
		}
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::loadLoadingImg(  ) {

	this->loading_image_id = ::n_commonRandN( nn::yb::e::ctrl::LOADING_MAX);//読み込むロード画像をランダムに決定
	int tex_id_loading = getLoadingTextureID(this->loading_image_id);
	image_ctrlLoadMain3dImagePaletteHeapVram( tex_id_loading );//for test

}

//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::releaseLoadingImg(  ) {

	int tex_id_loading = getLoadingTextureID(this->loading_image_id);
	image_ctrlReleaseMain3dImagePaletteHeapVram( tex_id_loading );

}


//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::changeWindowSizeAfter() {

	// フルスクリーン時 アスペクト比固定対応
	if(this->saveData->configData.dispSize == 1) {
		if(SetGraphMode( this->drawGlobalW, this->drawGlobalH, 32) != DX_CHANGESCREEN_OK) {
	
		}
		this->fullscreenGraph = MakeGraph( nn::yb::e::global::WINDOW_W, nn::yb::e::global::WINDOW_H);
	}
	else {
		if(this->state == nn::yb::e::ctrl::STATE_EDIT) {
			// エディット時は、ここではウィンドウサイズを変えない
		}
		else {
			if(SetGraphMode( nn::yb::e::global::WINDOW_W, nn::yb::e::global::WINDOW_H, 32) != DX_CHANGESCREEN_OK) {
		
			}
		}
	}

	if(SetTransColor( 255, 0, 255) == -1) {		// 抜き色を設定
		
	}
	
	SetDrawScreen( DX_SCREEN_BACK);		// 描画先を裏画面に設定

	fontInitFont( NULL, NULL);
	::fontSetTypeSimple( nn::yb::e::global::FONT_TYPE_SIZE_10, "MS ゴシック", 10);
	::fontSetTypeSimple( nn::yb::e::global::FONT_TYPE_SIZE_18, "MS ゴシック", 18);

}

//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::initializeEffectCtrl() {
	//nn::yb::main::CEffectCtrl::uninitialize();//終了処理
	int draw_w = nn::yb::e::global::WINDOW_W;//ウィンドウモード
	int draw_h = nn::yb::e::global::WINDOW_H;
	if (this->saveData->configData.dispSize == 1)//フルスクリーン
	{
		draw_w = this->drawGlobalW;
		draw_h = this->drawGlobalH;
	}
	nn::yb::main::CEffectCtrl::initialize(draw_w ,draw_h);//初期化
}

//---------------------------------------------------------------------------
// フルスクリーン時 アスペクト比固定対応
void nn::yb::main::CCtrl::fullscreenTask() {

	bool fullscreen_flag = false;
	if(this->menu->state == nn::yb::e::menu::STATE_CONFIG) {
		if(this->menu->config->oldConfig.dispSize == 1) {
			fullscreen_flag = true;
		}
	}
	else {
		if(this->saveData->configData.dispSize == 1) {
			fullscreen_flag = true;
		}
	}

	if(fullscreen_flag) {
		GetDrawScreenGraph(0, 0, nn::yb::e::global::WINDOW_W, nn::yb::e::global::WINDOW_H, this->fullscreenGraph);
		ClearDrawScreen();
		SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
		DrawExtendGraph( (this->drawGlobalW - this->drawDstW) / 2, (this->drawGlobalH - this->drawDstH) / 2,
			(this->drawGlobalW - this->drawDstW) / 2 + this->drawDstW, (this->drawGlobalH - this->drawDstH) / 2 + this->drawDstH,
			this->fullscreenGraph, FALSE
		);
	}

}
//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::changeWindowSize(int w, int h) {

	this->releaseImage( 0);
	fontDestructFont();
	nn::yb::main::CEffectCtrl::uninitialize();

	disp_commonSetCameraSize( w, h);
	if(SetGraphMode( w,  h, 32) != DX_CHANGESCREEN_OK) {
		printf( "SetGraphMode error\n");
	}

	this->changeWindowSizeAfter();
	this->loadImage( 0);

	this->initializeEffectCtrl();
}
//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::task() {

	if(this->state != this->oldState) {
		::nn_commonLog( 999, "CCtrl::task() change state = %d\n", this->state);
		this->oldState = this->state;
	}

	::KEY_WORK * key = &this->master->keyWork;

	switch(this->state) {
		case nn::yb::e::ctrl::STATE_EDIT:
			break;
	}

	this->disp();

}
//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::disp() {

	LAYOUT * layout_fps = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_FPS);
	LAYOUT * layout_fps_num = disp_commonGetLayoutMain3d( LAYOUT_ID_MAIN_FPS_NUM);
	::disp_commonObjFps( this->dispFps, layout_fps_num->obj_id, layout_fps->obj_id, layout_fps->x, layout_fps->y, layout_fps->z, -layout_fps_num->w);	

}
//---------------------------------------------------------------------------
void nn::yb::main::CCtrl::saveMain() {

	this->saveManager->save( this->saveDataFileName, true, nn::yb::e::global::CRC_KEY);

}
//---------------------------------------------------------------------------
// セーブデータの初期化 セーブデータは全て0でnewされるので最初から0以外である必要のあるもののみ設定
void nn::yb::main::CCtrl::saveDataInit() {

	nn::yb::main::CSaveData * save_data = new nn::yb::main::CSaveData( this->saveManager);
	save_data->stageOpen = 1;
	save_data->configData.bgmVol = 63;
	save_data->configData.seVol = 63;
	save_data->configData.pad[0] = 1;
	save_data->configData.pad[1] = 2;
	save_data->configData.pad[2] = 3;
	save_data->configData.pad[3] = 4;
	save_data->configData.pad[4] = 5;
	save_data->configData.pad[5] = 6;
	save_data->configData.pad[6] = 7;
	save_data->configData.pad[7] = 8;
	save_data->configData.pad[8] = 9;
	save_data->configData.pad[9] = 10;
	save_data->configData.pad[10] = 11;
	save_data->configData.pad[11] = 12;
	save_data->configData.pad[12] = 13;
	save_data->configData.pad[13] = 14;
	save_data->stageOpenStandard = 1;
	save_data->gameStageModeSelect = 1;

	this->saveData = save_data;
	this->saveMain();

}
//---------------------------------------------------------------------------
// セーブデータのバージョンアップ対応
void nn::yb::main::CCtrl::saveDataCorrect() {

	nn::yb::main::CSaveData * save_data = this->saveData;

	bool version_up_flag = false;

	// VersionUpサンプル
	/*nn_commonLog( 1, "version = %d\n", save_data->version);
	if(save_data->version == 0) {
		nn_commonLog( 1, "version = 1 にアップデート\n");
		
		save_data->version = 1;
		version_up_flag = true;
	}
	if(save_data->version == 1) {
		
	}
	*/
	if(save_data->version == 0) {
		nn_commonLog( 1, "version = 1 にアップデート\n");
		save_data->version = 1;
		save_data->breakData2.spell = 0;
		version_up_flag = true;
	}
	if(save_data->version == 1) {
		nn_commonLog( 1, "version = 2 にアップデート\n");
		save_data->version = 2;
		save_data->editSpell = 0;
		memset( &save_data->scoreStandard, 0, sizeof( nn::yb::main::SCORE));
		save_data->stageOpenStandard = 1;
		save_data->gameStageModeSelect = 1;
		memset( save_data->gameStageSelect, 0, sizeof( save_data->gameStageSelect));
		save_data->breakData3.stageMode = 0;
		version_up_flag = true;
	}

	if(version_up_flag) {
		this->saveMain();
	}

#ifdef _DEBUG
#if 0
	for( int i = 0; i < 6; i++) {

		save_data->score.bestScore[i].total = 24;
		for( int j = 0; j < 8; j++) {

			save_data->score.bestScore[i].score[j] = 3;

		}

	}
	this->saveMain();
#endif
#endif //_DEBUG

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
int getLoadingObjID(int r) {
	int tex_id_loading = OBJ_ID_LOAD_AYA_ITEM0_0;
	switch (r)
	{
	case nn::yb::e::ctrl::LOADING_AYA:
		tex_id_loading = OBJ_ID_LOAD_AYA_ITEM0_0;
		break;
	case nn::yb::e::ctrl::LOADING_MARISA:
		tex_id_loading = OBJ_ID_LOAD_MARISA_ITEM0_0;
		break;
	case nn::yb::e::ctrl::LOADING_REIMU:
		tex_id_loading = OBJ_ID_LOAD_REIMU_ITEM0_0;
		break;
	case nn::yb::e::ctrl::LOADING_TENKO:
		tex_id_loading = OBJ_ID_LOAD_TENKO_ITEM0_0;
		break;
	case nn::yb::e::ctrl::LOADING_00:
		tex_id_loading = OBJ_ID_LOAD_00_REIMU_0;
		break;
	case nn::yb::e::ctrl::LOADING_01:
		tex_id_loading = OBJ_ID_LOAD_01_REMI_FLAN_0;
		break;
	case nn::yb::e::ctrl::LOADING_SANAE:
		tex_id_loading = OBJ_ID_LOAD_SANAE_ITEM0_0;
		break;
	case nn::yb::e::ctrl::LOADING_FLAN:
		tex_id_loading = OBJ_ID_LOAD_FLAN_ITEM0_0;
		break;
	case nn::yb::e::ctrl::LOADING_02:
		tex_id_loading = OBJ_ID_LOAD_02_REMI_FLAN_0;
		break;
	}
	return tex_id_loading;
}

//---------------------------------------------------------------------------
// ローディング画面描画
void nn::yb::main::CCtrl::dispLoadingScreen(int& frameCnt, const nn::yb::e::ctrl::LOADING_SCREEN_PARAMS& params) {

	::disp_commonObjDxCenterFull( getLoadingObjID( this->loading_image_id) //ランダムで画像選択
		, 320, 240, nn::yb::e::global::DISP_Z_NOW_LOADING_IMAGE, 640, 480, 0, FALSE, FALSE, RGB_WHITE, 31, DISP_COMMON_BLEND_MODE_ALPHA);

	//文字背景
	::disp_commonMain3dBoard( params.window_x, params.window_y, nn::yb::e::global::DISP_Z_NOW_LOADING_ANIME+1, params.window_w, params.window_h, params.window_color, params.window_a);
 
	fontPutsType( params.text_x - 0, params.text_y - 0, params.text_size, RGB_BLACK, params.text); 
	fontPutsType( params.text_x - 0, params.text_y + 2, params.text_size, RGB_BLACK, params.text);
	fontPutsType( params.text_x + 2, params.text_y - 0, params.text_size, RGB_BLACK, params.text);
	fontPutsType( params.text_x + 2, params.text_y + 2, params.text_size, RGB_BLACK, params.text);
	fontPutsType( params.text_x + 1, params.text_y + 1, params.text_size, params.text_color, params.text);

	// loading status bar 
	const int BAROBJ_NUM = 15;
	for (int i = 0; i < BAROBJ_NUM; ++i)
	{
		int w = (int)params.bar_size;
		int h = (int)params.bar_size;
		int offset = (int)1;
		int x = (int)(w * i + offset * i) + params.bar_x;
		int y = (int)params.bar_y;
		int z = (int)nn::yb::e::global::DISP_Z_NOW_LOADING_ANIME;

		int b = (int)DISP_COMMON_BLEND_MODE_ALPHA;
		////int alpha = i/20.0 * 32;

		int alpha = 31;
		int alphaScale = 10;//デフォルトの透過率

		////int currPeak = this->frameCnt % 20;
		int currPeak = (frameCnt % (BAROBJ_NUM * 2) ) / 2;//2フレームに一回、アニメーションを更新

		if (
			i == ( currPeak ) % BAROBJ_NUM
			|| i == (currPeak + 1) % BAROBJ_NUM
			)
		{
			alphaScale = 90;
		}
		else if (
			i ==( currPeak - 1 ) % BAROBJ_NUM
			)
		{
			alphaScale = 60;
		}
		else if (
			i == ( currPeak - 2 ) % BAROBJ_NUM
			)
		{
			alphaScale = 30;
		}
		else if (
			i == ( currPeak + 2 ) % BAROBJ_NUM
			)
		{
			alphaScale = 80;
		}
		else if (
			i == ( currPeak + 3 ) % BAROBJ_NUM
			)
		{
			alphaScale = 50;
		}
		alpha = (int)(32 * alphaScale / 100.0);

		::disp_commonMain3dBoard( x, y, z, w, h, params.bar_color, alpha);
	}

}
//---------------------------------------------------------------------------

