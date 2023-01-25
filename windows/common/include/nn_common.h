#ifndef __N_COMMON_H__
#define __N_COMMON_H__

#pragma warning(disable:4996)	// sprintf

//-----------------------------------------------------------------------------
/*
//デバッグ debug
//#define PRINTF _printf
//#define PRINTF printf
#define PRINTF _printf0

#define PRINTF_INFO _printf
//#define PRINTF_INFO printf
//#define PRINTF_INFO _printf0

//#define PRINTF_ERROR _printf
//#define PRINTF_ERROR printf
#define PRINTF_ERROR _printf0

//define PRINTF_COM _printf
//#define PRINTF_COM _printf
#define PRINTF_COM _printf0
*/
//-----------------------------------------------------------------------------

#define BIT_ON( a, n) ( (a) |= (1 << (n)) )
#define BIT_OFF( a, n) ( (a) &= ~(1 << (n)) )
#define IS_BIT( a, n) ( (a) & (1 << (n)) )
#define BIT_ALL_ON(a) ( (a) |= ~0)
#define BIT_ALL_OFF(a) ( (a) = 0)

#define NN_COMMON_GET_ARRAY_SIZE( a) ( sizeof( a) / sizeof( a[0]) )

//-----------------------------------------------------------------------------

extern int gLogViewLevel;

#ifndef RVL
#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus
#endif //RVL

int n_commonGetFigure( unsigned long n);
int n_commonGetOneFigure( int n, int getFigure);
int n_commonGetFirstOnBitNum( int a);
int n_commonGetOnBitsNum( int a);
void n_commonMazeru( int * target, int n);
void n_commonRandInitCpu( unsigned int seed);
int n_commonRandCpu( void);
int n_commonRandNCpu( int max);
void n_commonRandInitDirect( unsigned int seed);
int n_commonRandDirect( void);
int n_commonRandNDirect( int max);
void n_commonRandInit( unsigned int seed);
int n_commonRand( void);
int n_commonRandN( int max);
int n_commonTime( void);
int n_commonRandFork( int * forkArray, int forkN);
int nn_commonRandBool( int percent);	//percentの確率でTRUE(0以外)を返す
#ifndef __TTG_WII_CHILD__
#ifndef __TTG_DS__
#ifndef __KAIJI_DS__
void _printf( char * format, ...);
#endif //__KAIJI_DS__
#endif //__TTG_DS__
#endif //__TTG_WII_CHILD__
void _printf0( char * format, ...);
void nn_commonLogInit( void);
void nn_commonLogPrintf( char * format, ...);
void nn_commonLogEnd( void);
void nn_commonLog( int level, char * format, ...);

#ifndef RVL
#ifdef __cplusplus
}
#endif //__cplusplus
#endif //RVL

namespace nn {
namespace common {
namespace e {
enum LOG_DST_INDEX {
	LOG_DST_INDEX_MAX = 8,
};
enum LOG_DST_INDEX_COMMON {
	LOG_DST_INDEX_COMMON_SAVE_MANAGER,

	LOG_DST_INDEX_COMMON_MAX
};
#define NN_COMMON_E_LOG_KIND_COMMON_SAVE	(0x0000000000000001)
#define NN_COMMON_E_LOG_KIND_COMMON_LOAD	(0x0000000000000002)
#define NN_COMMON_E_LOG_KIND_COMMON_TASK	(0x0000000000000004)
} // e;
void logInit( int dstIndex, char * fileName);
void logEnd( int dstIndex);
void log( int dstIndex, unsigned long long int kind, char * format, ...);
void setLogKind( unsigned long long int kind);
} // namesapce common
} // namespace nn

//-----------------------------------------------------------------------------
#endif	//__N_COMMON_H__

