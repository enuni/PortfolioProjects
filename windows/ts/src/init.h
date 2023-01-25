
#ifndef __INIT_H__
#define __INIT_H__

#include "type.h"

void initSystem( void);
void initMain( void);
void initRand( void);
BOOL initCardEject( void);
void initVBlankIntr( void);
void initCardSetThreadPriority( u32 priority);

#endif //__INIT_H__

