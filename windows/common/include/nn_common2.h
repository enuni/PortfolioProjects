
#ifndef __NN_COMMON_H__
#define __NN_COMMON_H__

#include <math.h>
#include <map>

namespace nn {
namespace common {

namespace util {

void setCrc( unsigned char * buf, int size, unsigned short key);		// size = CRC=2Byte�܂ރT�C�Y
int checkCrc( unsigned char * buf, int size, unsigned short key);		// return = 0 �������f�[�^ size = CRC=2Byte�܂ރT�C�Y

#define NN_COMMON_GET_ARRAY_N( a) ( sizeof( a) / sizeof( a[0]) )
#define NN_COMMON_GET_ARRAY_SIZE( a) ( sizeof( a) / sizeof( a[0]) )
#define NN_COMMON_IS_POS_AROUND( x, y, pos_x, pos_y, area) (pos_x - area <= x && x <= pos_x + area && pos_y - area <= y && y <= pos_y + area) \
	// x y���l�p�Ɏ��܂��Ă��邩��Ԃ� pos_x - area �` pos_x + area �� pos_y - area �` pos_y + area
#define NN_COMMON_IS_POS_RECT( x, y, pos_x, pos_y, pos_w, pos_h) (pos_x  <= x && x <= pos_x + pos_w && pos_y <= y && y <= pos_y + pos_h) \
	// x y���l�p�Ɏ��܂��Ă��邩��Ԃ� pos_x �` pos_x + pos_w �� pos_y �` pos_y + pos_h

#define NN_COMMON_PI (3.1415926535897932384626433832795)
#define NN_COMMON_ANGLE_2_RAD( a) (a * NN_COMMON_PI / 180.0)

#define NN_COMMON_SLANTING_COEFFICIENT (0.709)

int numofbits(long bits);	// �����Ă�r�b�g�̐���Ԃ�

namespace e {
namespace e2d {

enum {
	COLLISION_ITEM_SHAPE_LINE_UP,
	COLLISION_ITEM_SHAPE_LINE_RIGHT,
	COLLISION_ITEM_SHAPE_LINE_DOWN,
	COLLISION_ITEM_SHAPE_LINE_LEFT,
	COLLISION_ITEM_SHAPE_CIRCLE,

	COLLISION_ITEM_TYPE_MAX
};

}; // namespace e2d
}; // namespace e

namespace type {
namespace t2d {

// �O���t�B�b�N�X�ɂ��Ȃ����W�n
typedef struct _point2_s32 {
	int x1;
	int y1;
	int x2;
	int y2;
} POINT2_S32;

typedef struct _point2_f64 {
	double x1;
	double y1;
	double x2;
	double y2;
} POINT2_F64;

typedef struct _point_s32 {
	int x;
	int y;
} POINT_S32;

typedef struct _collision_item {
	int shape;
	POINT2_F64 p;
	double r;
	int id;
	void * target;
} COLLISION_ITEM;

} // namespace t2d
} // namespace type

class CCallBackManager {
public:
	CCallBackManager();
	virtual ~CCallBackManager();

	static void init();
	static void add( int key, void * callBackFunc);
	static void * get( int key);

