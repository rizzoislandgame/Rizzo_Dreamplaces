/*
Copyright (C) 2003  T. Joseph Carter

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
#undef WIN32_LEAN_AND_MEAN  //hush a warning, SDL.h redefines this
#include <SDL.h>
#include <stdio.h>

#include "quakedef.h"

#ifdef DEBUG_GL
#include "debug_gl.h"
#endif

// Tell startup code that we have a client
int cl_available = true;

qboolean vid_supportrefreshrate = false;

cvar_t joy_detected = {CVAR_READONLY, "joy_detected", "0", "number of joysticks detected by engine"};
cvar_t joy_enable = {0, "joy_enable", "1", "enables joystick support"};
cvar_t joy_index = {0, "joy_index", "0", "selects which joystick to use if you have multiple"};
cvar_t joy_axisforward = {0, "joy_axisforward", "1", "which joystick axis to query for forward/backward movement"};
cvar_t joy_axisside = {0, "joy_axisside", "0", "which joystick axis to query for right/left movement"};
cvar_t joy_axisup = {0, "joy_axisup", "-1", "which joystick axis to query for up/down movement"};
cvar_t joy_axispitch = {0, "joy_axispitch", "3", "which joystick axis to query for looking up/down"};
cvar_t joy_axisyaw = {0, "joy_axisyaw", "2", "which joystick axis to query for looking right/left"};
cvar_t joy_axisroll = {0, "joy_axisroll", "-1", "which joystick axis to query for tilting head right/left"};
cvar_t joy_deadzoneforward = {0, "joy_deadzoneforward", "0", "deadzone tolerance, suggested values are in the range 0 to 0.01"};
cvar_t joy_deadzoneside = {0, "joy_deadzoneside", "0", "deadzone tolerance, suggested values are in the range 0 to 0.01"};
cvar_t joy_deadzoneup = {0, "joy_deadzoneup", "0", "deadzone tolerance, suggested values are in the range 0 to 0.01"};
cvar_t joy_deadzonepitch = {0, "joy_deadzonepitch", "0", "deadzone tolerance, suggested values are in the range 0 to 0.01"};
cvar_t joy_deadzoneyaw = {0, "joy_deadzoneyaw", "0", "deadzone tolerance, suggested values are in the range 0 to 0.01"};
cvar_t joy_deadzoneroll = {0, "joy_deadzoneroll", "0", "deadzone tolerance, suggested values are in the range 0 to 0.01"};
cvar_t joy_sensitivityforward = {0, "joy_sensitivityforward", "-1", "movement multiplier"};
cvar_t joy_sensitivityside = {0, "joy_sensitivityside", "1", "movement multiplier"};
cvar_t joy_sensitivityup = {0, "joy_sensitivityup", "1", "movement multiplier"};
cvar_t joy_sensitivitypitch = {0, "joy_sensitivitypitch", "1", "movement multiplier"};
cvar_t joy_sensitivityyaw = {0, "joy_sensitivityyaw", "-1", "movement multiplier"};
cvar_t joy_sensitivityroll = {0, "joy_sensitivityroll", "1", "movement multiplier"};


static qboolean vid_usingmouse;
static qboolean vid_isfullscreen;
static int vid_numjoysticks = 0;
#define MAX_JOYSTICKS 8
static SDL_Joystick *vid_joysticks[MAX_JOYSTICKS];

static SDL_Surface *screen;

/////////////////////////
// Input handling
////
//TODO: Add joystick support
//TODO: Add error checking


//keysym to quake keysym mapping
#define tenoh	0,0,0,0,0, 0,0,0,0,0
#define fiftyoh tenoh, tenoh, tenoh, tenoh, tenoh
#define hundredoh fiftyoh, fiftyoh
static unsigned int tbl_sdltoquake[] =
{
	0,0,0,0,		//SDLK_UNKNOWN		= 0,
	0,0,0,0,		//SDLK_FIRST		= 0,
	K_BACKSPACE,	//SDLK_BACKSPACE	= 8,
	K_TAB,			//SDLK_TAB			= 9,
	0,0,
	0,				//SDLK_CLEAR		= 12,
	K_ENTER,		//SDLK_RETURN		= 13,
    0,0,0,0,0,
	K_PAUSE,		//SDLK_PAUSE		= 19,
	0,0,0,0,0,0,0,
	K_ESCAPE,		//SDLK_ESCAPE		= 27,
	0,0,0,0,
	K_SPACE,		//SDLK_SPACE		= 32,
	'!',			//SDLK_EXCLAIM		= 33,
	'"',			//SDLK_QUOTEDBL		= 34,
	'#',			//SDLK_HASH			= 35,
	'$',			//SDLK_DOLLAR		= 36,
	0,
	'&',			//SDLK_AMPERSAND	= 38,
	'\'',			//SDLK_QUOTE		= 39,
	'(',			//SDLK_LEFTPAREN	= 40,
	')',			//SDLK_RIGHTPAREN	= 41,
	'*',			//SDLK_ASTERISK		= 42,
	'+',			//SDLK_PLUS			= 43,
	',',			//SDLK_COMMA		= 44,
	'-',			//SDLK_MINUS		= 45,
	'.',			//SDLK_PERIOD		= 46,
	'/',			//SDLK_SLASH		= 47,
	'0',			//SDLK_0			= 48,
	'1',			//SDLK_1			= 49,
	'2',			//SDLK_2			= 50,
	'3',			//SDLK_3			= 51,
	'4',			//SDLK_4			= 52,
	'5',			//SDLK_5			= 53,
	'6',			//SDLK_6			= 54,
	'7',			//SDLK_7			= 55,
	'8',			//SDLK_8			= 56,
	'9',			//SDLK_9			= 57,
	':',			//SDLK_COLON		= 58,
	';',			//SDLK_SEMICOLON	= 59,
	'<',			//SDLK_LESS			= 60,
	'=',			//SDLK_EQUALS		= 61,
	'>',			//SDLK_GREATER		= 62,
	'?',			//SDLK_QUESTION		= 63,
	'@',			//SDLK_AT			= 64,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	'[',		//SDLK_LEFTBRACKET	= 91,
	'\\',		//SDLK_BACKSLASH	= 92,
	']',		//SDLK_RIGHTBRACKET	= 93,
	'^',		//SDLK_CARET		= 94,
	'_',		//SDLK_UNDERSCORE	= 95,
	'`',		//SDLK_BACKQUOTE	= 96,
	'a',		//SDLK_a			= 97,
	'b',		//SDLK_b			= 98,
	'c',		//SDLK_c			= 99,
	'd',		//SDLK_d			= 100,
	'e',		//SDLK_e			= 101,
	'f',		//SDLK_f			= 102,
	'g',		//SDLK_g			= 103,
	'h',		//SDLK_h			= 104,
	'i',		//SDLK_i			= 105,
	'j',		//SDLK_j			= 106,
	'k',		//SDLK_k			= 107,
	'l',		//SDLK_l			= 108,
	'm',		//SDLK_m			= 109,
	'n',		//SDLK_n			= 110,
	'o',		//SDLK_o			= 111,
	'p',		//SDLK_p			= 112,
	'q',		//SDLK_q			= 113,
	'r',		//SDLK_r			= 114,
	's',		//SDLK_s			= 115,
	't',		//SDLK_t			= 116,
	'u',		//SDLK_u			= 117,
	'v',		//SDLK_v			= 118,
	'w',		//SDLK_w			= 119,
	'x',		//SDLK_x			= 120,
	'y',		//SDLK_y			= 121,
	'z',		//SDLK_z			= 122,
	0,0,0,0,
	K_DEL, 		//SDLK_DELETE		= 127,
	hundredoh /*227*/, tenoh, tenoh, 0,0,0,0,0,0,0,0,
	K_KP_0,		//SDLK_KP0		= 256,
	K_KP_1,		//SDLK_KP1		= 257,
	K_KP_2,		//SDLK_KP2		= 258,
	K_KP_3,		//SDLK_KP3		= 259,
	K_KP_4,		//SDLK_KP4		= 260,
	K_KP_5,		//SDLK_KP5		= 261,
	K_KP_6,		//SDLK_KP6		= 262,
	K_KP_7,		//SDLK_KP7		= 263,
	K_KP_8,		//SDLK_KP8		= 264,
	K_KP_9,		//SDLK_KP9		= 265,
	K_KP_PERIOD,//SDLK_KP_PERIOD	= 266,
	K_KP_DIVIDE,//SDLK_KP_DIVIDE	= 267,
	K_KP_MULTIPLY,//SDLK_KP_MULTIPLY= 268,
	K_KP_MINUS,	//SDLK_KP_MINUS		= 269,
	K_KP_PLUS,	//SDLK_KP_PLUS		= 270,
	K_KP_ENTER,	//SDLK_KP_ENTER		= 271,
	K_KP_EQUALS,//SDLK_KP_EQUALS	= 272,
	K_UPARROW,	//SDLK_UP		= 273,
	K_DOWNARROW,//SDLK_DOWN		= 274,
	K_RIGHTARROW,//SDLK_RIGHT	= 275,
	K_LEFTARROW,//SDLK_LEFT		= 276,
	K_INS,		//SDLK_INSERT	= 277,
	K_HOME,		//SDLK_HOME		= 278,
	K_END,		//SDLK_END		= 279,
	K_PGUP, 	//SDLK_PAGEUP	= 280,
	K_PGDN,		//SDLK_PAGEDOWN	= 281,
	K_F1,		//SDLK_F1		= 282,
	K_F2,		//SDLK_F2		= 283,
	K_F3,		//SDLK_F3		= 284,
	K_F4,		//SDLK_F4		= 285,
	K_F5,		//SDLK_F5		= 286,
	K_F6,		//SDLK_F6		= 287,
	K_F7,		//SDLK_F7		= 288,
	K_F8,		//SDLK_F8		= 289,
	K_F9,		//SDLK_F9		= 290,
	K_F10,		//SDLK_F10		= 291,
	K_F11,		//SDLK_F11		= 292,
	K_F12,		//SDLK_F12		= 293,
	0,			//SDLK_F13		= 294,
	0,			//SDLK_F14		= 295,
	0,			//SDLK_F15		= 296,
	0,0,0,
	K_NUMLOCK,	//SDLK_NUMLOCK	= 300,
	K_CAPSLOCK,	//SDLK_CAPSLOCK	= 301,
	K_SCROLLOCK,//SDLK_SCROLLOCK= 302,
	K_SHIFT,	//SDLK_RSHIFT	= 303,
	K_SHIFT,	//SDLK_LSHIFT	= 304,
	K_CTRL,		//SDLK_RCTRL	= 305,
	K_CTRL,		//SDLK_LCTRL	= 306,
	K_ALT,		//SDLK_RALT		= 307,
	K_ALT,		//SDLK_LALT		= 308,
	0,			//SDLK_RMETA	= 309,
	0,			//SDLK_LMETA	= 310,
	0,			//SDLK_LSUPER	= 311,		/* Left "Windows" key */
	0,			//SDLK_RSUPER	= 312,		/* Right "Windows" key */
	K_ALT,			//SDLK_MODE		= 313,		/* "Alt Gr" key */
	0,			//SDLK_COMPOSE	= 314,		/* Multi-key compose key */
	0,			//SDLK_HELP		= 315,
	0,			//SDLK_PRINT	= 316,
	0,			//SDLK_SYSREQ	= 317,
	K_PAUSE,	//SDLK_BREAK	= 318,
	0,			//SDLK_MENU		= 319,
	0,			//SDLK_POWER	= 320,		/* Power Macintosh power key */
	'e',		//SDLK_EURO		= 321,		/* Some european keyboards */
	0			//SDLK_UNDO		= 322,		/* Atari keyboard has Undo */
};
#undef tenoh
#undef fiftyoh
#undef hundredoh

