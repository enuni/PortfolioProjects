// CTRL を置換
// ctrl を置換
// Ctrl を置換
// yb を置換
// main を置換

#ifndef __EFFECT_CTRL_H__
#define __EFFECT_CTRL_H__

#include "task_save.h"
////#include "master.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>

//---------------------------------------------------------------------------
// 前方宣言
namespace nn {
namespace yb {

namespace main {

} // namespece main

////namespace edit {
////	class CEdit;
////} // namespece edit

} // namespace yb
} // namespace nn
namespace nn {
namespace yb {
//---------------------------------------------------------------------------
// enum
////namespace e {
////namespace ctrl {
////
////enum STATE {
////	STATE_NONE,
////	STATE_EDIT,
////	
////	STATE_MAX
////};
////
////} // namespece ctrl
////} // namespece e
//---------------------------------------------------------------------------
// classs
namespace main {
	
class CEffectCtrl : public TASK_SAVE {
	
public:
	CEffectCtrl( TASK_SAVE * oya);
	virtual ~CEffectCtrl();
	void task();
	void disp();
	
	void start();



	//added by nullpo for Effect
	void push_event( void (* eventCallBackFunc)(void *, void *), void * eventCallBackArg1, void * eventCallBackArg2);
	void pop_event();
	void clear_cb();
private:
	void (* eventCallBackFuncStack[256])( void *, void *);
	void * eventCallBackArg1Stack[256];
	void * eventCallBackArg2Stack[256];
	int eventCallBackStackIndex;

public:
	//for hole effect
	void push_hole_event( void (* holeEventCallBackFunc)(void *, void *), void * holeEventCallBackArg1, void * holeEventCallBackArg2);
	void pop_hole_event();
	void clear_hole_cb();

	static void cb_pop_hole_event( void * effectCtrlArg, void * dummy);
private:
	void (* holeEventCallBackFuncStack[256])( void *, void *);
	void * holeEventCallBackArg1Stack[256];
	void * holeEventCallBackArg2Stack[256];
	int holeEventCallBackStackIndex;


	//added by nullpo for 3D Effect
private:
 static ::Effekseer::Manager*			m_manager;
 static ::EffekseerRenderer::Renderer*	m_renderer;
 static ::Effekseer::Effect*				m_effect ;
 static ::Effekseer::Handle				m_handle;
 static int grHandle;

public:
	void disp3DEffects();

	static void initialize(int displayW, int displayH);
	static void uninitialize();

	static void setRootPath(const WCHAR* path);

	static void loadEffect(const WCHAR* path);
	static void unloadEffect();
	static void playEffect();
	static void stopEffect();
	static void stopAllEffects();
	static void addLocation(float x, float y, float z);
	static void setLocation(float x, float y, float z);
	static void setRotation(::Effekseer::Vector3D axis, float angle);
	static void setRotation(float x, float y, float z);
	static void setScale(float x, float y, float z);
	static void setPaused(bool paused);
	static void setBaseTranslation(float x, float y, float z);

public:
	static float m_posX;
	static float m_posY;
	static void setScreenPos(float x, float y);
	static float m_camX;
	static float m_camY;
	static void setCameraPos(float x, float y);
	static void disp3DEffectsCb( void * effectCtrl);
	
	static int m_effect3D_z;//3Dエフェクトの描画時のZ値
	
	static int m_displayW;//ウィンドウサイズ
	static int m_displayH;//ウィンドウサイズ

}; // CEffectCtrl
	 
} // namespece main

} // namespace yb
} // namespace nn
	

#endif // __EFFECT_CTRL_H__
