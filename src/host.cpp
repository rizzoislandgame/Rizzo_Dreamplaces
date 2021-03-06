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
// host.c -- coordinates spawning and killing of local servers

#include <time.h>
#include "quakedef.h"
#include "libcurl.h"
#include "cdaudio.h"
#include "cl_video.h"
#include "progsvm.h"
#include "csprogs.h"

/*

A server can always be started, even if the system started out as a client
to a remote system.

A client can NOT be started if the system started as a dedicated server.

Memory is cleared / released when a server or client begins, not when they end.

*/

// how many frames have occurred
// (checked by Host_Error and Host_SaveConfig_f)
int host_framecount = 0;
// LordHavoc: set when quit is executed
qboolean host_shuttingdown = false;

// the real time since application started, without any slowmo or clamping
double realtime;

// current client
client_t *host_client;

#if 0
jmp_buf host_abortframe;
#endif

// random seed
cvar_t sv_random_seed = {0, "sv_random_seed", "", "random seed; when set, on every map start this random seed is used to initialize the random number generator. Don't touch it unless for benchmarking or debugging"};

// pretend frames take this amount of time (in seconds), 0 = realtime
cvar_t host_framerate = {0, "host_framerate","0", "locks frame timing to this value in seconds, 0.05 is 20fps for example, note that this can easily run too fast, use cl_maxfps if you want to limit your framerate instead, or sys_ticrate to limit server speed"};
// shows time used by certain subsystems
cvar_t host_speeds = {0, "host_speeds","0", "reports how much time is used in server/graphics/sound"};
// LordHavoc: framerate independent slowmo
cvar_t slowmo = {0, "slowmo", "1.0", "controls game speed, 0.5 is half speed, 2 is double speed"};
// LordHavoc: framerate upper cap
cvar_t cl_maxfps = {CVAR_SAVE, "cl_maxfps", "1000", "maximum fps cap, if game is running faster than this it will wait before running another frame (useful to make cpu time available to other programs)"};

// print broadcast messages in dedicated mode
cvar_t sv_echobprint = {CVAR_SAVE, "sv_echobprint", "1", "prints gamecode bprint() calls to server console"};

cvar_t sys_ticrate = {CVAR_SAVE, "sys_ticrate","0.05", "how long a server frame is in seconds, 0.05 is 20fps server rate, 0.1 is 10fps (can not be set higher than 0.1), 0 runs as many server frames as possible (makes games against bots a little smoother, overwhelms network players)"};
cvar_t sv_fixedframeratesingleplayer = {0, "sv_fixedframeratesingleplayer", "0", "allows you to use server-style timing system in singleplayer (don't run faster than sys_ticrate)"};

cvar_t fraglimit = {CVAR_NOTIFY, "fraglimit","0", "ends level if this many frags is reached by any player"};
cvar_t timelimit = {CVAR_NOTIFY, "timelimit","0", "ends level at this time (in minutes)"};
cvar_t teamplay = {CVAR_NOTIFY, "teamplay","0", "teamplay mode, values depend on mod but typically 0 = no teams, 1 = no team damage no self damage, 2 = team damage and self damage, some mods support 3 = no team damage but can damage self"};

cvar_t samelevel = {CVAR_NOTIFY, "samelevel","0", "repeats same level if level ends (due to timelimit or someone hitting an exit)"};
cvar_t noexit = {CVAR_NOTIFY, "noexit","0", "kills anyone attempting to use an exit"};

#ifndef NO_DEVELOPER
cvar_t developer = {0, "developer","0", "prints additional debugging messages and information (recommended for modders and level designers)"};
cvar_t developer_entityparsing = {0, "developer_entityparsing", "0", "prints detailed network entities information each time a packet is received"};
#endif

cvar_t skill = {0, "skill","1", "difficulty level of game, affects monster layouts in levels, 0 = easy, 1 = normal, 2 = hard, 3 = nightmare (same layout as hard but monsters fire twice)"};
cvar_t deathmatch = {0, "deathmatch","0", "deathmatch mode, values depend on mod but typically 0 = no deathmatch, 1 = normal deathmatch with respawning weapons, 2 = weapons stay (players can only pick up new weapons)"};
cvar_t coop = {0, "coop","0", "coop mode, 0 = no coop, 1 = coop mode, multiple players playing through the singleplayer game (coop mode also shuts off deathmatch)"};

cvar_t pausable = {0, "pausable","1", "allow players to pause or not"};

cvar_t temp1 = {0, "temp1","0", "general cvar for mods to use, in stock id1 this selects which death animation to use on players (0 = random death, other values select specific death scenes)"};

