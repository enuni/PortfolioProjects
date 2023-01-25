// ctrl ��u��
// Ctrl ��u��
// yb ��u��
// main ��u��

#include "nn_common.h"

#include "disp_common.h"
#include "image_ctrl.h"

#include "obj_main_3d_data.h"
#include "layout_main_3d_data.h"

#include "dxtexture_loader.h"

#include "effect_ctrl.h"

#include <Effekseer.h>
#include "enum.h"

#include "font.h"
#include "fs.h"
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib" )
#pragma comment(lib, "d3dx9.lib" )


#if _DEBUG
#pragma comment(lib, "src\\effect\\Effekseer\\lib\\Effekseer.Debug.lib" )
#pragma comment(lib, "src\\effect\\Effekseer\\lib\\EffekseerRendererDX9.Debug.lib" )
#else
#pragma comment(lib, "src\\effect\\Effekseer\\lib\\Effekseer.Release.lib" )
#pragma comment(lib, "src\\effect\\Effekseer\\lib\\EffekseerRendererDX9.Release.lib" )
#endif

//static
  ::Effekseer::Manager*			nn::yb::main::CEffectCtrl::m_manager = NULL;
  ::EffekseerRenderer::Renderer*	nn::yb::main::CEffectCtrl::m_renderer = NULL;
  ::Effekseer::Effect*				nn::yb::main::CEffectCtrl::m_effect  = NULL;
  ::Effekseer::Handle				nn::yb::main::CEffectCtrl::m_handle = 0;
  int grHandle = 0;
  
  float				nn::yb::main::CEffectCtrl::m_posX = 0;
  float				nn::yb::main::CEffectCtrl::m_posY = 0;
  float				nn::yb::main::CEffectCtrl::m_camX = 0;
  float				nn::yb::main::CEffectCtrl::m_camY = 0;

  int				nn::yb::main::CEffectCtrl::m_effect3D_z = nn::yb::e::global::DISP_Z_SPELL_BALL - 1;

  int nn::yb::main::CEffectCtrl::m_displayW = nn::yb::e::global::WINDOW_W;//�E�B���h�E�T�C�Y
  int nn::yb::main::CEffectCtrl::m_displayH = nn::yb::e::global::WINDOW_H;//�E�B���h�E�T�C�Y

