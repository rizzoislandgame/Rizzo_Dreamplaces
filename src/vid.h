/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// vid.h -- video driver defs

#ifndef VID_H
#define VID_H

extern int cl_available;

typedef struct viddef_s
{
	// these are set by VID_Mode
	int width;
	int height;
	int bitsperpixel;
	int fullscreen;
	int refreshrate;
#ifndef NO_VIDEOSTEREO
	int stereobuffer;
#endif
} viddef_t;

// global video state
extern viddef_t vid;
extern void (*vid_menudrawfn)(void);
extern void (*vid_menukeyfn)(int key);

extern qboolean vid_hidden;
extern qboolean vid_activewindow;
extern cvar_t vid_hardwaregammasupported;
extern qboolean vid_usinghwgamma;
extern qboolean vid_supportrefreshrate;

extern cvar_t vid_fullscreen;
extern cvar_t vid_width;
extern cvar_t vid_height;
extern cvar_t vid_bitsperpixel;
extern cvar_t vid_refreshrate;
extern cvar_t vid_vsync;
extern cvar_t vid_mouse;
extern cvar_t vid_grabkeyboard;
extern cvar_t vid_minwidth;
extern cvar_t vid_minheight;

extern cvar_t gl_combine;
extern cvar_t gl_finish;

extern cvar_t v_gamma;
extern cvar_t v_contrast;
extern cvar_t v_brightness;
extern cvar_t v_color_enable;
extern cvar_t v_color_black_r;
extern cvar_t v_color_black_g;
extern cvar_t v_color_black_b;
extern cvar_t v_color_grey_r;
extern cvar_t v_color_grey_g;
extern cvar_t v_color_grey_b;
extern cvar_t v_color_white_r;
extern cvar_t v_color_white_g;
extern cvar_t v_color_white_b;
extern cvar_t v_hwgamma;

extern int gl_stencil;

// brand of graphics chip
extern const char *gl_vendor;
// graphics chip model and other information
extern const char *gl_renderer;
// begins with 1.0.0, 1.1.0, 1.2.0, 1.2.1, 1.3.0, 1.3.1, or 1.4.0
extern const char *gl_version;
// extensions list, space separated
#ifndef NO_GLEXTENSIONS
extern const char *gl_extensions;
// WGL, GLX, or AGL
extern const char *gl_platform;
// another extensions list, containing platform-specific extensions that are
// not in the main list
extern const char *gl_platformextensions;
#endif
// name of driver library (opengl32.dll, libGL.so.1, or whatever)
extern char gl_driver[64];

// compatibility hacks
extern qboolean isG200;
extern qboolean isRagePro;

// LordHavoc: GLX_SGI_swap_control and WGL_EXT_swap_control
extern int gl_videosyncavailable;

void *GL_GetProcAddress(const char *name);
int GL_CheckExtension(const char *name, const dllfunction_t *funcs, const char *disableparm, int silent);

void VID_Shared_Init(void);

void GL_Init (void);

void VID_CheckExtensions(void);

void VID_Init (void);
// Called at startup

void VID_Shutdown (void);
// Called at shutdown

int VID_SetMode (int modenum);
// sets the mode; only used by the Quake engine for resetting to mode 0 (the
// base mode) on memory allocation failures

int VID_InitMode(int fullscreen, int width, int height, int bpp, int refreshrate, int stereobuffer);
// allocates and opens an appropriate OpenGL context (and its window)


// sets hardware gamma correction, returns false if the device does not
// support gamma control
// (ONLY called by VID_UpdateGamma and VID_RestoreSystemGamma)
int VID_SetGamma(unsigned short *ramps, int rampsize);
// gets hardware gamma correction, returns false if the device does not
// support gamma control
// (ONLY called by VID_UpdateGamma and VID_RestoreSystemGamma)
int VID_GetGamma(unsigned short *ramps, int rampsize);
// makes sure ramp arrays are big enough and calls VID_GetGamma/VID_SetGamma
// (ONLY to be called from VID_Finish!)
void VID_UpdateGamma(qboolean force, int rampsize);
// turns off hardware gamma ramps immediately
// (called from various shutdown/deactivation functions)
void VID_RestoreSystemGamma(void);

void VID_Finish (qboolean allowmousegrab);

void VID_Restart_f(void);

void VID_Start(void);

#endif