cvar_t timestamps = {CVAR_SAVE, "timestamps", "0", "prints timestamps on console messages"};
cvar_t timeformat = {CVAR_SAVE, "timeformat", "[%Y-%m-%d %H:%M:%S] ", "time format to use on timestamped console messages"};

cvar_t sv_checkforpacketsduringsleep = {0, "sv_checkforpacketsduringsleep", "0", "uses select() function to wait between frames which can be interrupted by packets being received, instead of Sleep()/usleep()/SDL_Sleep() functions which do not check for packets"};

/*
================
Host_AbortCurrentFrame

aborts the current host frame and goes on with the next one
================
*/
void Host_AbortCurrentFrame(void)
{
#if 0
	longjmp (host_abortframe, 1);
#endif
}

/*
================
Host_Error

This shuts down both the client and server
================
*/
#ifndef NO_CONSOLE
void Host_Error (const char *error, ...)
{
	static char hosterrorstring1[MAX_INPUTLINE];
	static char hosterrorstring2[MAX_INPUTLINE];
	static qboolean hosterror = false;
	va_list argptr;

	// turn off rcon redirect if it was active when the crash occurred
	rcon_redirect = false;

	va_start (argptr,error);
	dpvsnprintf (hosterrorstring1,sizeof(hosterrorstring1),error,argptr);
	va_end (argptr);

	Con_Printf("Host_Error: %s\n", hosterrorstring1);

	// LordHavoc: if crashing very early, or currently shutting down, do
	// Sys_Error instead
	if (host_framecount < 3 || host_shuttingdown)
		Sys_Error ("Host_Error: %s", hosterrorstring1);

	if (hosterror)
		Sys_Error ("Host_Error: recursively entered (original error was: %s    new error is: %s)", hosterrorstring2, hosterrorstring1);
	hosterror = true;

	strlcpy(hosterrorstring2, hosterrorstring1, sizeof(hosterrorstring2));

	CL_Parse_DumpPacket();

	CL_Parse_ErrorCleanUp();

	//PR_Crash();

	// print out where the crash happened, if it was caused by QC (and do a cleanup)
	PRVM_Crash();


	Host_ShutdownServer ();

	if (cls.state == ca_dedicated)
		Sys_Error ("Host_Error: %s",hosterrorstring2);	// dedicated servers exit

	CL_Disconnect ();
#ifndef NO_DEMO
	cls.demonum = -1;
#endif

	hosterror = false;

	Host_AbortCurrentFrame();
}
#endif

void Host_ServerOptions (void)
{
	int i;

	// general default
	svs.maxclients = 8;

// COMMANDLINEOPTION: Server: -dedicated [playerlimit] starts a dedicated server (with a command console), default playerlimit is 8
// COMMANDLINEOPTION: Server: -listen [playerlimit] starts a multiplayer server with graphical client, like singleplayer but other players can connect, default playerlimit is 8
	// if no client is in the executable or -dedicated is specified on
	// commandline, start a dedicated server
	i = COM_CheckParm ("-dedicated");
	if (i || !cl_available)
	{
		cls.state = ca_dedicated;
		// check for -dedicated specifying how many players
		if (i && i + 1 < com_argc && atoi (com_argv[i+1]) >= 1)
			svs.maxclients = atoi (com_argv[i+1]);
		if (COM_CheckParm ("-listen"))
			Con_Printf ("Only one of -dedicated or -listen can be specified\n");
		// default sv_public on for dedicated servers (often hosted by serious administrators), off for listen servers (often hosted by clueless users)
		Cvar_SetValue("sv_public", 1);
	}
	else if (cl_available)
	{
		// client exists and not dedicated, check if -listen is specified
		cls.state = ca_disconnected;
		i = COM_CheckParm ("-listen");
		if (i)
		{
			// default players unless specified
			if (i + 1 < com_argc && atoi (com_argv[i+1]) >= 1)
				svs.maxclients = atoi (com_argv[i+1]);
		}
		else
		{
			// default players in some games, singleplayer in most
#ifndef NO_GAMEMODE
			if (gamemode != GAME_GOODVSBAD2 && gamemode != GAME_NEXUIZ && gamemode != GAME_BATTLEMECH)
#endif
				svs.maxclients = 1;
		}
	}

	svs.maxclients = bound(1, svs.maxclients, MAX_SCOREBOARD);

	svs.clients = (client_t *)Mem_Alloc(sv_mempool, sizeof(client_t) * svs.maxclients);

	if (svs.maxclients > 1 && !deathmatch.integer && !coop.integer)
		Cvar_SetValueQuick(&deathmatch, 1);
}

