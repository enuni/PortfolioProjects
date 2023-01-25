
#ifndef __AI_MOVE_2D_H__
#define __AI_MOVE_2D_H__

namespace nn {
namespace common {

namespace e {
namespace ai_move_2d {

enum ACTION_STATE {
	ACTION_STATE_IDLE,
	ACTION_STATE_APPROCH,

	ACTION_STATE_MAX
};

enum ACTION_TYPE {
	ACTION_TYPE_STOP,
	ACTION_TYPE_LOITERING,
	ACTION_TYPE_APPROCH,
	ACTION_TYPE_LEAVE,

	ACTION_TYPE_MAX
};

} // namespace ai_move_2d
} // namespace e

namespace util {

class CAIMove2d {
	
public:
	CAIMove2d();
	~CAIMove2d();
	void constructorCommon();
	void taskMain();
	void approach( double speed);
	void leave( double speed);
	bool isCollisionHitLine( double x, double y, double r);
	bool isCollisionHitLine2Line( double x1, double y1, double x2, double y2);

	double * x;
	double * y;
	double * targetX;
	double * targetY;
	double * r;
	
	double moveX;
	double moveY;

	int actionType[nn::common::e::ai_move_2d::ACTION_STATE_MAX];
	double actionApprochLength;
	double actionSpeed[nn::common::e::ai_move_2d::ACTION_STATE_MAX];

	nn::common::util::type::t2d::POINT2_S32 * hitLines;
	int hitLineN;

	unsigned int moveFrameCntDwn;
	
};

} // namespace util
} // namespace common
} // namespace nn

#endif // __AI_MOVE_2D_H__