	static nn::common::util::CCallBackManager * staticCallBackManager;
	std::map<int, void *> callbackFuncMap;
};

} // namespace util

namespace def {

namespace graphics2d {

typedef struct _rect {
	int x;
	int y;
	int w;
	int h;
} RECT;

typedef struct _area {
	int x1;
	int y1;
	int x2;
	int y2;
} AREA;

typedef struct _circle {
	int x;
	int y;
	int r;
} CIRCLE;

typedef struct _vector2 {
	int x;
	int y;
} VECTOR2;

typedef struct _vector2_d {
	double x;
	double y;
} VECTOR2_D;	// 2�����x�N�g��

typedef struct _line {
	nn::common::def::graphics2d::VECTOR2 s;
	nn::common::def::graphics2d::VECTOR2 e;
} LINE;

typedef struct _line_d {
	nn::common::def::graphics2d::VECTOR2_D s;
	nn::common::def::graphics2d::VECTOR2_D e;
} LINE_D;

typedef struct segment2_d {
	nn::common::def::graphics2d::VECTOR2_D s;
	nn::common::def::graphics2d::VECTOR2_D v;
} SEGMENT2_D;	// 2�̃x�N�g���������߂�(�n�_�ƃx�N�g��) 2����

typedef struct _dda_data {
	int x;
	int y;
	int xDst;
	int yDst;
	
	int vx;
	int vy;
	int dx;
	int dy;
	int diff;

	int speed;
} DDA_DATA;

} // namespace graphics2d
} // namespace def

namespace graphics2d {

#define NN_COMMON_POW2(a) ((a) * (a))

#define NN_COMMON_IS_CIRCLE_HIT( a, b, r) ((NN_COMMON_POW2(a) + NN_COMMON_POW2(b)) <= NN_COMMON_POW2(r))

#define NN_COMMON_VEC2ANGLE(dp, len1, len2) ((int)(180.0 * acos(dp / (len1 * len2)) / NN_COMMON_PI))

#define NN_COMMON_VEC2ANGLE2(dot, crs) ((int)(180.0 * atan2(crs, dot) / NN_COMMON_PI))

#define NN_COMMON_VEC2ANGLE2D(dot, crs) ((180.0 * atan2(crs, dot) / NN_COMMON_PI))

#define NN_COMMON_POS_2_LEN( x1, y1, x2, y2) sqrt(NN_COMMON_POW2((x1) - (x2)) + NN_COMMON_POW2((y1) - (y2)))

void vec2segment( nn::common::def::graphics2d::SEGMENT2_D * r, nn::common::def::graphics2d::VECTOR2_D * s,
				 nn::common::def::graphics2d::VECTOR2_D * e);	// 2�̃x�N�g���̐������߂�(�n�_�ƃx�N�g��)
double vec2dot( nn::common::def::graphics2d::VECTOR2_D * v1, nn::common::def::graphics2d::VECTOR2_D * v2);	// ����
double vec2cross( nn::common::def::graphics2d::VECTOR2_D * v1, nn::common::def::graphics2d::VECTOR2_D * v2);	// �O��
bool isCollisionLine( nn::common::def::graphics2d::VECTOR2_D * collision, nn::common::def::graphics2d::SEGMENT2_D * after,
					 nn::common::def::graphics2d::SEGMENT2_D * move, nn::common::def::graphics2d::SEGMENT2_D * line);	// �ړ���(����)�Ɛ����̏Փ˔���	
void line2segment( nn::common::def::graphics2d::SEGMENT2_D * r, nn::common::def::graphics2d::LINE * line);	//���C�����n�_�ƃx�N�g���̌`�ɂ���
bool isCollisionCircle( nn::common::def::graphics2d::SEGMENT2_D * after, double * afterLen, nn::common::def::graphics2d::SEGMENT2_D * move_vec, double move_r, nn::common::def::graphics2d::VECTOR2_D * circle_pos, double circle_r);	// �ړ���(�~�̐���)�Ɖ~�̏Փ˔���
bool isArea( int x, int y, nn::common::def::graphics2d::RECT * rect);	// x y��rect���̂����true

double vec2dot( double vx1, double vy1, double vx2, double vy2);
double vec2cross( double vx1, double vy1, double vx2, double vy2);

void initStart2GollDDA( int gx, int gy,		// �ڕW���W
				 int sx, int sy,			// �X�^�[�g���W
				 int & vx, int & vy,		// �ݒ�ړ�����
				 int & dx, int & dy,		// �ݒ荷��(�ړ���)
				 int & diff					// �ݒ�덷
);
bool moveStart2GollDDA( int & x, int & y,	// ���W
					  int gx, int gy,		// �ڕW���W
					  int vx, int vy,		// �ړ�����
					  int dx, int dy,		// ����(�ړ���)
					  int & diff,			// �덷
					  int speed			// ���x
);

int pos2angle( double x1, double y1, double x2, double y2);
double vec2angle_d( const nn::common::def::graphics2d::VECTOR2_D * vec); \
	// VECTOR2_D��n����x=1 y=0����̊p�x��0.0�`359.0�ŕԂ�

} // namespace graphics2d
namespace math {

namespace e {

enum LEFT_RIGHT {
	LEFT_RIGHT_LEFT,
	LEFT_RIGHT_RIGHT,
	LEFT_RIGHT_STRAIGHT,