int SDL_Quake_MapKey( unsigned int sdlkey )
{
	if( sdlkey > sizeof(tbl_sdltoquake)/ sizeof(int) )
		return 0;
    return tbl_sdltoquake[ sdlkey ];
}

static void IN_Activate( qboolean grab )
{
	if (grab)
	{
		if (!vid_usingmouse)
		{
			vid_usingmouse = true;
			cl_ignoremousemove = true;
			SDL_WM_GrabInput( SDL_GRAB_ON );
			SDL_ShowCursor( SDL_DISABLE );
		}
	}
	else
	{
		if (vid_usingmouse)
		{
			vid_usingmouse = false;
			cl_ignoremousemove = true;
			SDL_WM_GrabInput( SDL_GRAB_OFF );
			SDL_ShowCursor( SDL_ENABLE );
		}
	}
}

static double IN_JoystickGetAxis(SDL_Joystick *joy, int axis, double sensitivity, double deadzone)
{
#ifndef MAX_AUTOEVENTS
	double value;
	if (axis < 0 || axis >= SDL_JoystickNumAxes(joy))
		return 0; // no such axis on this joystick
	value = SDL_JoystickGetAxis(joy, axis) * (1.0 / 32767.0);
	value = bound(-1, value, 1);
	if (dr_fabs(value) < deadzone)
		return 0; // within deadzone around center
	return value * sensitivity;
#else
	return 0.0;
#endif
}

