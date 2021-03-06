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

#ifndef CONSOLE_H
#define CONSOLE_H

#ifndef NO_CONSOLE
//
// console
//
extern int con_totallines;
extern int con_backscroll;
extern qboolean con_initialized;

extern qboolean rcon_redirect;
extern int rcon_redirect_bufferpos;
extern char rcon_redirect_buffer[1400];

void Con_CheckResize (void);
void Con_Init (void);
void Con_Init_Commands (void);
void Con_DrawConsole (int lines);
void Con_Print(const char *txt);
void Con_Printf(const char *fmt, ...) DP_FUNC_PRINTF(1);
#ifndef NO_DEVELOPER
void Con_DPrint(const char *msg);
void Con_DPrintf(const char *fmt, ...) DP_FUNC_PRINTF(1);
#else
#define Con_DPrint(MSG)
#define Con_DPrintf(FMT, RESTO...)
#endif
void Con_Clear_f (void);
void Con_DrawNotify (void);
void Con_ClearNotify (void);
void Con_ToggleConsole_f (void);

qboolean GetMapList (const char *s, char *completedname, int completednamebufferlength);

// wrapper function to attempt to either complete the command line
// or to list possible matches grouped by type
// (i.e. will display possible variables, aliases, commands
// that match what they've typed so far)
void Con_CompleteCommandLine(void);

// Generic libs/util/console.c function to display a list
// formatted in columns on the console
void Con_DisplayList(const char **list);


//
// log
//
void Log_Init (void);
void Log_Close (void);
void Log_Start (void);

void Log_Printf(const char *logfilename, const char *fmt, ...) DP_FUNC_PRINTF(2);

#else

#define Con_CheckResize()
#define Con_Init()
#define Con_Init_Commands()
#define Con_DrawConsole(l)
#define Con_Print(TXT)
#define Con_Printf(FMT, RESTO...)
#define Con_DPrint(MSG)
#define Con_DPrintf(FMT, RESTO...)
#define Con_Clear_f()
#define Con_DrawNotify()
#define Con_ClearNotify()
#define Con_ToggleConsole_f()
#define Con_DisplayList(LST)
#define Con_CompleteCommandLine()
#define GetMapList(S,CN,CBL) (0)
#define con_initialized (1)

#define Log_Init()
#define Log_Close()
#define Log_Start()

#define Log_Printf(LFN, FMT, RESTO...)

#endif


#endif