	LEFT_RIGHT_MAX
};

}

#if 0
// ����Ă݂����ǁA����
// ��������֐����x��tamplate�̕ϊ��ӎ�����̖ʓ|
// int�Ŏg�����Ƃ�����H
// �Ȃ�double�Œ�ŗǂ��ˁH
// ���Z�q�I�[�o�[���[�h�����ŗǂ���
template<class T> class CVector3 {

public:

	T x;
	T y;
	T z;

	inline CVector3() {
		this->x = (T)0;
		this->y = (T)0;
		this->z = (T)0;
	};

	inline CVector3(T x, T y, T z) {
		this->x = x;
		this->y = y;
		this->z = z;
	};

	inline CVector3(T startX, T startY, T endX, T endY) {
		this->x = endX - startX;
		this->y = endY - startY;
		this->z = (T)0;
	};

	inline CVector3(T startX, T startY, T startZ, T endX, T endY, T endZ) {
		this->x = endX - startX;
		this->y = endY - startY;
		this->z = endZ - startZ;
	};

	~CVector3() {
		
	};

	inline CVector3<T> & operator=(const CVector3<T> & arg) {
		this->x = arg.x;
		this->y = arg.y;
		this->z = arg.z;
		return *this;
	};

	inline CVector3<T> operator+(const CVector3<T> & arg) {
		CVector3<T> temp;
		temp.x = this->x + arg.x;
		temp.y = this->y + arg.y;
		temp.z = this->z + arg.z;
		return temp;
	};

	inline CVector3<T> & operator+=(const CVector3<T> & arg) {
		this->x += arg.x;
		this->y += arg.y;
		this->z += arg.z;
		return *this;
	};

	inline CVector3<T> operator-(const CVector3<T> & arg) {
		CVector3<T> temp;
		temp.x = this->x - arg.x;
		temp.y = this->y - arg.y;
		temp.z = this->z - arg.z;
		return temp;
	};

	inline CVector3<T> & operator-=(const CVector3<T> & arg) {
		this->x -= arg.x;
		this->y -= arg.y;
		this->z -= arg.z;
		return *this;
	};

	inline T dot(const CVector3<T> & arg) {
		return (this->x * arg.x +
			this->y * arg.y +
			this->z * arg.z);
	}

	inline CVector3<T> cross(const CVector3<T> & arg) {
		CVector3<T> temp;
		temp.x = this->y * arg.z - this->z * arg.y;
		temp.y = this->z * arg.x - this->x * arg.z;
		temp.z = this->x * arg.y - this->y * arg.x;
		return temp;
	}

	inline T cross2d(const CVector3<T> & arg) {
		return (this->x * arg.y - this->y * arg.x);
	}

};

template<class T> inline int vec2d2angle( const CVector3<T> & vec) {

	CVector3<T> base;
	base.x = 1;
	base.y = 0;
	base.z = 0;

	CVector3<T> target;
	target = vec;
	target.y = -target.y;

	double dot = (double)base.dot( target);
	double cross = (double)base.cross2d( target);

	int angle = (int)NN_COMMON_VEC2ANGLE2D( dot, cross);
	angle = (angle + 360) % 360;

	return angle;
};	// CVector3��n����x=1 y=0����̊p�x��0�`359�ŕԂ�

template<class T> inline int pos2Angle( const T startX, const T startY, const T endX, const T endY) {

	CVector3<T> vec( startX, startY, endX, endY);
	return vec2d2angle( vec);

};	// startX startY����endX endY�܂ł̃x�N�g����x=1 y=0����̊p�x��0�`359�ŕԂ�
#else
//-----------------------------------------------------------------------------
class CVector3_d {
public:
	double x;
	double y;
	double z;

	CVector3_d();
	CVector3_d(const CVector3_d & vec);
	CVector3_d(double x, double y, double z);
	CVector3_d(double startX, double startY, double endX, double endY);
	CVector3_d(double startX, double startY, double startZ,
		double endX, double endY, double endZ
	);
	~CVector3_d();
	CVector3_d & operator=(const CVector3_d & arg);
	CVector3_d operator+(const CVector3_d & arg);
	CVector3_d & operator+=(const CVector3_d & arg);
	CVector3_d operator-(const CVector3_d & arg);
	CVector3_d & operator-=(const CVector3_d & arg);
	double dot(const CVector3_d & arg);
	CVector3_d cross(const CVector3_d & arg);
	double cross2d(const CVector3_d & arg);
};
//-----------------------------------------------------------------------------
class CPosVector3_d {
public:
	CVector3_d p;
	CVector3_d v;