/*
=======================
Host_InitLocal
======================
*/
void Host_SaveConfig_f(void);
void Host_LoadConfig_f(void);
static void Host_InitLocal (void)
{
	Cmd_AddCommand("saveconfig", Host_SaveConfig_f, "save settings to config.cfg immediately (also automatic when quitting)");
	Cmd_AddCommand("loadconfig", Host_LoadConfig_f, "reset everything and reload configs");

	Cvar_RegisterVariable (&sv_random_seed);
	Cvar_RegisterVariable (&host_framerate);
	Cvar_RegisterVariable (&host_speeds);
	Cvar_RegisterVariable (&slowmo);
	Cvar_RegisterVariable (&cl_maxfps);

	Cvar_RegisterVariable (&sv_echobprint);

	Cvar_RegisterVariable (&sys_ticrate);
	Cvar_RegisterVariable (&sv_fixedframeratesingleplayer);

	Cvar_RegisterVariable (&fraglimit);
	Cvar_RegisterVariable (&timelimit);
	Cvar_RegisterVariable (&teamplay);
	Cvar_RegisterVariable (&samelevel);
	Cvar_RegisterVariable (&noexit);
	Cvar_RegisterVariable (&skill);
#ifndef NO_DEVELOPER
	Cvar_RegisterVariable (&developer);
	Cvar_RegisterVariable (&developer_entityparsing);
#endif
	Cvar_RegisterVariable (&deathmatch);
	Cvar_RegisterVariable (&coop);

	Cvar_RegisterVariable (&pausable);

	Cvar_RegisterVariable (&temp1);

	Cvar_RegisterVariable (&timestamps);
	Cvar_RegisterVariable (&timeformat);

	Cvar_RegisterVariable (&sv_checkforpacketsduringsleep);
}


/*
===============
Host_SaveConfig_f

Writes key bindings and archived cvars to config.cfg
===============
*/
void Host_SaveConfig_f(void)
{
	qfile_t *f;

// dedicated servers initialize the host but don't parse and set the
// config.cfg cvars
	// LordHavoc: don't save a config if it crashed in startup
	if (host_framecount >= 3 && cls.state != ca_dedicated && !COM_CheckParm("-benchmark"))
	{
		f = FS_Open ("config.cfg", "wb", false, false);
		if (!f)
		{
			Con_Print("Couldn't write config.cfg.\n");
			return;
		}

		Key_WriteBindings (f);
		Cvar_WriteVariables (f);

		FS_Close (f);
	}
}


/*
===============
Host_LoadConfig_f

Resets key bindings and cvars to defaults and then reloads scripts
===============
*/
void Host_LoadConfig_f(void)
{
	// unlock the cvar default strings so they can be updated by the new default.cfg
	Cvar_UnlockDefaults();
	// reset cvars to their defaults, and then exec startup scripts again
	Cbuf_InsertText("cvar_resettodefaults_all;exec quake.rc\n");
}

/*
=================
SV_ClientPrint

Sends text across to be displayed
FIXME: make this just a stuffed echo?
=================
*/
void SV_ClientPrint(const char *msg)
{
	if (host_client->netconnection)
	{
		MSG_WriteByte(&host_client->netconnection->message, svc_print);
		MSG_WriteString(&host_client->netconnection->message, msg);
	}
}

/*
=================
SV_ClientPrintf

Sends text across to be displayed
FIXME: make this just a stuffed echo?
=================
*/
void SV_ClientPrintf(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAX_INPUTLINE];

	va_start(argptr,fmt);
	dpvsnprintf(msg,sizeof(msg),fmt,argptr);
	va_end(argptr);

	SV_ClientPrint(msg);
}

/*
=================
SV_BroadcastPrint

Sends text to all active clients
=================
*/
void SV_BroadcastPrint(const char *msg)
{
	int i;
	client_t *client;

	for (i = 0, client = svs.clients;i < svs.maxclients;i++, client++)
	{
		if (client->active && client->netconnection)
		{
			MSG_WriteByte(&client->netconnection->message, svc_print);
			MSG_WriteString(&client->netconnection->message, msg);
		}
	}

	if (sv_echobprint.integer && cls.state == ca_dedicated)
		Con_Print(msg);
}

/*
=================
SV_BroadcastPrintf

Sends text to all active clients
=================
*/
void SV_BroadcastPrintf(const char *fmt, ...)
{
	va_list argptr;
	char msg[MAX_INPUTLINE];

	va_start(argptr,fmt);
	dpvsnprintf(msg,sizeof(msg),fmt,argptr);
	va_end(argptr);

	SV_BroadcastPrint(msg);
}