void IN_Move( void )
{
#ifndef MAX_AUTOEVENTS
	int j;
	int x, y;
	if( vid_usingmouse )
	{
		SDL_GetRelativeMouseState( &x, &y );
		in_mouse_x = x;
		in_mouse_y = y;
	}
	if (vid_numjoysticks && joy_enable.integer && joy_index.integer >= 0 && joy_index.integer < vid_numjoysticks)
	{
		SDL_Joystick *joy = vid_joysticks[joy_index.integer];
		int numballs = SDL_JoystickNumBalls(joy);
		for (j = 0;j < numballs;j++)
		{
			SDL_JoystickGetBall(joy, j, &x, &y);
			in_mouse_x += x;
			in_mouse_y += y;
		}
		cl.cmd.forwardmove += IN_JoystickGetAxis(joy, joy_axisforward.integer, joy_sensitivityforward.value, joy_deadzoneforward.value) * cl_forwardspeed.value;
		cl.cmd.sidemove    += IN_JoystickGetAxis(joy, joy_axisside.integer, joy_sensitivityside.value, joy_deadzoneside.value) * cl_sidespeed.value;
		cl.cmd.upmove      += IN_JoystickGetAxis(joy, joy_axisup.integer, joy_sensitivityup.value, joy_deadzoneup.value) * cl_upspeed.value;
		cl.viewangles[0]   += IN_JoystickGetAxis(joy, joy_axispitch.integer, joy_sensitivitypitch.value, joy_deadzonepitch.value) * cl.realframetime * cl_pitchspeed.value;
		cl.viewangles[1]   += IN_JoystickGetAxis(joy, joy_axisyaw.integer, joy_sensitivityyaw.value, joy_deadzoneyaw.value) * cl.realframetime * cl_yawspeed.value;
		//cl.viewangles[2]   += IN_JoystickGetAxis(joy, joy_axisroll.integer, joy_sensitivityroll.value, joy_deadzoneroll.value) * cl.realframetime * cl_rollspeed.value;
	}
#endif
}