	CPosVector3_d() :
		p(),
		v()
	{};
	CPosVector3_d( const CVector3_d & p, const CVector3_d & v)
		: p(p),
		v(v)
	{};
	CPosVector3_d( const CPosVector3_d & pv)
		: p(pv.p),
		v(pv.v)
	{};
	CPosVector3_d( const double startX, const double startY, const double endX, const double endY)
		: p( startX, startY, 0.0),
		v( startX, startY, endX, endY)
	{};
	~CPosVector3_d() {};
};
//-----------------------------------------------------------------------------
// CVector3��n����x=1 y=0����̊p�x��0�`359�ŕԂ�
int vec2d2angle( const CVector3_d & vec);

// startX startY����endX endY�܂ł̃x�N�g����x=1 y=0����̊p�x��0�`359�ŕԂ�
int pos2Angle( const double startX, const double startY, const double endX, const double endY);

// CVector3��n����x=1 y=0����̊p�x��0.0�`359.X�ŕԂ�
double vec2d2angle_d( const CVector3_d & vec);

// startX startY����endX endY�܂ł̃x�N�g����x=1.0 y=0.0����̊p�x��0.0�`359.X�ŕԂ�
double pos2Angle_d( const double startX, const double startY, const double endX, const double endY);

// �~�Ɖ~�̏Փ˔���
bool isCollisionCircle2Circle(double ax, double ay, double ar, double bx, double by, double br);

// a��b�̋�����Ԃ�
double getLength( double ax, double ay, double bx, double by);

// thisAngle�̌����ɑ΂��āAthis2goal�̕�������0�ɂ��邩�E1�ɂ��邩����2�ɂ��邩��Ԃ� enum LEFT_RIGHT
int getLeftRight( int thisAngle, CVector3_d & this2goal);

// angle��speed���琅���̈ړ��ʂ�Ԃ�
double angleSpeed2MoveX( double angle, double speed);

// angle��speed���琂���̈ړ��ʂ�Ԃ�
double angleSpeed2MoveY( double angle, double speed);

bool isCollisionLine2CircleEX(
	double * collisionX, double * collisionY,
	double * afterXRate, double * afterYRate,
	double * afterLen,
	double cx, double cy, double cr,
	double lx1, double ly1, double lx2, double ly2
);
bool isCollisionLine2Circle( double cx, double cy, double cr,
	double lx1, double ly1, double lx2, double ly2
);

// ���Ɛ��̏Փ˔��� �Փˈʒu������
bool isCollisionLine2Line( double * collisionX, double * collisionY,
	  double ax1, double ay1, double ax2, double ay2,
	  double bx1, double by1, double bx2, double by2
);

// ���Ɛ��̏Փ˔��� �̂�
bool isCollisionLine2LineSimple(
	  double ax1, double ay1, double ax2, double ay2,
	  double bx1, double by1, double bx2, double by2
);

// ���Ɛ��̏Փ˔��� �Փˌ��a�̒��˕Ԃ����������
bool isCollisionLine2LineEx(
	double * collisionX1, double * collisionY1,
	double * collisionX2, double * collisionY2,
	double ax1, double ay1, double ax2, double ay2,
	double bx1, double by1, double bx2, double by2
);

// n�̌����𓾂�
int getDigitNum( int n);	

// n�̐�Βl�𓾂�
int getAbs( int n);

// angle + var��0.0�`359.0�Ɏ��߂ĕԂ�
double varAngle( double angle, double var);

double getBezier( double p1, double p2, double p3, double t);
double getAccPos( int sPos, int ePos, int now, int total, unsigned char add);

#endif
//-----------------------------------------------------------------------------
class CEllipse {

public:
	CEllipse();
	void task();

	double move;
	double moveSpeed;
	double xWidthRate;
	double yWidthRate;
	int baseX;
	int baseY;
	int angle;
	double dispX;
	double dispY;
	double r;
	int angleSpeed;

};
//-----------------------------------------------------------------------------
} // namespace math
} // namespace common
} // namespace nn

#endif //__NN_COMMON_H__


