#ifndef __DRAW_3D_H__
#define __DRAW_3D_H__

#include "dxlib.h"

namespace nn {
namespace common {
namespace dxlib {
namespace draw3d {

#define DRAW3D_MODEL_FILE_NAME_LENGTH	(64)
#define DRAW3D_MODEL_CTRL_N				(256)

typedef struct _model {
	char fileName[DRAW3D_MODEL_FILE_NAME_LENGTH];
} MODEL;

typedef struct _model_ctrl {
	int modelHandle;
	int animeHandle;
	int animeIndex;
	int animeModelId;
	float allTime;
} MODEL_CTRL;

typedef struct _ctrl {
	MODEL * modelArray;
	MODEL_CTRL modelCtrlArray[DRAW3D_MODEL_CTRL_N];
	int modelCtrlN;
	int modelN;
} CTRL;
} // draw3d

class CDraw3d {
public:
	CDraw3d( nn::common::dxlib::draw3d::MODEL * modelArray = NULL, int modelArrayIndexN = 0);
	int loadModel( char * fileName);
	void loadModel( int modelId);
	nn::common::dxlib::draw3d::MODEL_CTRL * getModelCtrl( int modelId);
	void model( int modelId, int animeModelId, int animeIndex, double x, double y, double z, double time);
	
	void cameraTarget( double x, double y, double z, double hAngle, double vAngle, double length);
	void cameraView( double x, double y, double z, double hAngle, double vAngle, double length);
	nn::common::dxlib::draw3d::CTRL ctrl;

	void modelScale( int modelId, int animmeModelId, int animeIndex, DxLib::VECTOR pos, DxLib::VECTOR scale, double time); 
	void modelEx( int modelId, int animmeModelId, int animeIndex,
		DxLib::VECTOR pos, DxLib::VECTOR rot, ::DxLib::VECTOR scale,
		double time
	);
	void modelExColor( int modelId, int animmeModelId, int animeIndex,
		DxLib::VECTOR pos, DxLib::VECTOR rot, ::DxLib::VECTOR scale,
		double time,
		COLOR_F lightDifColor
	);

};


} // dxlib
} // common
} // nn

#endif //__DRAW_3D_H__