/////////////////////
// Message Handling
////

static int Sys_EventFilter( SDL_Event *event )
{
	//TODO: Add a quit query in linux, too - though linux user are more likely to know what they do
#ifdef WIN32
	if( event->type == SDL_QUIT && MessageBox( NULL, "Are you sure you want to quit?", "Confirm Exit", MB_YESNO | MB_SETFOREGROUND | MB_ICONQUESTION ) == IDNO )
		return 0;
	else
		return 1;
#else
	return 1;
#endif
}

static keynum_t buttonremap[18] =
{
	K_MOUSE1,
	K_MOUSE3,
	K_MOUSE2,
	K_MWHEELUP,
	K_MWHEELDOWN,
	K_MOUSE4,
	K_MOUSE5,
	K_MOUSE6,
	K_MOUSE7,
	K_MOUSE8,
	K_MOUSE9,
	K_MOUSE10,
	K_MOUSE11,
	K_MOUSE12,
	K_MOUSE13,
	K_MOUSE14,
	K_MOUSE15,
	K_MOUSE16,
};

void Sys_SendKeyEvents( void )
{
	static unsigned cuenta=0;
	static qboolean sound_active = true;
	SDL_Event event;

	if (!cuenta)
		while( SDL_PollEvent( &event ) )
			SDL_Delay(10);
			{ ; }

	while( SDL_PollEvent( &event ) )
		switch( event.type ) {
			case SDL_QUIT:
				Sys_Quit();
				break;
#ifndef MAX_AUTOEVENTS
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				Key_Event( SDL_Quake_MapKey( event.key.keysym.sym ), (char)event.key.keysym.unicode, (event.key.state == SDL_PRESSED) );
				break;
			case SDL_ACTIVEEVENT:
				if( event.active.state == SDL_APPACTIVE )
				{
					if( event.active.gain )
						vid_hidden = false;
					else
						vid_hidden = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				if (event.button.button <= 18)
					Key_Event( buttonremap[event.button.button - 1], 0, event.button.state == SDL_PRESSED );
				break;
			case SDL_JOYBUTTONDOWN:
				if (!joy_enable.integer)
					break; // ignore down events if joystick has been disabled
			case SDL_JOYBUTTONUP:
				if (event.jbutton.button < 48)
					Key_Event( event.jbutton.button + (event.jbutton.button < 16 ? K_JOY1 : K_AUX1 - 16), 0, (event.jbutton.state == SDL_PRESSED) );
				break;
#endif
		}

#ifdef MAX_AUTOEVENTS
// Simple autoevents..
//	if ((cuenta&63)==10)
		Key_Event( K_UPARROW, 0, 1);
#endif

#if defined(DEBUG_DREAMPLACES) && defined(START_DEBUG)
	if (cuenta>START_DEBUG)
	{
#ifdef DEBUG_FILE
		if (!DEBUG_STR_FILE)
			DEBUG_STR_FILE=fopen(DEBUG_FILE,"wt");
#endif
		DEBUG_AHORA=1;
	}
#endif

#ifdef RESET_PROFILER
	if (cuenta==RESET_PROFILER)
		dreamplaces_prof_reset();
#endif

#ifdef MAX_AUTOEVENTS
	if (cuenta>MAX_AUTOEVENTS)
	{
		int i;
#ifdef DEBUG_FILE
		fclose(DEBUG_STR_FILE);
		SDL_Delay(500);
#endif
		dreamplaces_prof_show();
		exit(0);

	}
	else
		dbgf("auto-event %i\n",cuenta);

	vid_activewindow=true;
#else
	// enable/disable sound on focus gain/loss
	if (!vid_activewindow && sound_active)
	{
		S_BlockSound ();
		sound_active = false;
	}
	else if (vid_activewindow && !sound_active)
	{
		S_UnblockSound ();
		sound_active = true;
	}
#endif
	cuenta++;
}

/////////////////
// Video system
////

void *GL_GetProcAddress(const char *name)
{
	void *p = NULL;
	p = SDL_GL_GetProcAddress(name);
	return p;
}

static int Sys_EventFilter( SDL_Event *event );
static qboolean vid_sdl_initjoysticksystem = false;
void VID_Init (void)
{
	Cvar_RegisterVariable(&joy_detected);
	Cvar_RegisterVariable(&joy_enable);
	Cvar_RegisterVariable(&joy_index);
	Cvar_RegisterVariable(&joy_axisforward);
	Cvar_RegisterVariable(&joy_axisside);
	Cvar_RegisterVariable(&joy_axisup);
	Cvar_RegisterVariable(&joy_axispitch);
	Cvar_RegisterVariable(&joy_axisyaw);
	//Cvar_RegisterVariable(&joy_axisroll);
	Cvar_RegisterVariable(&joy_deadzoneforward);
	Cvar_RegisterVariable(&joy_deadzoneside);
	Cvar_RegisterVariable(&joy_deadzoneup);
	Cvar_RegisterVariable(&joy_deadzonepitch);
	Cvar_RegisterVariable(&joy_deadzoneyaw);
	//Cvar_RegisterVariable(&joy_deadzoneroll);
	Cvar_RegisterVariable(&joy_sensitivityforward);
	Cvar_RegisterVariable(&joy_sensitivityside);
	Cvar_RegisterVariable(&joy_sensitivityup);
	Cvar_RegisterVariable(&joy_sensitivitypitch);
	Cvar_RegisterVariable(&joy_sensitivityyaw);
	//Cvar_RegisterVariable(&joy_sensitivityroll);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		Sys_Error ("Failed to init SDL video subsystem: %s", SDL_GetError());
	vid_sdl_initjoysticksystem = SDL_Init(SDL_INIT_JOYSTICK) >= 0;
	if (vid_sdl_initjoysticksystem)
		Con_Printf("Failed to init SDL joystick subsystem: %s\n", SDL_GetError());
	vid_isfullscreen = false;
	{ SDL_Event e; while(SDL_PollEvent(&e)) SDL_Delay(10); }
}

// set the icon (we dont use SDL here since it would be too much a PITA)
#ifdef WIN32
#include "resource.h"
#include <SDL_syswm.h>
static void VID_SetCaption()
{
    SDL_SysWMinfo	info;
	HICON			icon;

#ifndef NO_GAMEMODE
	// set the caption
	SDL_WM_SetCaption( gamename, NULL );
#endif

	// get the HWND handle
    SDL_VERSION( &info.version );
	if( !SDL_GetWMInfo( &info ) )
		return;

	icon = LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDI_ICON1 ) );