//---------------------------------------------------------------------------
nn::yb::main::CEffectCtrl::CEffectCtrl(TASK_SAVE * oya) : TASK_SAVE( oya) {

	::nn_commonLog( 1, "CEffectCtrl Create\n");

	memset( this->eventCallBackFuncStack, 0, sizeof( this->eventCallBackFuncStack));
	memset( this->eventCallBackArg1Stack, 0, sizeof( this->eventCallBackArg1Stack));
	memset( this->eventCallBackArg2Stack, 0, sizeof( this->eventCallBackArg2Stack));
	this->eventCallBackStackIndex = 0;


	memset( this->holeEventCallBackFuncStack, 0, sizeof( this->holeEventCallBackFuncStack));
	memset( this->holeEventCallBackArg1Stack, 0, sizeof( this->holeEventCallBackArg1Stack));
	memset( this->holeEventCallBackArg2Stack, 0, sizeof( this->holeEventCallBackArg2Stack));
	this->holeEventCallBackStackIndex = 0;
}
//---------------------------------------------------------------------------
nn::yb::main::CEffectCtrl::~CEffectCtrl() {
	::nn_commonLog( 1, "CEffectCtrl Delete\n");


}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::start() {


}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::task() {

	this->disp();

}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::disp() {
	
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::initialize(int displayW, int displayH) {
	
	//initialize
	m_manager = NULL;
	m_renderer = NULL;
	m_effect = NULL;
	m_handle = 0;

	m_displayW = displayW;
	m_displayH = displayH;
	
	::nn_commonLog(1,  "m_displayW = %d \n", m_displayW);
	::nn_commonLog(1,  "m_displayH = %d \n", m_displayH);

	// �`��p�C���X�^���X�̐���
	m_renderer = ::EffekseerRenderer::Renderer::Create( (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9(), 2000 );
	
	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	m_manager = ::Effekseer::Manager::Create( 2000 );

	// �`��p�C���X�^���X����`��@�\��ݒ�
	m_manager->SetSpriteRenderer( m_renderer->CreateSpriteRenderer() );
	m_manager->SetRibbonRenderer( m_renderer->CreateRibbonRenderer() );

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	////m_manager->SetTextureLoader( m_renderer->CreateTextureLoader() );

	// �Ǝ��g���ݒ肷��B
	m_manager->SetTextureLoader( new CDxTextureLoader(m_renderer) );
	//���[�g�p�X�̐ݒ�
	////setRootPath(L"data/graphics/image/bmp_main/effect/" );
	setRootPath(L"../data/graphics/image/bmp_main/effect/" );


	// ���e�s���ݒ�
	m_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovRH( 90.0f / 180.0f * 3.14f, (float)m_displayW / (float)m_displayH, 1.0f, 50.0f ) );

	// �J�����s���ݒ�
	m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH( ::Effekseer::Vector3D( 3.0f, 20.0f, 15.0f ), ::Effekseer::Vector3D( 0.0f, 0.0f, 0.0f ), ::Effekseer::Vector3D( 0.0f, 1.0f, 0.0f ) ) );
	
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::uninitialize() {
	
	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE( m_effect );

	// �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
	if (m_manager != NULL)
	{
		m_manager->Destroy();
		m_manager = NULL;
	}

	// �`��p�C���X�^���X��j��
	if (m_renderer != NULL)
	{
		m_renderer->Destory();
		m_renderer = NULL;
	}
	m_handle = 0;
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setRootPath(const WCHAR* path) {
	((CDxTextureLoader*)m_manager->GetTextureLoader())->RootPath = std::wstring( path );
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::loadEffect(const WCHAR* path) {
	// �G�t�F�N�g�̓Ǎ� 
 
	std::wstring dstPath = std::wstring();
	dstPath.append(std::wstring((const wchar_t *)path));

	char *mbs = new char[dstPath.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, dstPath.c_str(), dstPath.length() * MB_CUR_MAX + 1);
	////std::string dest = mbs;

	// �t�@�C���̃T�C�Y�𓾂�
	int FileSize = fsGetFileSize( mbs ) ; 

	if (FileSize > 0) 
	{
		// �f�[�^�i�[�p�̃������̈���m��
		void *Buffer = malloc( FileSize ) ;
		// �t�@�C�����ۂ��ƃ������ɓǂݍ��� 
		BOOL bRes = fsLoadFile( Buffer, mbs, FileSize );

		if (bRes)
		{
			m_effect = Effekseer::Effect::Create( m_manager, Buffer, FileSize );
		}

		delete [] Buffer;
	} 
	delete [] mbs;
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::unloadEffect() {
	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE( m_effect );
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::playEffect() {
	// �G�t�F�N�g�̍Đ�
	if (m_effect != NULL)
	{
		m_handle = m_manager->Play( m_effect, 0, 0, 0 );
	}
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::stopEffect() {
	m_manager->StopEffect(m_handle);
	m_handle = -1;
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::stopAllEffects() {
	m_manager->StopAllEffects();
	m_handle = -1;
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::addLocation(float x, float y, float z) {
	if (m_handle >= 0)
		m_manager->AddLocation(m_handle, ::Effekseer::Vector3D(x, y, z));
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setLocation(float x, float y, float z) {
	if (m_handle >= 0)
		m_manager->SetLocation(m_handle, ::Effekseer::Vector3D(x, y, z));
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setRotation(::Effekseer::Vector3D axis, float angle) {
	if (m_handle >= 0)
		m_manager->SetRotation(m_handle, axis, angle);
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setRotation(float x, float y, float z) {
	if (m_handle >= 0)
		m_manager->SetRotation(m_handle, x, y, z);
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setScale(float x, float y, float z) {
	if (
		m_manager != NULL &&
		m_handle >= 0 &&
		m_manager->Exists(m_handle)
		)
	{
		m_manager->SetScale(m_handle, x, y, z);
	}
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setPaused(bool paused) {
	if (m_handle >= 0)
		m_manager->SetPaused(m_handle, paused);
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setBaseTranslation(float x, float y, float z) {
	::Effekseer::Matrix44 mat;
	mat.Indentity();
	mat.Values[3][0] = x / (float)m_displayW;
	mat.Values[3][1] = y / (float)m_displayH;
	mat.Values[3][2] = 0.0f;
	
	::Effekseer::Matrix44 matProj;
	mat.Mul(matProj, ::Effekseer::Matrix44().PerspectiveFovRH( 90.0f / 180.0f * 3.14f, (float)m_displayW / (float)m_displayH, 1.0f, 50.0f ) , mat);

	m_renderer->SetProjectionMatrix( matProj);
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setScreenPos(float x, float y) {
	// �G�t�F�N�g���W��ݒ�
	m_posX = x - ::abs(m_displayW - nn::yb::e::global::WINDOW_W) / 2;//��ʃT�C�Y�ɑΉ����ăI�t�Z�b�g��������
	m_posY = y - ::abs(m_displayH - nn::yb::e::global::WINDOW_H) / 2;
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::setCameraPos(float x, float y) {
	//�G�t�F�N�g�̃J�������W��ݒ�
	m_camX = x;
	m_camY = y;
}

//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::disp3DEffects() {
	
	////::nn_commonLog( 1, "m_posX = %f, m_posY = %f\n", m_posX, m_posY);
	////::nn_commonLog( 1, "m_camX = %f, m_camY = %f\n", m_camX, m_camY);

	::Effekseer::Matrix44 mat;
	mat.Indentity();
	mat.Values[3][0] = (m_posX - m_camX) / (float)m_displayW * 2.0f;
	mat.Values[3][1] = -(m_posY - m_camY) / (float)m_displayH * 2.0f;
	mat.Values[3][2] = 0.0f;
	
	::Effekseer::Matrix44 matProj;
	mat.Mul(matProj, ::Effekseer::Matrix44().PerspectiveFovRH( 90.0f / 180.0f * 3.14f, (float)m_displayW / (float)m_displayH, 1.0f, 50.0f ) , mat);

	m_renderer->SetProjectionMatrix( matProj);


	if (
		m_effect != NULL 
		////&& m_handle > 0
		)
	{
		// ���ł������̂�TransFrag��L���ɂ��ĉ摜��`�悷��B
		// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ��B
		////DrawGraph( 0, 0, grHandle, TRUE );
		fontPutsType( -1000, -1000, nn::yb::e::global::FONT_TYPE_SIZE_18, RGB_WHITE, ".");//�ŏ��ɕ`�悵�Ȃ��ƁAEffekseer�͕`��ł��Ȃ����߁A��ʊO�ɕ`�悷��

		// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
		RenderVertex();

		// �G�t�F�N�g�̈ړ��������s��
		////m_manager->AddLocation( m_handle, ::Effekseer::Vector3D( 0.2f, 0.0f, 0.0f ) );

		// �G�t�F�N�g�̍X�V�������s��
		m_manager->Update();
			
		// �G�t�F�N�g�̕`��J�n�������s���B
		m_renderer->BeginRendering();

		// �G�t�F�N�g�̕`����s���B
		m_manager->Draw();

		// �G�t�F�N�g�̕`��I���������s���B
		m_renderer->EndRendering();

		// �X�N���[������ւ�
		////ScreenFlip();
	}
}
//---------------------------------------------------------------------------

void nn::yb::main::CEffectCtrl::push_event( void (* eventCallBackFunc)(void *, void *), void * eventCallBackArg1, void * eventCallBackArg2) {

	////::nn_commonLog(1,  "before push_event() index = %d\n", this->eventCallBackStackIndex);
	this->eventCallBackFuncStack[this->eventCallBackStackIndex] = eventCallBackFunc;
	this->eventCallBackArg1Stack[this->eventCallBackStackIndex] = eventCallBackArg1;
	this->eventCallBackArg2Stack[this->eventCallBackStackIndex] = eventCallBackArg2;
	this->eventCallBackStackIndex++;
	////::nn_commonLog(1,  "after push_event() index = %d\n", this->eventCallBackStackIndex);

}

//---------------------------------------------------------------------------

void nn::yb::main::CEffectCtrl::pop_event() {

	::nn_commonLog(999,  "before pop_event() index = %d\n", this->eventCallBackStackIndex);
	if(this->eventCallBackStackIndex >= 1) {
		this->eventCallBackStackIndex--;
		if(this->eventCallBackFuncStack[this->eventCallBackStackIndex] != NULL) {
			this->eventCallBackFuncStack[this->eventCallBackStackIndex](
				this->eventCallBackArg1Stack[this->eventCallBackStackIndex],
				this->eventCallBackArg2Stack[this->eventCallBackStackIndex]
			);
		}
	}
	::nn_commonLog(999,  "after pop_event() index = %d\n", this->eventCallBackStackIndex);
}
//---------------------------------------------------------------------------

void nn::yb::main::CEffectCtrl::clear_cb() {

	memset( this->eventCallBackFuncStack, 0, sizeof( this->eventCallBackFuncStack));
	memset( this->eventCallBackArg1Stack, 0, sizeof( this->eventCallBackArg1Stack));
	memset( this->eventCallBackArg2Stack, 0, sizeof( this->eventCallBackArg2Stack));
	this->eventCallBackStackIndex = 0;

}

//---------------------------------------------------------------------------

void nn::yb::main::CEffectCtrl::push_hole_event( void (* holeEventCallBackFunc)(void *, void *), void * holeEventCallBackArg1, void * holeEventCallBackArg2) {

	////::nn_commonLog(1,  "before push_event() index = %d\n", this->holeEventCallBackStackIndex);
	this->holeEventCallBackFuncStack[this->holeEventCallBackStackIndex] = holeEventCallBackFunc;
	this->holeEventCallBackArg1Stack[this->holeEventCallBackStackIndex] = holeEventCallBackArg1;
	this->holeEventCallBackArg2Stack[this->holeEventCallBackStackIndex] = holeEventCallBackArg2;
	this->holeEventCallBackStackIndex++;
	////::nn_commonLog(1,  "after push_event() index = %d\n", this->holeEventCallBackStackIndex);
}

//---------------------------------------------------------------------------

void nn::yb::main::CEffectCtrl::pop_hole_event() {

	::nn_commonLog(999,  "before pop_hole_event() index = %d\n", this->holeEventCallBackStackIndex);
	if(this->holeEventCallBackStackIndex >= 1) {
		this->holeEventCallBackStackIndex--;
		if(this->holeEventCallBackFuncStack[this->holeEventCallBackStackIndex] != NULL) {
			this->holeEventCallBackFuncStack[this->holeEventCallBackStackIndex](
				this->holeEventCallBackArg1Stack[this->holeEventCallBackStackIndex],
				this->holeEventCallBackArg2Stack[this->holeEventCallBackStackIndex]
			);
		}
	}
	::nn_commonLog(999,  "after pop_hole_event() index = %d\n", this->holeEventCallBackStackIndex);
}
//---------------------------------------------------------------------------

void nn::yb::main::CEffectCtrl::clear_hole_cb() {

	memset( this->holeEventCallBackFuncStack, 0, sizeof( this->holeEventCallBackFuncStack));
	memset( this->holeEventCallBackArg1Stack, 0, sizeof( this->holeEventCallBackArg1Stack));
	memset( this->holeEventCallBackArg2Stack, 0, sizeof( this->holeEventCallBackArg2Stack));
	this->holeEventCallBackStackIndex = 0;

}
//---------------------------------------------------------------------------

void nn::yb::main::CEffectCtrl::cb_pop_hole_event( void * effectCtrlArg, void * dummy) {

	nn::yb::main::CEffectCtrl * effect = (nn::yb::main::CEffectCtrl *)effectCtrlArg;

	effect->pop_hole_event();
}
//---------------------------------------------------------------------------
void nn::yb::main::CEffectCtrl::disp3DEffectsCb( void * effectCtrl) {
	
	nn::yb::main::CEffectCtrl * effect_ctrl = (nn::yb::main::CEffectCtrl *)effectCtrl;

	effect_ctrl->disp3DEffects();

}
//---------------------------------------------------------------------------