/*
=================
Host_ClientCommands

Send text over to the client to be executed
=================
*/
void Host_ClientCommands(const char *fmt, ...)
{
	va_list argptr;
	char string[MAX_INPUTLINE];

	if (!host_client->netconnection)
		return;

	va_start(argptr,fmt);
	dpvsnprintf(string, sizeof(string), fmt, argptr);
	va_end(argptr);

	MSG_WriteByte(&host_client->netconnection->message, svc_stufftext);
	MSG_WriteString(&host_client->netconnection->message, string);
}

/*
=====================
SV_DropClient

Called when the player is getting totally kicked off the host
if (crash = true), don't bother sending signofs
=====================
*/
void SV_DropClient(qboolean crash)
{
	int i;
	Con_Printf("Client \"%s\" dropped\n", host_client->name);

	// make sure edict is not corrupt (from a level change for example)
	host_client->edict = PRVM_EDICT_NUM(host_client - svs.clients + 1);

	if (host_client->netconnection)
	{
		// free the client (the body stays around)
		if (!crash)
		{
			// LordHavoc: no opportunity for resending, so use unreliable 3 times
			unsigned char bufdata[8];
			sizebuf_t buf;
			memset(&buf, 0, sizeof(buf));
			buf.data = bufdata;
			buf.maxsize = sizeof(bufdata);
			MSG_WriteByte(&buf, svc_disconnect);
			NetConn_SendUnreliableMessage(host_client->netconnection, &buf, sv.protocol);
			NetConn_SendUnreliableMessage(host_client->netconnection, &buf, sv.protocol);
			NetConn_SendUnreliableMessage(host_client->netconnection, &buf, sv.protocol);
		}
		// break the net connection
		NetConn_Close(host_client->netconnection);
		host_client->netconnection = NULL;
	}

	// call qc ClientDisconnect function
	// LordHavoc: don't call QC if server is dead (avoids recursive
	// Host_Error in some mods when they run out of edicts)
	if (host_client->clientconnectcalled && sv.active && host_client->edict)
	{
		// call the prog function for removing a client
		// this will set the body to a dead frame, among other things
		int saveSelf = prog->globals.server->self;
		host_client->clientconnectcalled = false;
		prog->globals.server->self = PRVM_EDICT_TO_PROG(host_client->edict);
		PRVM_ExecuteProgram(prog->globals.server->ClientDisconnect, "QC function ClientDisconnect is missing");
		prog->globals.server->self = saveSelf;
	}

	// if a download is active, close it
	if (host_client->download_file)
	{
		Con_DPrintf("Download of %s aborted when %s dropped\n", host_client->download_name, host_client->name);
		FS_Close(host_client->download_file);
		host_client->download_file = NULL;
		host_client->download_name[0] = 0;
		host_client->download_expectedposition = 0;
		host_client->download_started = false;
	}

	// remove leaving player from scoreboard
	host_client->name[0] = 0;
	host_client->colors = 0;
	host_client->frags = 0;
	// send notification to all clients
	// get number of client manually just to make sure we get it right...
	i = host_client - svs.clients;
	MSG_WriteByte (&sv.reliable_datagram, svc_updatename);
	MSG_WriteByte (&sv.reliable_datagram, i);
	MSG_WriteString (&sv.reliable_datagram, host_client->name);
	MSG_WriteByte (&sv.reliable_datagram, svc_updatecolors);
	MSG_WriteByte (&sv.reliable_datagram, i);
	MSG_WriteByte (&sv.reliable_datagram, host_client->colors);
	MSG_WriteByte (&sv.reliable_datagram, svc_updatefrags);
	MSG_WriteByte (&sv.reliable_datagram, i);
	MSG_WriteShort (&sv.reliable_datagram, host_client->frags);

	// free the client now
	if (host_client->entitydatabase)
		EntityFrame_FreeDatabase(host_client->entitydatabase);
	if (host_client->entitydatabase4)
		EntityFrame4_FreeDatabase(host_client->entitydatabase4);
	if (host_client->entitydatabase5)
		EntityFrame5_FreeDatabase(host_client->entitydatabase5);

	if (sv.active)
	{
		// clear a fields that matter to DP_SV_CLIENTNAME and DP_SV_CLIENTCOLORS, and also frags
		PRVM_ED_ClearEdict(host_client->edict);
	}

	// clear the client struct (this sets active to false)
	memset(host_client, 0, sizeof(*host_client));

	// update server listing on the master because player count changed
	// (which the master uses for filtering empty/full servers)
	NetConn_Heartbeat(1);
}