#ifndef _W64 //If Windows 64bit data types don't exist
#define SetClassLongPtr SetClassLong
#define GCLP_HICON GCL_HICON
#define LONG_PTR LONG
#endif
	SetClassLongPtr( info.window, GCLP_HICON, (LONG_PTR)icon );
}
#else
static void VID_SetCaption()
{
#ifndef NO_GAMEMODE
	SDL_WM_SetCaption( gamename, NULL );
#endif
}
#endif

static void VID_OutputVersion()
{
	const SDL_version *version;
	version = SDL_Linked_Version();
	Con_Printf(	"Linked against SDL version %d.%d.%d\n"
					"Using SDL library version %d.%d.%d\n",
					SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL,
					version->major, version->minor, version->patch );
}

int VID_InitMode(int fullscreen, int width, int height, int bpp, int refreshrate, int stereobuffer)
{
	int i;
	static int notfirstvideomode = false;
	int flags = SDL_OPENGL;
	const char *drivername;

	VID_OutputVersion();

	/*
	SDL Hack
		We cant switch from one OpenGL video mode to another.
		Thus we first switch to some stupid 2D mode and then back to OpenGL.
	*/
	if (notfirstvideomode)
		SDL_SetVideoMode( 0, 0, 0, 0 );
	{ SDL_Event e; while(SDL_PollEvent(&e)) SDL_Delay(10); }
	notfirstvideomode = true;

	// SDL usually knows best
	drivername = NULL;

// COMMANDLINEOPTION: SDL GL: -gl_driver <drivername> selects a GL driver library, default is whatever SDL recommends, useful only for 3dfxogl.dll/3dfxvgl.dll or fxmesa or similar, if you don't know what this is for, you don't need it
	i = COM_CheckParm("-gl_driver");
	if (i && i < com_argc - 1)
		drivername = com_argv[i + 1];
#ifndef FIXED_OPENGL
	if (SDL_GL_LoadLibrary(drivername) < 0)
	{
		Con_Printf("Unable to load GL driver \"%s\": %s\n", drivername, SDL_GetError());
		return false;
	}

	if ((qglGetString = (const GLubyte* (GLAPIENTRY *)(GLenum name))GL_GetProcAddress("glGetString")) == NULL)
	{
		VID_Shutdown();
		Con_Print("Required OpenGL function glGetString not found\n");
		return false;
	}
#endif

	// Knghtbrd: should do platform-specific extension string function here

	vid_isfullscreen = false;
	if (fullscreen) {
		flags |= SDL_FULLSCREEN;
		vid_isfullscreen = true;
	}

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
	if (bpp >= 32)
	{
		SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute (SDL_GL_STENCIL_SIZE, 8);
	}
	else
	{
		SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 1);
		SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 1);
		SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 1);
		SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
	}
