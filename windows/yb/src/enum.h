
#ifndef __ENUM_H__
#define __ENUM_H__

namespace nn {
namespace yb {

namespace e {
namespace global {

enum SPELL_CARD {
	SPELL_CARD_NONE,
	SPELL_CARD_MUSO,
	SPELL_CARD_SPEA,
	SPELL_CARD_MSPA,
	SPELL_CARD_MOMIJI,
	SPELL_CARD_TENKO,
	SPELL_CARD_NITORI,
	SPELL_CARD_FLAN,
	SPELL_CARD_ORIN,

	SPELL_CARD_MAX
};

const unsigned short CRC_KEY	= 0x2347;

const int		WINDOW_W		= 640;
const int		WINDOW_H		= 480;
const int		WINDOW_W_HALF	= 320;
const int		WINDOW_H_HALF	= 240;

const int		STAGE_N			= 6;

const int		FONT_TYPE_SIZE_18		= 0;
const int		FONT_TYPE_SIZE_10		= 1;

//#define SHOT_BASE_SPEED					(9.0)
//#define SHOT_BASE_MAX_POWER				(100)

// �ʏ퓮��֌W
const double	FLYING_G					= 0.160;

const double	FIELD_DEC_SPEED_BASE			= 0.056;	
const double	FIELD_DEC_SPEED_BASE_FLY		= 0.014;
const double	FIELD_DEC_SPEED_BASE_BOUND_RATE	= 0.056;
const double	FIELD_DEC_SPIN_BASE_RATE		= 0.990;

const double	SHOT_BASE_MAX_POWER				= 60.000;
const double	SHOT_BASE_Z_SPEED_NORMAL		=  8.000;		// �t���C�́A�t���p���[�̂�
const double	SHOT_BASE_Z_SPEED_FULL			=  8.000;
const double	SHOT_BASE_SPEED_NORMAL			= 10.500;		// �p���[�؂�ւ��͂Ȃ�
const double	SHOT_BASE_SPEED_FULL			= 10.500;	
const double	SHOT_BASE_SPEED_FLY_NORMAL		=  4.500;		// �t���C�́A�t���p���[�̂�
const double	SHOT_BASE_SPEED_FLY_FULL		=  4.500;
const int		SHOT_ROT_HORIZON_MAX			=  5;
const int		SHOT_ROT_VERTICAL_MAX			=  6;
const int		SHOT_SPIN_VERTICAL_WAIT_FRAME	=  3;
const int		SHOT_SPIN_VERTICAL_MOVE_DIV		=  3;

const double	FLY_SPIN_VERTICAL_TOP_RATE				=	0.350;			// �n�ʒ��˕Ԃ莞�ɐ�����]�l�𑬓x�ɉ��Z���鎞�̐�����]�l�̌W��
const double	FLY_SPIN_VERTICAL_BACK_RATE				=	0.700;			// �n�ʒ��˕Ԃ莞�ɐ�����]�l�𑬓x�ɉ��Z���鎞�̐�����]�l�̌W��
const double	FLY_BOUND_Z_SPEED_RATE					=	0.600;			// �n�ʒ��˕Ԃ莞��z�����x�̌W��

const double	FLY_SPIN_HORIZON_FLY_RATE					=	 8.650;		// FLY���̐�����]�l���p�x�ɉ��Z���鎞�̐�����]�l�̌W��
const double	FLY_SPIN_HORIZON_RATE						=	 0.900;		// ������]�l���p�x�ɉ��Z���鎞�̐�����]�l�̌W��

// �X�y�J �}�X�^�[�X�p�[�N
const double	FLYING_MSPARK_RELEASE_SPEED				= 13.000;
const double	FLYING_MSPARK_RELEASE_SPEED_DEC_RATE	= 0.400;
const double	FLYING_MSPARK_SET_SPEED					= 15.000;

// �X�y�J ���z����
const double	FLYING_MUSOFUIN_SET_SPEED		= 3.500;
const double	FLYING_MUSOFUIN_SET_Z_SPEED		= 7.800;

// �J�b�v�֌W
const double	FLYING_HIGH_SPEED		= 10.0;
const double	FLYING_MIDDLE_SPEED		=  2.0;
const double	FLYING_LOW_SPEED		=  FIELD_DEC_SPEED_BASE + 0.001;

const double	FLYING_HIT_CUP_DEC_RATE								= 0.6;
const int		FLYING_HIT_CUP_SKIP_CNT								= 4;
const double	FLYING_FALL_CONST_SPEED								= 0.9;
const double	FLYING_SKATE_CUP_EDGE_ACC_SPEED						= 0.0300;
const double	FLYING_SKATE_CUP_EDGE_ANGLE_ALTER_RATE_MIDDLE_SPEED	= 0.0400;
const double	FLYING_SKATE_CUP_EDGE_ANGLE_ALTER_RATE_LOW_SPEED	= 0.125;

const int		DP_VEC_CHECK_SLIDE_ANGLE	= 5;

const int		BALL_2_CUP_REAR_ANGLE	= 150;
const int		BALL_2_CUP_SIDE_ANGLE	=  60;

// �{�[���O���\��
const double	BALL_VEC_GUIDE_DISP_START				= 19.0;
const double	BALL_VEC_GUIDE_DISP_INTERVAL_NORMAL		= 19.0;
const double	BALL_VEC_GUIDE_DISP_INTERVAL_FULL		= 53.0;
const double	BALL_VEC_GUIDE_DISP_INTERVAL_FLY		= 30.0;
const double	BALL_VEC_GUIDE_DISP_DOT_MOVE			=  0.0;
const int		BALL_VEC_GUIDE_DISP_DOT_N				=   10;
const int		BALL_VEC_GUIDE_DISP_DOT_N_FLY			= 1000;
const int		BALL_VEC_GUIDE_DISP_BOUND_N				=    3;
//const double	BALL_VEC_GUIDE_DISP_POWER_NORMAL		= 20.0;	// SHOT_BASE_MAX_POWER / 3;
//const double	BALL_VEC_GUIDE_DISP_POWER_FULL			= 40.0;	// SHOT_BASE_MAX_POWER * 2 / 3;
//const double	BALL_VEC_GUIDE_DISP_POWER_FLY			= 60.0;	// SHOT_BASE_MAX_POWER
const double	BALL_VEC_GUIDE_DISP_POWER				= 60.0;	// SHOT_BASE_MAX_POWER

// �o���p�[
const double	FLY_BUMPER_HIT_ADD_SPEED_RATE			= 1.025;
const int		HOLE_BUMPER_Z						=	5;

// ����΂�{�^��
const int		FLYING_GOOD_LUCK_UP_BEFORE_FRAME					= 3;
const int		FLYING_GOOD_LUCK_UP_AFTER_FRAME						= 3;

const double	FLY_GOOD_LUCK_TIMING_JUDGE_SPEED_BUMPER		=	5.0;	// ����΂�{�^�����^�C�~���O�ǂ��o���p�[���˕Ԃ�̏u�Ԃɉ������ꍇ�ɐݒ肷�鑬�x��臒l
const double	FLY_GOOD_LUCK_TIMING_SET_SPEED_BUMPER		=	6.0;	// ����΂�{�^�����^�C�~���O�ǂ��o���p�[���˕Ԃ�̏u�Ԃɉ������ꍇ�ɐݒ肷�鑬�x�A臒l�����͂���ɂȂ�
const double	FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BUMPER	=	1.100;	// ����΂�{�^���̉����ň��ȏ�̑��x�̏ꍇ�̑��x�ɂ�����{��
const double	FLY_GOOD_LUCK_TIMING_UP_SPEED_MUL_BOUND		=	1.200;	// ����΂�{�^�����^�C�~���O�ǂ��n�ʂ𒵂˕Ԃ�u�Ԃɉ������ꍇ�̑��x��z�����x�ɂ�����{��
const double	FLY_GOOD_LUCK_UP_SPEED_MUL					=	1.020;	// ����΂�{�^�����ђ��ɉ��������̑��x�ɂ�����{��

// ����
const double	FLY_FALL_Z	=	-1024.0;									// ���������ɂȂ�z�ʒu

// �n�ʂ̉��n
const double	FLY_GROUND_SIDE_G_RATE					=	0.750;			// �n�ʂ̉��ɂ����Ԃŉ��ɗ����鎞��G�̂����銄�荇��
const double	FLY_GROUND_SIDE_SPEED_UP_MAX_SPEED		=	2.000;			// �n�ʂ̉��ɂ����Ԃŉ��ɗ����鎞�̉�������ő呬�x�A����𒴂������x�ł͉������Ȃ�
const double	FLY_GROUND_SIDE_SPEED_UP_RATE			=	1.133;			// �n�ʂ̉��ɂ����Ԃŉ��ɗ����鎞�̉����̑��x�ɂ����銄�荇��

// ��
const double	FLY_SLOPE_CALC_BASE_SPEED			=	4.000;			// BASE_SPEED / (this->speed + 1.0) �ŗ����鎞�̉������v�Z����A���̑��x�������قǉ������Ȃ�
const double	FLY_SLOPE_BOUND_Z_SPEED_DIV			=	2.500;			// �o�E���h���č�̏ꍇ�ABASE_SPEED = BASE_SPEED * (Z_SPEED / Z_SPEED_DIV ���Ă���������v�Z����
const int		FLY_SLOPE_EFFECT_INTERVAL_FRAME		=	7;				// ��̌��ʂ́A INTERVAL_FRAME �ɂP��o��

// ���s�d
const double	FLY_PILLAR_SPEED_RATE				=	0.200;			// �����˕Ԃ莞�̑��x�̌W��
const int		HOLE_PILLAR_Z						=	120;
const int		HOLE_PILLAR_RADIUS					=	8;

// �W�����v
const double	FLY_JUMP_Z_SPEED_BASE				=   1.200;			// �W�����v��ɏ�������ɓK�p������z�X�s�[�h 
//const double	FLY_JUMP_Z_SPEED_MAX				=   9.000;			// �W�����v��ɏ�������ɓK�p������z�X�s�[�h 
const double	FLY_JUMP_SPEED_BASE					=	0.500;			// �W�����v��ɏ�������ɓK�p������X�s�[�h 
//const double	FLY_JUMP_SPEED_MAX					=	4.500;			// �W�����v��ɏ�������ɓK�p������X�s�[�h 

// �R���x�A
const double	FLY_CONVEYOR_MAX_SPEED				=	2.500;			// �R���x���[��K�p�����锻��̃X�s�[�h�ő�l
const double	FLY_CONVEYOR_SPEED					=	1.500;			// �R���x���[�ɏ�������ɓK�p������X�s�[�h
const double	FLY_CONVEYOR_ESCAPE_SPEED			=	2.000;			// �R���x���[�͈͂��o�鎞�ɓK�p������X�s�[�h

// �����グ
const double	FLY_FUKIAGE_Z_SPEED					=	0.300;			// �����グ�G���A�ɏ�������ɓK�p������z�X�s�[�h
const double	FLY_FUKIAGE_Z_SPEED_PRESS			=	0.030;			// �����グ�G���A�ɏ�������ɓK�p������z�X�s�[�h(�v���X���́A�����ʂ��傫��)

// ��W�����v�E���W�����v
const double	FLY_JUMP_WEAK_Z_SPEED_BASE			=	1.25;			// ���W�����v��ɏ�������ɓK�p������z�X�s�[�h��{�l 
const double	FLY_JUMP_WEAK_Z_SPEED_MAX			=	6.0;			// ���W�����v��ɏ�������ɓK�p������z�X�s�[�h�ő�l
const double	FLY_JUMP_STRONG_Z_SPEED				=	7.0;			// ��W�����v��ɏ�������ɓK�p������z�X�s�[�h 
//const double	FLY_JUMP_STRONG_SPEED_MAX			=	6.0;			// ��W�����v��ɏ�������̃X�s�[�hMAX

// �_�b�V��
const double	FLY_DASH_SPEED						=	12.500;			// �_�b�V���ɏ�������ɐݒ肷��X�s�[�h

// �o���J�[
const double	FLY_BUNKER_SPEED_RATE				=	0.880;			// �o���J�[�ɏ�������ɓK�p������X�s�[�h 

// �d�̓G���A
const double	FLY_GRAVITY_Z_SPEED_BASE			=	1.5;			// �d�͏�ɏ�������ɓK�p������z�X�s�[�h
const double	FLY_GRAVITY_Z_SPEED_MIN				=	-15.0;			// �d�͏�ɏ�������ɓK�p������z�X�s�[�h 

// �_���[�W�G���A
const double	FLY_DAMAGE_SPEED					=	3.0;			// �_���[�W�G���A�ɏ�������ɓK�p������X�s�[�h 
const double	FLY_DAMAGE_Z_SPEED					=	7.0;			// �_���[�W�G���A�ɏ�������ɓK�p������z�X�s�[�h 

// ���X�y�J
const double	FLY_MOMIJI_RELEASE_SPEED			=	6.000;
const double	FLY_MOMIJI_RELEASE_Z_SPEED			=	2.000;
const double	SPELL_MOMIJI_CURVE					=	3.000;			// �˖��ۃX�y���J�[�h�̗����𑀍삷��ۂ̉�]�p�x
const double	FIELD_TORNADO_DEC_SPEED_BASE		=	0.038;
const double	FLYING_TORNADO_UP_G					=	0.250;
const double	FLYING_TORNADO_DOWN_G				=	0.100;

// �V�q�X�y�J
const double	FLY_TENKO_RELEASE_SPEED					=	1.5;
const double	FLY_TENKO_RELEASE_Z_SPEED				=	0.0;
const double	FLYING_PRESS_G							=	0.010;
const double	FLY_GROUND_PRESS_SIDE_SPEED_UP_RATE		=	1.080;		// �y���y�����A�n�ʂ̉��ɂ����Ԃŉ��ɗ����鎞�̉����̑��x�ɂ����銄�荇��

// �ɂƂ�X�y�J
const double	FLY_NITORI_RELEASE_SPEED_MIN				=  6.0;
const double	FLY_NITORI_RELEASE_SPEED_BUMPER_MIN			= 11.0;
const double	FLY_NITORI_RELEASE_SPEED_BUMPER_RATE		= 1.25;

// ���ז��L����

//�_�R
const int		UNZAN_X_SHIFT							= -2;
const int		UNZAN_Y_SHIFT							= -24;
const int		UNZAN_Z_SHIFT							= 20;
const int		UNZAN_RADIUS							= 48;
const int		UNZAN_MOVE_BEFORE_WAIT_CNT				= 30;
const int		UNZAN_MOVE_SPEED						= 2;

// ����
const int		OKU_NORMAL_SCENE_DISP_FRAME_CNT		=	17;
const int		OKU_NORMAL_SCENE_DISP_N				=	3;
const int		OKU_NORMAL_2_ATTACK_FRAME_CNT		=	4 * nn::yb::e::global::OKU_NORMAL_SCENE_DISP_FRAME_CNT * nn::yb::e::global::OKU_NORMAL_SCENE_DISP_N;
const int		OKU_ATTACK_2_NORMAL_FRAME_CNT		=	60 * 2;

const int		OKU_Z_SHIFT							= 30;
const int		OKU_RADIUS							= 56;
const int		OKU_BULLET_RADIUS					= 32;

// ����
const double	ORIN_CLASH_AFTER_Z_SPEED			=	6.0;

// �L�b�N
const int		HOLE_KICK_HALF_W					=	48;
const int		HOLE_KICK_HALF_H					=	48;
const int		HOLE_WARP_HALF_W					=	48;
const int		HOLE_WARP_HALF_H					=	48;

// �{�[��
const int		BALL_RADIUS							=	24;

// z�l
const int		DISP_Z_SPELL_GET_CARD_KEY	=	-110;
const int		DISP_Z_SPELL_GET			=	-20;
const int		DISP_Z_KEY					=	-100;
const int		DISP_Z_NOW_LOADING_ANIME	=	-30;
const int		DISP_Z_NOW_LOADING_IMAGE	=	-25;
const int		DISP_Z_STATUS				=	-25;
const int		DISP_Z_SPELL_BALL			=	 175;
const int		DISP_Z_SHOT_GUIDE			=	 190;	// �K�C�h��+3�܂Ŏg��
const int		DISP_Z_BALL_DOWN_ENEMY		=	 194;
const int		DISP_Z_BALL_DOWN_OBJ		=	 195;
const int		DISP_Z_BALL_MASK_RELEASE	=	 199;
const int		DISP_Z_BALL					=	 200;
const int		DISP_Z_BALL_MASK			=	 201;
const int		DISP_Z_BALL_UP_ENEMY		=	 204;
const int		DISP_Z_BALL_UP_OBJ			=	 205;
const int		DISP_Z_OBJ_SHADOW			=	 250;
const int		DISP_Z_CUP					=	 252;
const int		DISP_Z_BUMPER				=	 254;
const int		DISP_Z_CONVEYOR				=	 256;
const int		DISP_Z_WARP					=	 256;
const int		DISP_Z_FUKIAGE				=	 258;
const int		DISP_Z_BUNKER				=	 258;
const int		DISP_Z_DAMAGE				=	 258;
const int		DISP_Z_SLOPE				=	 258;	// ���-1���g��
const int		DISP_Z_GROUND				=	 260;
const int		DISP_Z_FALL_BALL			=	 300;
const int		DISP_Z_BG					=	 500;

const int		STAGE_HP_MAX				=		4;

const int		EDIT_DATA_MAX					=	100;
const int		EDIT_DATA_DISP_DATA_PER_PAGE	=	20;
const int		EDIT_DATA_DISP_PAGE				=	(nn::yb::e::global::EDIT_DATA_MAX / nn::yb::e::global::EDIT_DATA_DISP_DATA_PER_PAGE);

} // namespece global
} // namespece e

} // namespece yb
} // namespece nn

#endif // __ENUM_H__