/*
==================
Host_ShutdownServer

This only happens at the end of a game, not between levels
==================
*/
void Host_ShutdownServer(void)
{
	int i;

	Con_DPrintf("Host_ShutdownServer\n");

	if (!sv.active)
		return;

	NetConn_Heartbeat(2);
	NetConn_Heartbeat(2);

// make sure all the clients know we're disconnecting
	SV_VM_Begin();
	for (i = 0, host_client = svs.clients;i < svs.maxclients;i++, host_client++)
		if (host_client->active)
			SV_DropClient(false); // server shutdown
	SV_VM_End();

	NetConn_CloseServerPorts();

	sv.active = false;
//
// clear structures
//
	memset(&sv, 0, sizeof(sv));
	memset(svs.clients, 0, svs.maxclients*sizeof(client_t));
}


//============================================================================

/*
===================
Host_GetConsoleCommands

Add them exactly as if they had been typed at the console
===================
*/
void Host_GetConsoleCommands (void)
{
	char *cmd;

	while (1)
	{
		cmd = Sys_ConsoleInput ();
		if (!cmd)
			break;
		Cbuf_AddText (cmd);
	}
}

/*
==================
Host_Frame

Runs all active servers
==================
*/
static void Host_Init(void);
void Host_Main(void)
{
	double time1 = 0;
	double time2 = 0;
	double time3 = 0;
	double cl_timer, sv_timer;
	double clframetime, deltarealtime, oldrealtime;
	double wait;
	int pass1, pass2, pass3;

	Host_Init();

	cl_timer = 0;
	sv_timer = 0;

	realtime = Sys_DoubleTime();
	dreamplaces_prof_start(PROFILER_TOTAL);
	for (;;)
	{
#if 0
		if (setjmp(host_abortframe))
			continue;			// something bad happened, or the server disconnected
#endif

		oldrealtime = realtime;
		realtime = Sys_DoubleTime();

		deltarealtime = realtime - oldrealtime;
		cl_timer += deltarealtime;
		sv_timer += deltarealtime;

		if (slowmo.value < 0)
			Cvar_SetValue("slowmo", 0);
		if (host_framerate.value < 0.00001 && host_framerate.value != 0)
			Cvar_SetValue("host_framerate", 0);
		if (cl_maxfps.value < 1)
			Cvar_SetValue("cl_maxfps", 1);

		// keep the random time dependent, but not when playing demos/benchmarking
		if(!*sv_random_seed.string
#ifndef NO_DEMO
		       	&& !cls.demoplayback
#endif
		  )
			rand();

		// get new key events
		Sys_SendKeyEvents();

		dreamplaces_prof_start(PROFILER_NET);
		cl.islocalgame = NetConn_IsLocalGame();
		NetConn_UpdateSockets();

		// receive packets on each main loop iteration, as the main loop may
		// be undersleeping due to select() detecting a new packet
		if (sv.active)
			NetConn_ServerFrame();
		dreamplaces_prof_end(PROFILER_NET);

		Curl_Run();

		// check for commands typed to the host
		Host_GetConsoleCommands();

		// when a server is running we only execute console commands on server frames
		// (this mainly allows frikbot .way config files to work properly by staying in sync with the server qc)
		// otherwise we execute them on all frames
		if (sv_timer > 0 || !sv.active)
		{
			// process console commands
			Cbuf_Execute();
		}

		//Con_Printf("%6.0f %6.0f\n", cl_timer * 1000000.0, sv_timer * 1000000.0);

		// if the accumulators haven't become positive yet, wait a while
		if (cls.state == ca_dedicated)
			wait = sv_timer * -1000000.0;
		else if (!sv.active)
			wait = cl_timer * -1000000.0;
		else
			wait = max(cl_timer, sv_timer) * -1000000.0;
		if (wait > 100000)
			wait = 100000;
		if ( wait > 0
#ifndef NO_DEMO
			&& !cls.timedemo
#endif
		   )
		{
#if 0
			if (sv_checkforpacketsduringsleep.integer)
			{
				if (wait >= 1)
					NetConn_SleepMicroseconds((int)wait);
			}
			else
			{
				if (wait >= 1000)
					Sys_Sleep((int)wait / 1000);
			}
#endif
			continue;
		}

	//-------------------
	//
	// server operations
	//
	//-------------------

		// limit the frametime steps to no more than 100ms each
		if (cl_timer > 0.1)
			cl_timer = 0.1;
		if (sv_timer > 0.1)
			sv_timer = 0.1;

		if (sv.active && sv_timer > 0)
		{
			// execute one or more server frames, with an upper limit on how much
			// execution time to spend on server frames to avoid freezing the game if
			// the server is overloaded, this execution time limit means the game will
			// slow down if the server is taking too long.
			int framecount, framelimit = 1;
			double advancetime, aborttime = 0;

			// run the world state
			// don't allow simulation to run too fast or too slow or logic glitches can occur

			// stop running server frames if the wall time reaches this value
			if (sys_ticrate.value <= 0)
				advancetime = sv_timer;
			else if (cl.islocalgame && !sv_fixedframeratesingleplayer.integer)
			{
				// synchronize to the client frametime, but no less than 10ms and no more than sys_ticrate
				advancetime = bound(0.01, cl_timer, sys_ticrate.value);
				framelimit = 10;
				aborttime = realtime + 0.1;
			}
			else
			{
				advancetime = sys_ticrate.value;
				// listen servers can run multiple server frames per client frame
				if (cls.state == ca_connected)
				{
					framelimit = 10;
					aborttime = realtime + 0.1;
				}
			}
			advancetime = min(advancetime, 0.1);

			// only advance time if not paused
			// the game also pauses in singleplayer when menu or console is used
			sv.frametime = advancetime * slowmo.value;
			if (host_framerate.value)
				sv.frametime = host_framerate.value;
			if (sv.paused || (cl.islocalgame && (key_dest != key_game || key_consoleactive)))
				sv.frametime = 0;

			// setup the VM frame
			SV_VM_Begin();

			for (framecount = 0;framecount < framelimit && sv_timer > 0;framecount++)
			{
				sv_timer -= advancetime;

				// move things around and think unless paused
				if (sv.frametime)
					SV_Physics();

				// send all messages to the clients
				SV_SendClientMessages();

				// clear the general datagram
				SV_ClearDatagram();

				// if this server frame took too long, break out of the loop
				if (framelimit > 1 && Sys_DoubleTime() >= aborttime)
					break;
			}

			// end the server VM frame
			SV_VM_End();

			// send an heartbeat if enough time has passed since the last one
			NetConn_Heartbeat(0);
		}

	//-------------------
	//
	// client operations
	//
	//-------------------

		if (cls.state != ca_dedicated && (cl_timer > 0
#ifndef NO_DEMO
				       	|| cls.timedemo
#endif
		    ) )
		{
			dreamplaces_prof_start(PROFILER_CL);
			// decide the simulation time
#ifndef NO_CAPTURE_VIDEO
			if (cls.capturevideo.active)
			{
				if (cls.capturevideo.realtime)
					clframetime = cl.realframetime = max(cl_timer, 1.0 / cls.capturevideo.framerate);
				else
				{
					clframetime = 1.0 / cls.capturevideo.framerate;
					cl.realframetime = max(cl_timer, clframetime);
				}
			}
			else
#endif
			if (vid_activewindow)
				clframetime = cl.realframetime = max(cl_timer, 1.0 / cl_maxfps.value);
			else
				clframetime = cl.realframetime = 0.1;

			// apply slowmo scaling
			clframetime *= slowmo.value;

			// host_framerate overrides all else
			if (host_framerate.value)
				clframetime = host_framerate.value;

#ifndef NO_DEMO
			if (cls.timedemo)
				clframetime = cl.realframetime = cl_timer;
#endif

			// deduct the frame time from the accumulator
			cl_timer -= cl.realframetime;

			cl.oldtime = cl.time;
			cl.time += clframetime;

			// Collect input into cmd
			CL_Input();

			dreamplaces_prof_start(PROFILER_NET);
			NetConn_ClientFrame();
			dreamplaces_prof_end(PROFILER_NET);

			if (cls.state == ca_connected)
			{
				CL_ReadFromServer();
				// if running the server remotely, send intentions now after
				// the incoming messages have been read
				//if (!cl.islocalgame)
				//	CL_SendCmd();
			}

			// update video
			if (host_speeds.integer)
				time1 = Sys_DoubleTime();

			//ui_update();

			CL_VideoFrame();

			CL_UpdateScreen();

			if (host_speeds.integer)
				time2 = Sys_DoubleTime();

			// update audio
			if(csqc_usecsqclistener)
			{
				S_Update(&csqc_listenermatrix);
				csqc_usecsqclistener = false;
			}
			else
				S_Update(&r_view.matrix);

			CDAudio_Update();

			if (host_speeds.integer)
			{
				pass1 = (int)((time1 - time3)*1000000);
				time3 = Sys_DoubleTime();
				pass2 = (int)((time2 - time1)*1000000);
				pass3 = (int)((time3 - time2)*1000000);
				Con_Printf("%6ius total %6ius server %6ius gfx %6ius snd\n",
							pass1+pass2+pass3, pass1, pass2, pass3);
			}
			dreamplaces_prof_end(PROFILER_CL);
		}

		// if there is some time remaining from this frame, reset the timers
		if (cl_timer >= 0)
			cl_timer = 0;
		if (sv_timer >= 0)
			sv_timer = 0;

		host_framecount++;
	}
}