#ifndef NO_VIDEOSTEREO
	if (stereobuffer)
		SDL_GL_SetAttribute (SDL_GL_STEREO, 1);
#endif

	screen = SDL_SetVideoMode(width, height, bpp, flags);
	{ SDL_Event e; while(SDL_PollEvent(&e)) SDL_Delay(10); }
	if (screen == NULL)
	{
		Con_Printf("Failed to set video mode to %ix%i: %s\n", width, height, SDL_GetError());
		VID_Shutdown();
		return false;
	}

	// set window title
	VID_SetCaption();
	// set up an event filter to ask confirmation on close button in WIN32
	SDL_SetEventFilter( (SDL_EventFilter) Sys_EventFilter );
	// init keyboard
	SDL_EnableUNICODE( SDL_ENABLE );
	// enable key repeat since everyone expects it
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	gl_renderer = (const char *)qglGetString(GL_RENDERER);
	gl_vendor = (const char *)qglGetString(GL_VENDOR);
	gl_version = (const char *)qglGetString(GL_VERSION);
#ifndef NO_GLEXTENSIONS
	gl_extensions = (const char *)qglGetString(GL_EXTENSIONS);
	gl_platform = "SDL";
	// Knghtbrd: should assign platform-specific extensions here
	//TODO: maybe ;)
	gl_platformextensions = "";
