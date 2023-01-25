// PARTICLE を置換
// particle を置換
// Particle を置換
// common を置換
// graphics2d を置換

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "task_save.h"
//---------------------------------------------------------------------------
namespace nn {
namespace common {

// enum
namespace e {
namespace particle {

enum STATE {
	STATE_NONE,
	STATE_DISP,
	
	STATE_MAX
};

} // namespece particle
} // namespece e
//---------------------------------------------------------------------------
// classs
namespace graphics2d {
	
class CParticle : public TASK_SAVE {
	
public:
	CParticle( TASK_SAVE * oya);
	CParticle( TASK_SAVE * oya, nn_common::SaveManager * m);
	virtual ~CParticle();
	void constructorCommon();
	void task();
	void disp();
	
	void start();

	int state;
	int oldState;
	int waitCnt;
	
	int objId;
	double x;
	double y;
	double z;
	int dispZ;
	double angle;
	double speed;
	double angleSpeed;
	double addSpeed;
	double addAngleSpeed;
	double zSpeed;
	double addZSpeed;
	double rotate;
	double rotateSpeed;
	double w;
	double h;
	double wSpeed;
	double hSpeed;
	double alpha;
	double alphaSpeed;
	
	void (* taskCb)(void * particleArg, void * taskCbArg);
	void * taskCbArg;
	void (* dispCb)(void * particleArg, void * dispCbArg);
	void * dispCbArg;
	
	void (* endCb)( void * endArg);
	void * endCbArg;

	unsigned int frameCnt;
	int stateUser;

	void (* task2Cb)(void * particleArg, void * task2CbArg);
	void * task2CbArg;

}; // CParticle
	 
} // namespece graphics2d

} // namespace common
} // namespace nn
	

#endif // __PARTICLE_H__