//============================================================================

qboolean vid_opened = false;
void Host_StartVideo(void)
{
	if (!vid_opened && cls.state != ca_dedicated)
	{
		vid_opened = true;
		VID_Start();
#ifndef NO_SOUND
		CDAudio_Startup();
#endif
	}
}

#ifndef NO_GAMEMODE
static char engineversion[128];
#endif

qboolean sys_nostdout = false;

extern void Render_Init(void);
extern void Mathlib_Init(void);
extern void FS_Init(void);
extern void FS_Shutdown(void);
extern void PR_Cmd_Init(void);
extern void COM_Init_Commands(void);
extern void FS_Init_Commands(void);
extern qboolean host_stuffcmdsrun;

/*
====================
Host_Init
====================
*/
static void Host_Init (void)
{
	int i;
	const char* os;

	// LordHavoc: quake never seeded the random number generator before... heh
#if !defined(DEBUG_DREAMPLACES) && !defined(PROFILER_DREAMPLACES) && !defined(MAX_AUTOEVENTS) && !defined(DEBUG_GL)
	if (!COM_CheckParm("-benchmark"))
		srand(time(NULL));
	else
#endif
		srand(0); // predictable random sequence for -benchmark

#ifndef NO_DEVELOPER
	// FIXME: this is evil, but possibly temporary
// COMMANDLINEOPTION: Console: -developer enables warnings and other notices (RECOMMENDED for mod developers)
	if (COM_CheckParm("-developer"))
	{
		developer.value = developer.integer = 100;
		developer.string = "100";
	}

	if (COM_CheckParm("-developer2"))
	{
		developer.value = developer.integer = 100;
		developer.string = "100";
		developer_memory.value = developer_memory.integer = 100;
		developer.string = "100";
		developer_memorydebug.value = developer_memorydebug.integer = 100;
		developer_memorydebug.string = "100";
	}
#endif

// COMMANDLINEOPTION: Console: -nostdout disables text output to the terminal the game was launched from
	if (COM_CheckParm("-nostdout"))
		sys_nostdout = 1;

	// used by everything
	Memory_Init();

	// initialize console command/cvar/alias/command execution systems
	Cmd_Init();

	// initialize memory subsystem cvars/commands
	Memory_Init_Commands();

	// initialize console and logging and its cvars/commands
	Con_Init();

	// initialize various cvars that could not be initialized earlier
	Curl_Init_Commands();
	Cmd_Init_Commands();
	Sys_Init_Commands();
	COM_Init_Commands();
	FS_Init_Commands();

	// initialize console window (only used by sys_win.c)
	Sys_InitConsole();

	// detect gamemode from commandline options or executable name
	COM_InitGameType();

	// construct a version string for the corner of the console
#if defined(__linux__)
	os = "Linux";
#elif defined(WIN32)
	os = "Windows";
#elif defined(__FreeBSD__)
	os = "FreeBSD";
#elif defined(__NetBSD__)
	os = "NetBSD";
#elif defined(__OpenBSD__)
	os = "OpenBSD";
#elif defined(MACOSX)
	os = "Mac OS X";
#elif defined(__MORPHOS__)
	os = "MorphOS";
#elif defined(DREAMCAST)
	os = "Dreamcast";
#else
	os = "Unknown";
#endif
#ifndef NO_GAMEMODE
	dpsnprintf (engineversion, sizeof (engineversion), "%s %s %s", gamename, os, buildstring);
	Con_Printf("%s\n", engineversion);
#endif

	// initialize ixtable
	Mathlib_Init();

	// initialize filesystem (including fs_basedir, fs_gamedir, -game, scr_screenshot_name)
	FS_Init();

	NetConn_Init();
	Curl_Init();
	//PR_Init();
	//PR_Cmd_Init();
	PRVM_Init();
	Mod_Init();
	World_Init();
	SV_Init();
	Host_InitCommands();
	Host_InitLocal();
	Host_ServerOptions();

	if (cls.state != ca_dedicated)
	{
		Con_Printf("Initializing client\n");

		R_Modules_Init();
		Palette_Init();
		MR_Init_Commands();
		VID_Shared_Init();
		VID_Init();
		Render_Init();
		S_Init();
#ifndef NO_SOUND
		CDAudio_Init();
#endif
		Key_Init();
		V_Init();
		CL_Init();
	}

	// set up the default startmap_sp and startmap_dm aliases (mods can
	// override these) and then execute the quake.rc startup script
#ifndef NO_GAMEMODE
	dpsnprintf (engineversion, sizeof (engineversion), "%s %s %s", gamename, os, buildstring);
	if (gamemode == GAME_NEHAHRA)
		Cbuf_AddText("alias startmap_sp \"map nehstart\"\nalias startmap_dm \"map nehstart\"\nexec quake.rc\n");
	else if (gamemode == GAME_TRANSFUSION)
		Cbuf_AddText("alias startmap_sp \"map e1m1\"\n""alias startmap_dm \"map bb1\"\nexec quake.rc\n");
	else if (gamemode == GAME_TEU)
		Cbuf_AddText("alias startmap_sp \"map start\"\nalias startmap_dm \"map start\"\nexec teu.rc\n");
	else
#endif
		Cbuf_AddText("alias startmap_sp \"map start\"\nalias startmap_dm \"map start\"\nexec quake.rc\n");
	Cbuf_Execute();

	// if stuffcmds wasn't run, then quake.rc is probably missing, use default
	if (!host_stuffcmdsrun)
	{
		Cbuf_AddText("exec default.cfg\nexec config.cfg\nexec autoexec.cfg\nstuffcmds\n");
		Cbuf_Execute();
	}

	// put up the loading image so the user doesn't stare at a black screen...
	SCR_BeginLoadingPlaque();

	// FIXME: put this into some neat design, but the menu should be allowed to crash
	// without crashing the whole game, so this should just be a short-time solution

	// here comes the not so critical stuff
#if 0
	if (setjmp(host_abortframe)) {
		return;
	}
#endif

	if (cls.state != ca_dedicated)
	{
		MR_Init();
	}

	// check for special benchmark mode
// COMMANDLINEOPTION: Client: -benchmark <demoname> runs a timedemo and quits, results of any timedemo can be found in gamedir/benchmark.log (for example id1/benchmark.log)
	i = COM_CheckParm("-benchmark");
	if (i && i + 1 < com_argc)
	if (!sv.active
#ifndef NO_DEMO
		   && !cls.demoplayback
#endif
		   && !cls.connect_trying)
	{
		Cbuf_AddText(va("timedemo %s\n", com_argv[i + 1]));
		Cbuf_Execute();
	}

	// check for special demo mode
// COMMANDLINEOPTION: Client: -demo <demoname> runs a playdemo and quits
#ifndef NO_DEMO
	i = COM_CheckParm("-demo");
	if (i && i + 1 < com_argc)
	if (!sv.active && !cls.demoplayback && !cls.connect_trying)
	{
		Cbuf_AddText(va("playdemo %s\n", com_argv[i + 1]));
		Cbuf_Execute();
	}
#endif

	if (cls.state == ca_dedicated || COM_CheckParm("-listen"))
	if (!sv.active
#ifndef NO_DEMO
		      	&& !cls.demoplayback
#endif
		       	&& !cls.connect_trying)
	{
		Cbuf_AddText("startmap_dm\n");
		Cbuf_Execute();
	}

	if (!sv.active
#ifndef NO_DEMO
		       	&& !cls.demoplayback
#endif
		       	&& !cls.connect_trying)
	{
#ifndef NO_GAMEMODE
		if (gamemode == GAME_NEXUIZ)
			Cbuf_AddText("togglemenu\nplayvideo logo\ncd loop 1\n");
		else
#endif
			Cbuf_AddText("togglemenu\n");
		Cbuf_Execute();
	}

	Con_DPrint("========Initialized=========\n");

	//Host_StartVideo();
}


/*
===============
Host_Shutdown

FIXME: this is a callback from Sys_Quit and Sys_Error.  It would be better
to run quit through here before the final handoff to the sys code.
===============
*/
void Host_Shutdown(void)
{
	static qboolean isdown = false;

	if (isdown)
	{
		Con_Print("recursive shutdown\n");
		return;
	}
	isdown = true;

	// be quiet while shutting down
	S_StopAllSounds();

	// disconnect client from server if active
	CL_Disconnect();

	// shut down local server if active
	Host_ShutdownServer ();

	// Shutdown menu
	if(MR_Shutdown)
		MR_Shutdown();

	// AK shutdown PRVM
	// AK hmm, no PRVM_Shutdown(); yet

	CL_Video_Shutdown();

	Host_SaveConfig_f();

	CDAudio_Shutdown ();
	S_Terminate ();
	Curl_Shutdown ();
	NetConn_Shutdown ();
	//PR_Shutdown ();

	if (cls.state != ca_dedicated)
	{
		R_Modules_Shutdown();
		VID_Shutdown();
	}

	Cmd_Shutdown();
	CL_Shutdown();
	Sys_Shutdown();
	Log_Close();
	FS_Shutdown();
	Memory_Shutdown();
}