#endif
	gl_videosyncavailable = false;

	GL_Init();

	vid_numjoysticks = SDL_NumJoysticks();
	vid_numjoysticks = bound(0, vid_numjoysticks, MAX_JOYSTICKS);
	Cvar_SetValueQuick(&joy_detected, vid_numjoysticks);
	Con_Printf("%d SDL joystick(s) found:\n", vid_numjoysticks);
	memset(vid_joysticks, 0, sizeof(vid_joysticks));
	for (i = 0;i < vid_numjoysticks;i++)
	{
		SDL_Joystick *joy;
		joy = vid_joysticks[i] = SDL_JoystickOpen(i);
		if (!joy)
		{
			Con_Printf("joystick #%i: open failed: %s\n", i, SDL_GetError());
			continue;
		}
		Con_Printf("joystick #%i: opened \"%s\" with %i axes, %i buttons, %i balls\n", i, SDL_JoystickName(i), (int)SDL_JoystickNumAxes(joy), (int)SDL_JoystickNumButtons(joy), (int)SDL_JoystickNumBalls(joy));
	}
#ifdef DREAMCAST
	qglEnable(GL_KOS_NEARZ_CLIPPING);
#endif

	vid_hidden = false;
	vid_activewindow = true; //false;
	vid_usingmouse = true; //false;

	return true;
}

void VID_Shutdown (void)
{
	// this is needed to retry gamma after a vid_restart
	VID_RestoreSystemGamma();

	IN_Activate(false);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

int VID_SetGamma (unsigned short *ramps, int rampsize)
{
	return !SDL_SetGammaRamp (ramps, ramps + rampsize, ramps + rampsize*2);
}

int VID_GetGamma (unsigned short *ramps, int rampsize)
{
	return !SDL_GetGammaRamp (ramps, ramps + rampsize, ramps + rampsize*2);
}

void VID_Finish (qboolean allowmousegrab)
{
	Uint8 appstate;
	qboolean vid_usemouse;

#ifndef DREAMCAST
#ifdef MAX_AUTOEVENTS
	//react on appstate changes
	appstate = SDL_GetAppState();

	vid_hidden = !(appstate & SDL_APPACTIVE);

	if( vid_hidden || !( appstate & SDL_APPMOUSEFOCUS ) || !( appstate & SDL_APPINPUTFOCUS ) )
		vid_activewindow = false;
	else
		vid_activewindow = true;

	vid_usemouse = false;
	if( allowmousegrab && vid_mouse.integer && !key_consoleactive
#ifndef NO_DEMO
	    && (key_dest != key_game || !cls.demoplayback)
#endif
	  )
		vid_usemouse = true;
	if( vid_isfullscreen )
		vid_usemouse = true;
	if( !vid_activewindow )
		vid_usemouse = false;
#else
	vid_hidden = false;
	vid_activewindow = true;
	vid_usemouse = true;
#endif
#endif

	IN_Activate(vid_usemouse);

	VID_UpdateGamma(false, 256);

	if (r_render.integer && !vid_hidden)
	{
		CHECKGLERROR
		if (
#ifndef VIEW_SIMPLE
			r_speeds.integer ||
#endif
		       	gl_finish.integer)
		{
			qglFinish();CHECKGLERROR
		}
#ifdef DEBUG_GL
		debug_SDL_GL_SwapBuffers();
#else
		SDL_GL_SwapBuffers();
#endif
	}
}
