#ifndef __VRAM_CTRL_H__
#define __VRAM_CTRL_H__

enum {
	VRAM_CTRL_STATE_NONE,
	VRAM_CTRL_STATE_LOAD,
	VRAM_CTRL_STATE_ACTIVE,
	
	VRAM_CTRL_STATE_MAX
};

//---------------------------------------------------------------------------

void vram_ctrlTask( void);

#endif	//__VRAM_CTRL_H__

