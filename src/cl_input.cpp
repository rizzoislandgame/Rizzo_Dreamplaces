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
// cl.input.c  -- builds an intended movement command to send to the server

// Quake is a trademark of Id Software, Inc., (c) 1996 Id Software, Inc. All
// rights reserved.

#include "quakedef.h"
#include "csprogs.h"

/*
===============================================================================

KEY BUTTONS

Continuous button event tracking is complicated by the fact that two different
input sources (say, mouse button 1 and the control key) can both press the
same button, but the button should only be released when both of the
pressing key have been released.

When a key event issues a button command (+forward, +attack, etc), it appends
its key number as a parameter to the command so it can be matched up with
the release.

state bit 0 is the current state of the key
state bit 1 is edge triggered on the up to down transition
state bit 2 is edge triggered on the down to up transition

===============================================================================
*/


kbutton_t	in_mlook, in_klook;
kbutton_t	in_left, in_right, in_forward, in_back;
kbutton_t	in_lookup, in_lookdown, in_moveleft, in_moveright;
kbutton_t	in_strafe, in_speed, in_jump, in_attack, in_use;
kbutton_t	in_up, in_down;
// LordHavoc: added 6 new buttons
kbutton_t	in_button3, in_button4, in_button5, in_button6, in_button7, in_button8;
//even more
kbutton_t	in_button9, in_button10, in_button11, in_button12, in_button13, in_button14, in_button15, in_button16;

int			in_impulse;



void KeyDown (kbutton_t *b)
{
	int k;
	const char *c;

	c = Cmd_Argv(1);
	if (c[0])
		k = atoi(c);
	else
		k = -1;		// typed manually at the console for continuous down

	if (k == b->down[0] || k == b->down[1])
		return;		// repeating key

	if (!b->down[0])
		b->down[0] = k;
	else if (!b->down[1])
		b->down[1] = k;
	else
	{
		Con_Print("Three keys down for a button!\n");
		return;
	}

	if (b->state & 1)
		return;		// still down
	b->state |= 1 + 2;	// down + impulse down
}

void KeyUp (kbutton_t *b)
{
	int k;
	const char *c;

	c = Cmd_Argv(1);
	if (c[0])
		k = atoi(c);
	else
	{ // typed manually at the console, assume for unsticking, so clear all
		b->down[0] = b->down[1] = 0;
		b->state = 4;	// impulse up
		return;
	}

	if (b->down[0] == k)
		b->down[0] = 0;
	else if (b->down[1] == k)
		b->down[1] = 0;
	else
		return;		// key up without coresponding down (menu pass through)
	if (b->down[0] || b->down[1])
		return;		// some other key is still holding it down

	if (!(b->state & 1))
		return;		// still up (this should not happen)
	b->state &= ~1;		// now up
	b->state |= 4; 		// impulse up
}

void IN_KLookDown (void) {KeyDown(&in_klook);}
void IN_KLookUp (void) {KeyUp(&in_klook);}
void IN_MLookDown (void) {KeyDown(&in_mlook);}
void IN_MLookUp (void)
{
	KeyUp(&in_mlook);
	if ( !(in_mlook.state&1) && lookspring.value)
		V_StartPitchDrift();
}
void IN_UpDown(void) {KeyDown(&in_up);}
void IN_UpUp(void) {KeyUp(&in_up);}
void IN_DownDown(void) {KeyDown(&in_down);}
void IN_DownUp(void) {KeyUp(&in_down);}
void IN_LeftDown(void) {KeyDown(&in_left);}
void IN_LeftUp(void) {KeyUp(&in_left);}
void IN_RightDown(void) {KeyDown(&in_right);}
void IN_RightUp(void) {KeyUp(&in_right);}
void IN_ForwardDown(void) {KeyDown(&in_forward);}
void IN_ForwardUp(void) {KeyUp(&in_forward);}
void IN_BackDown(void) {KeyDown(&in_back);}
void IN_BackUp(void) {KeyUp(&in_back);}
void IN_LookupDown(void) {KeyDown(&in_lookup);}
void IN_LookupUp(void) {KeyUp(&in_lookup);}
void IN_LookdownDown(void) {KeyDown(&in_lookdown);}
void IN_LookdownUp(void) {KeyUp(&in_lookdown);}
void IN_MoveleftDown(void) {KeyDown(&in_moveleft);}
void IN_MoveleftUp(void) {KeyUp(&in_moveleft);}
void IN_MoverightDown(void) {KeyDown(&in_moveright);}
void IN_MoverightUp(void) {KeyUp(&in_moveright);}

void IN_SpeedDown(void) {KeyDown(&in_speed);}
void IN_SpeedUp(void) {KeyUp(&in_speed);}
void IN_StrafeDown(void) {KeyDown(&in_strafe);}
void IN_StrafeUp(void) {KeyUp(&in_strafe);}

void IN_AttackDown(void) {KeyDown(&in_attack);}
void IN_AttackUp(void) {KeyUp(&in_attack);}

void IN_UseDown(void) {KeyDown(&in_use);}
void IN_UseUp(void) {KeyUp(&in_use);}

// LordHavoc: added 6 new buttons
void IN_Button3Down(void) {KeyDown(&in_button3);}
void IN_Button3Up(void) {KeyUp(&in_button3);}
void IN_Button4Down(void) {KeyDown(&in_button4);}
void IN_Button4Up(void) {KeyUp(&in_button4);}
void IN_Button5Down(void) {KeyDown(&in_button5);}
void IN_Button5Up(void) {KeyUp(&in_button5);}
void IN_Button6Down(void) {KeyDown(&in_button6);}
void IN_Button6Up(void) {KeyUp(&in_button6);}
void IN_Button7Down(void) {KeyDown(&in_button7);}
void IN_Button7Up(void) {KeyUp(&in_button7);}
void IN_Button8Down(void) {KeyDown(&in_button8);}
void IN_Button8Up(void) {KeyUp(&in_button8);}

void IN_Button9Down(void) {KeyDown(&in_button9);}
void IN_Button9Up(void) {KeyUp(&in_button9);}
void IN_Button10Down(void) {KeyDown(&in_button10);}
void IN_Button10Up(void) {KeyUp(&in_button10);}
void IN_Button11Down(void) {KeyDown(&in_button11);}
void IN_Button11Up(void) {KeyUp(&in_button11);}
void IN_Button12Down(void) {KeyDown(&in_button12);}
void IN_Button12Up(void) {KeyUp(&in_button12);}
void IN_Button13Down(void) {KeyDown(&in_button13);}
void IN_Button13Up(void) {KeyUp(&in_button13);}
void IN_Button14Down(void) {KeyDown(&in_button14);}
void IN_Button14Up(void) {KeyUp(&in_button14);}
void IN_Button15Down(void) {KeyDown(&in_button15);}
void IN_Button15Up(void) {KeyUp(&in_button15);}
void IN_Button16Down(void) {KeyDown(&in_button16);}
void IN_Button16Up(void) {KeyUp(&in_button16);}

void IN_JumpDown (void) {KeyDown(&in_jump);}
void IN_JumpUp (void) {KeyUp(&in_jump);}

void IN_Impulse (void) {in_impulse=atoi(Cmd_Argv(1));}

struct
{
	const char *name;
	int impulse;
	int weaponbit;
	int ammostat;
	int ammomin;
}
in_bestweapon_info[] =
{
	{"1", 1, IT_AXE, STAT_SHELLS, 0},
	{"2", 2, IT_SHOTGUN, STAT_SHELLS, 1},
	{"3", 3, IT_SUPER_SHOTGUN, STAT_SHELLS, 1},
	{"4", 4, IT_NAILGUN, STAT_NAILS, 1},
	{"5", 5, IT_SUPER_NAILGUN, STAT_NAILS, 1},
	{"6", 6, IT_GRENADE_LAUNCHER, STAT_ROCKETS, 1},
	{"7", 7, IT_ROCKET_LAUNCHER, STAT_ROCKETS, 1},
	{"8", 8, IT_LIGHTNING, STAT_CELLS, 1},
	{"9", 9, 128, STAT_CELLS, 1}, // generic energy weapon for mods
	{"p", 209, 128, STAT_CELLS, 1}, // dpmod plasma gun
	{"w", 210, 8388608, STAT_CELLS, 1}, // dpmod plasma wave cannon
	{"l", 225, HIT_LASER_CANNON, STAT_CELLS, 1}, // hipnotic laser cannon
	{"h", 226, HIT_MJOLNIR, STAT_CELLS, 0}, // hipnotic mjolnir hammer
	{NULL, 0, 0, 0, 0}
};
void IN_BestWeapon (void)
{
	int i, n;
	const char *t;
	if (Cmd_Argc() < 2)
	{
		Con_Printf("bestweapon requires 1 or more parameters\n");
		return;
	}
	for (i = 1;i < Cmd_Argc();i++)
	{
		t = Cmd_Argv(i);
		// figure out which weapon this character refers to
		for (n = 0;in_bestweapon_info[n].name;n++)
		{
			if (!strcmp(in_bestweapon_info[n].name, t))
			{
				// we found out what weapon this character refers to
				// check if the inventory contains the weapon and enough ammo
				if ((cl.stats[STAT_ITEMS] & in_bestweapon_info[n].weaponbit) && (cl.stats[in_bestweapon_info[n].ammostat] >= in_bestweapon_info[n].ammomin))
				{
					// we found one of the weapons the player wanted
					// send an impulse to switch to it
					in_impulse = in_bestweapon_info[n].impulse;
					return;
				}
				break;
			}
		}
		// if we couldn't identify the weapon we just ignore it and continue checking for other weapons
	}
	// if we couldn't find any of the weapons, there's nothing more we can do...
}

/*
===============
CL_KeyState

Returns 0.25 if a key was pressed and released during the frame,
0.5 if it was pressed and held
0 if held then released, and
1.0 if held for the entire time
===============
*/
float CL_KeyState (kbutton_t *key)
{
	float		val;
	qboolean	impulsedown, impulseup, down;

	impulsedown = key->state & 2;
	impulseup = key->state & 4;
	down = key->state & 1;
	val = 0;

	if (impulsedown && !impulseup)
	{
		if (down)
			val = 0.5;	// pressed and held this frame
		else
			val = 0;	//	I_Error ();
	}
	if (impulseup && !impulsedown)
	{
		if (down)
			val = 0;	//	I_Error ();
		else
			val = 0;	// released this frame
	}
	if (!impulsedown && !impulseup)
	{
		if (down)
			val = 1.0;	// held the entire frame
		else
			val = 0;	// up the entire frame
	}
	if (impulsedown && impulseup)
	{
		if (down)
			val = 0.75;	// released and re-pressed this frame
		else
			val = 0.25;	// pressed and released this frame
	}

	key->state &= 1;		// clear impulses

	return val;
}




//==========================================================================

cvar_t cl_upspeed = {CVAR_SAVE, "cl_upspeed","400","vertical movement speed (while swimming or flying)"};
cvar_t cl_forwardspeed = {CVAR_SAVE, "cl_forwardspeed","400","forward movement speed"};
cvar_t cl_backspeed = {CVAR_SAVE, "cl_backspeed","400","backward movement speed"};
cvar_t cl_sidespeed = {CVAR_SAVE, "cl_sidespeed","350","strafe movement speed"};

cvar_t cl_movespeedkey = {CVAR_SAVE, "cl_movespeedkey","2.0","how much +speed multiplies keyboard movement speed"};

cvar_t cl_yawspeed = {CVAR_SAVE, "cl_yawspeed","140","keyboard yaw turning speed"};
cvar_t cl_pitchspeed = {CVAR_SAVE, "cl_pitchspeed","150","keyboard pitch turning speed"};

cvar_t cl_anglespeedkey = {CVAR_SAVE, "cl_anglespeedkey","1.5","how much +speed multiplies keyboard turning speed"};

cvar_t cl_movement = {CVAR_SAVE, "cl_movement", "0", "enables clientside prediction of your player movement"};
cvar_t cl_movement_minping = {CVAR_SAVE, "cl_movement_minping", "50", "whether to use prediction when ping is lower than this value in milliseconds"};
cvar_t cl_movement_maxspeed = {0, "cl_movement_maxspeed", "320", "how fast you can move (should match sv_maxspeed)"};
cvar_t cl_movement_maxairspeed = {0, "cl_movement_maxairspeed", "30", "how fast you can move while in the air (should match sv_maxairspeed)"};
cvar_t cl_movement_stopspeed = {0, "cl_movement_stopspeed", "100", "speed below which you will be slowed rapidly to a stop rather than sliding endlessly (should match sv_stopspeed)"};
cvar_t cl_movement_friction = {0, "cl_movement_friction", "4", "how fast you slow down (should match sv_friction)"};
cvar_t cl_movement_waterfriction = {0, "cl_movement_waterfriction", "-1", "how fast you slow down (should match sv_friction), if less than 0 the cl_movement_friction variable is used instead"};
cvar_t cl_movement_edgefriction = {0, "cl_movement_edgefriction", "2", "how much to slow down when you may be about to fall off a ledge (should match edgefriction)"};
cvar_t cl_movement_stepheight = {0, "cl_movement_stepheight", "18", "how tall a step you can step in one instant (should match sv_stepheight)"};
cvar_t cl_movement_accelerate = {0, "cl_movement_accelerate", "10", "how fast you accelerate (should match sv_accelerate)"};
cvar_t cl_movement_airaccelerate = {0, "cl_movement_airaccelerate", "-1", "how fast you accelerate while in the air (should match sv_airaccelerate), if less than 0 the cl_movement_accelerate variable is used instead"};
cvar_t cl_movement_wateraccelerate = {0, "cl_movement_wateraccelerate", "-1", "how fast you accelerate while in the air (should match sv_airaccelerate), if less than 0 the cl_movement_accelerate variable is used instead"};
cvar_t cl_movement_jumpvelocity = {0, "cl_movement_jumpvelocity", "270", "how fast you move upward when you begin a jump (should match the quakec code)"};
cvar_t cl_movement_airaccel_qw = {0, "cl_movement_airaccel_qw", "1", "ratio of QW-style air control as opposed to simple acceleration (should match sv_airaccel_qw)"};
cvar_t cl_movement_airaccel_sideways_friction = {0, "cl_movement_airaccel_sideways_friction", "0", "anti-sideways movement stabilization (should match sv_airaccel_sideways_friction)"};
cvar_t cl_gravity = {0, "cl_gravity", "800", "how much gravity to apply in client physics (should match sv_gravity)"};
cvar_t cl_slowmo = {0, "cl_slowmo", "1", "speed of game time (should match slowmo)"};

cvar_t in_pitch_min = {0, "in_pitch_min", "-90", "how far downward you can aim (quake used -70"};
cvar_t in_pitch_max = {0, "in_pitch_max", "90", "how far upward you can aim (quake used 80"};

cvar_t m_filter = {CVAR_SAVE, "m_filter","0", "smoothes mouse movement, less responsive but smoother aiming"};

cvar_t cl_netinputpacketspersecond = {CVAR_SAVE, "cl_netinputpacketspersecond","50", "how many input packets to send to server each second"};
cvar_t cl_netinputpacketlosstolerance = {CVAR_SAVE, "cl_netinputpacketlosstolerance", "1", "how many packets in a row can be lost without movement issues when using cl_movement (technically how many input messages to repeat in each packet that have not yet been acknowledged by the server), only affects DP7 servers (Quake uses 0, QuakeWorld uses 2, and just for comparison Quake3 uses 1)"};

cvar_t cl_nodelta = {0, "cl_nodelta", "0", "disables delta compression of non-player entities in QW network protocol"};


/*
================
CL_AdjustAngles

Moves the local angle positions
================
*/
void CL_AdjustAngles (void)
{
	float	speed;
	float	up, down;

	if (in_speed.state & 1)
		speed = cl.realframetime * cl_anglespeedkey.value;
	else
		speed = cl.realframetime;

	if (!(in_strafe.state & 1))
	{
		cl.viewangles[YAW] -= speed*cl_yawspeed.value*CL_KeyState (&in_right);
		cl.viewangles[YAW] += speed*cl_yawspeed.value*CL_KeyState (&in_left);
	}
	if (in_klook.state & 1)
	{
		V_StopPitchDrift ();
		cl.viewangles[PITCH] -= speed*cl_pitchspeed.value * CL_KeyState (&in_forward);
		cl.viewangles[PITCH] += speed*cl_pitchspeed.value * CL_KeyState (&in_back);
	}

	up = CL_KeyState (&in_lookup);
	down = CL_KeyState(&in_lookdown);

	cl.viewangles[PITCH] -= speed*cl_pitchspeed.value * up;
	cl.viewangles[PITCH] += speed*cl_pitchspeed.value * down;

	if (up || down)
		V_StopPitchDrift ();

	cl.viewangles[YAW] = ANGLEMOD(cl.viewangles[YAW]);
	cl.viewangles[PITCH] = ANGLEMOD(cl.viewangles[PITCH]);
	cl.viewangles[ROLL] = ANGLEMOD(cl.viewangles[ROLL]);
	if (cl.viewangles[YAW] >= 180)
		cl.viewangles[YAW] -= 360;
	if (cl.viewangles[PITCH] >= 180)
		cl.viewangles[PITCH] -= 360;
	if (cl.viewangles[ROLL] >= 180)
		cl.viewangles[ROLL] -= 360;

	cl.viewangles[PITCH] = bound (in_pitch_min.value, cl.viewangles[PITCH], in_pitch_max.value);
	cl.viewangles[ROLL] = bound(-50, cl.viewangles[ROLL], 50);
}

qboolean cl_ignoremousemove = false;

/*
================
CL_Input

Send the intended movement message to the server
================
*/
void CL_Input (void)
{
	float mx, my;
	static float old_mouse_x = 0, old_mouse_y = 0;

	// clamp before the move to prevent starting with bad angles
	CL_AdjustAngles ();

	// reset some of the command fields
	cl.cmd.forwardmove = 0;
	cl.cmd.sidemove = 0;
	cl.cmd.upmove = 0;

	// get basic movement from keyboard
	if (in_strafe.state & 1)
	{
		cl.cmd.sidemove += cl_sidespeed.value * CL_KeyState (&in_right);
		cl.cmd.sidemove -= cl_sidespeed.value * CL_KeyState (&in_left);
	}

	cl.cmd.sidemove += cl_sidespeed.value * CL_KeyState (&in_moveright);
	cl.cmd.sidemove -= cl_sidespeed.value * CL_KeyState (&in_moveleft);

	cl.cmd.upmove += cl_upspeed.value * CL_KeyState (&in_up);
	cl.cmd.upmove -= cl_upspeed.value * CL_KeyState (&in_down);

	if (! (in_klook.state & 1) )
	{
		cl.cmd.forwardmove += cl_forwardspeed.value * CL_KeyState (&in_forward);
		cl.cmd.forwardmove -= cl_backspeed.value * CL_KeyState (&in_back);
	}

	// adjust for speed key
	if (in_speed.state & 1)
	{
		cl.cmd.forwardmove *= cl_movespeedkey.value;
		cl.cmd.sidemove *= cl_movespeedkey.value;
		cl.cmd.upmove *= cl_movespeedkey.value;
	}

	in_mouse_x = 0;
	in_mouse_y = 0;

	// allow mice or other external controllers to add to the move
	IN_Move ();

	// ignore a mouse move if mouse was activated/deactivated this frame
	if (cl_ignoremousemove)
	{
		cl_ignoremousemove = false;
		in_mouse_x = 0;
		in_mouse_y = 0;
	}

	// apply m_filter if it is on
	mx = in_mouse_x;
	my = in_mouse_y;
	if (m_filter.integer)
	{
		in_mouse_x = (mx + old_mouse_x) * 0.5;
		in_mouse_y = (my + old_mouse_y) * 0.5;
	}
	old_mouse_x = mx;
	old_mouse_y = my;

	// if not in menu, apply mouse move to viewangles/movement
	if (!cl.csqc_wantsmousemove && in_client_mouse)
	{
		if (cl_prydoncursor.integer)
		{
			// mouse interacting with the scene, mostly stationary view
			V_StopPitchDrift();
			cl.cmd.cursor_screen[0] += in_mouse_x * sensitivity.value / vid.width;
			cl.cmd.cursor_screen[1] += in_mouse_y * sensitivity.value / vid.height;
		}
		else if (in_strafe.state & 1)
		{
			// strafing mode, all looking is movement
			V_StopPitchDrift();
			cl.cmd.sidemove += m_side.value * in_mouse_x * sensitivity.value;
			if (noclip_anglehack)
				cl.cmd.upmove -= m_forward.value * in_mouse_y * sensitivity.value;
			else
				cl.cmd.forwardmove -= m_forward.value * in_mouse_y * sensitivity.value;
		}
		else if ((in_mlook.state & 1) || freelook.integer)
		{
			// mouselook, lookstrafe causes turning to become strafing
			V_StopPitchDrift();
			if (lookstrafe.integer)
				cl.cmd.sidemove += m_side.value * in_mouse_x * sensitivity.value;
			else
				cl.viewangles[YAW] -= m_yaw.value * in_mouse_x * sensitivity.value * cl.viewzoom;
			cl.viewangles[PITCH] += m_pitch.value * in_mouse_y * sensitivity.value * cl.viewzoom;
		}
		else
		{
			// non-mouselook, yaw turning and forward/back movement
			cl.viewangles[YAW] -= m_yaw.value * in_mouse_x * sensitivity.value * cl.viewzoom;
			cl.cmd.forwardmove -= m_forward.value * in_mouse_y * sensitivity.value;
		}
	}

	// clamp after the move to prevent rendering with bad angles
	CL_AdjustAngles ();
}

#include "cl_collision.h"

void CL_UpdatePrydonCursor(void)
{
	vec3_t temp;

	if (!cl_prydoncursor.integer)
		VectorClear(cl.cmd.cursor_screen);

	/*
	if (cl.cmd.cursor_screen[0] < -1)
	{
		cl.viewangles[YAW] -= m_yaw.value * (cl.cmd.cursor_screen[0] - -1) * vid.width * sensitivity.value * cl.viewzoom;
		cl.cmd.cursor_screen[0] = -1;
	}
	if (cl.cmd.cursor_screen[0] > 1)
	{
		cl.viewangles[YAW] -= m_yaw.value * (cl.cmd.cursor_screen[0] - 1) * vid.width * sensitivity.value * cl.viewzoom;
		cl.cmd.cursor_screen[0] = 1;
	}
	if (cl.cmd.cursor_screen[1] < -1)
	{
		cl.viewangles[PITCH] += m_pitch.value * (cl.cmd.cursor_screen[1] - -1) * vid.height * sensitivity.value * cl.viewzoom;
		cl.cmd.cursor_screen[1] = -1;
	}
	if (cl.cmd.cursor_screen[1] > 1)
	{
		cl.viewangles[PITCH] += m_pitch.value * (cl.cmd.cursor_screen[1] - 1) * vid.height * sensitivity.value * cl.viewzoom;
		cl.cmd.cursor_screen[1] = 1;
	}
	*/
	cl.cmd.cursor_screen[0] = bound(-1, cl.cmd.cursor_screen[0], 1);
	cl.cmd.cursor_screen[1] = bound(-1, cl.cmd.cursor_screen[1], 1);
	cl.cmd.cursor_screen[2] = 1;

	// calculate current view matrix
	Matrix4x4_OriginFromMatrix(&r_view.matrix, cl.cmd.cursor_start);
	// calculate direction vector of cursor in viewspace by using frustum slopes
	VectorSet(temp, cl.cmd.cursor_screen[2] * 1000000, cl.cmd.cursor_screen[0] * -r_view.frustum_x * 1000000, cl.cmd.cursor_screen[1] * -r_view.frustum_y * 1000000);
	Matrix4x4_Transform(&r_view.matrix, temp, cl.cmd.cursor_end);
	// trace from view origin to the cursor
	cl.cmd.cursor_fraction = CL_SelectTraceLine(cl.cmd.cursor_start, cl.cmd.cursor_end, cl.cmd.cursor_impact, cl.cmd.cursor_normal, &cl.cmd.cursor_entitynumber, (chase_active.integer || cl.intermission) ? &cl.entities[cl.playerentity].render : NULL);
}

void CL_ClientMovement_InputQW(void)
{
	int i;
	int n;
	// if time has not advanced, do nothing
	if (cl.movecmd[0].time <= cl.movecmd[1].time)
		return;
	// remove stale queue items
	n = cl.movement_numqueue;
	cl.movement_numqueue = 0;
	for (i = 0;i < n;i++)
	{
		if (cl.movement_queue[i].sequence > cls.netcon->qw.incoming_sequence)
			cl.movement_queue[cl.movement_numqueue++] = cl.movement_queue[i];
		else if (i == 0)
			cl.movement_replay_canjump = !cl.movement_queue[i].jump; // FIXME: this logic is quite broken
	}
	// add to input queue if there is room
	if (cl.movement_numqueue < (int)(sizeof(cl.movement_queue)/sizeof(cl.movement_queue[0])))
	{
		// add to input queue
		cl.movement_queue[cl.movement_numqueue].sequence = cls.netcon->qw.outgoing_sequence;
		cl.movement_queue[cl.movement_numqueue].time = cl.movecmd[0].time;
		cl.movement_queue[cl.movement_numqueue].frametime = cl.cmd.msec / 1000.0;
		VectorCopy(cl.cmd.viewangles, cl.movement_queue[cl.movement_numqueue].viewangles);
		cl.movement_queue[cl.movement_numqueue].move[0] = cl.cmd.forwardmove;
		cl.movement_queue[cl.movement_numqueue].move[1] = cl.cmd.sidemove;
		cl.movement_queue[cl.movement_numqueue].move[2] = cl.cmd.upmove;
		cl.movement_queue[cl.movement_numqueue].jump = (cl.cmd.buttons & 2) != 0;
		cl.movement_queue[cl.movement_numqueue].crouch = false;
		cl.movement_numqueue++;
	}

	cl.movement_replay = true;
}

void CL_ClientMovement_Input(qboolean buttonjump, qboolean buttoncrouch)
{
	int i;
	int n;
	// if time has not advanced, do nothing
	if (cl.movecmd[0].time <= cl.movecmd[1].time)
		return;
	// remove stale queue items
	n = cl.movement_numqueue;
	cl.movement_numqueue = 0;
	if (cls.servermovesequence)
	{
		for (i = 0;i < n;i++)
		{
			if (cl.movement_queue[i].sequence > cls.servermovesequence)
				cl.movement_queue[cl.movement_numqueue++] = cl.movement_queue[i];
			else if (i == 0)
				cl.movement_replay_canjump = !cl.movement_queue[i].jump; // FIXME: this logic is quite broken
		}
	}
	// add to input queue if there is room
	if (cl.movement_numqueue < (int)(sizeof(cl.movement_queue)/sizeof(cl.movement_queue[0])))
	{
		// add to input queue
		cl.movement_queue[cl.movement_numqueue].sequence = cls.movesequence;
		cl.movement_queue[cl.movement_numqueue].time = cl.movecmd[0].time;
		cl.movement_queue[cl.movement_numqueue].frametime = bound(0, cl.movecmd[0].time - cl.movecmd[1].time, 0.1);
		VectorCopy(cl.viewangles, cl.movement_queue[cl.movement_numqueue].viewangles);
		cl.movement_queue[cl.movement_numqueue].move[0] = cl.cmd.forwardmove;
		cl.movement_queue[cl.movement_numqueue].move[1] = cl.cmd.sidemove;
		cl.movement_queue[cl.movement_numqueue].move[2] = cl.cmd.upmove;
		cl.movement_queue[cl.movement_numqueue].jump = buttonjump;
		cl.movement_queue[cl.movement_numqueue].crouch = buttoncrouch;
		cl.movement_numqueue++;
	}

	cl.movement_replay = true;
}

typedef enum waterlevel_e
{
	WATERLEVEL_NONE,
	WATERLEVEL_WETFEET,
	WATERLEVEL_SWIMMING,
	WATERLEVEL_SUBMERGED
}
waterlevel_t;

typedef struct cl_clientmovement_state_s
{
	// position
	vec3_t origin;
	vec3_t velocity;
	// current bounding box (different if crouched vs standing)
	vec3_t mins;
	vec3_t maxs;
	// currently on the ground
	qboolean onground;
	// currently crouching
	qboolean crouched;
	// whether jump button has been released since last jump
	qboolean canjump;
	// what kind of water (SUPERCONTENTS_LAVA for instance)
	int watertype;
	// how deep
	waterlevel_t waterlevel;
	// weird hacks when jumping out of water
	// (this is in seconds and counts down to 0)
	float waterjumptime;

	// movement parameters for physics code
	float movevars_gravity;
	float movevars_stopspeed;
	float movevars_maxspeed;
	float movevars_spectatormaxspeed;
	float movevars_accelerate;
	float movevars_airaccelerate;
	float movevars_wateraccelerate;
	float movevars_friction;
	float movevars_waterfriction;
	float movevars_entgravity;
	float movevars_jumpvelocity;
	float movevars_edgefriction;
	float movevars_maxairspeed;
	float movevars_stepheight;
	float movevars_airaccel_qw;
	float movevars_airaccel_sideways_friction;

	// user command
	client_movementqueue_t q;
}
cl_clientmovement_state_t;

#define NUMOFFSETS 27
static vec3_t offsets[NUMOFFSETS] =
{
// 1 no nudge (just return the original if this test passes)
	{ 0.000,  0.000,  0.000},
// 6 simple nudges
	{ 0.000,  0.000,  0.125}, { 0.000,  0.000, -0.125},
	{-0.125,  0.000,  0.000}, { 0.125,  0.000,  0.000},
	{ 0.000, -0.125,  0.000}, { 0.000,  0.125,  0.000},
// 4 diagonal flat nudges
	{-0.125, -0.125,  0.000}, { 0.125, -0.125,  0.000},
	{-0.125,  0.125,  0.000}, { 0.125,  0.125,  0.000},
// 8 diagonal upward nudges
	{-0.125,  0.000,  0.125}, { 0.125,  0.000,  0.125},
	{ 0.000, -0.125,  0.125}, { 0.000,  0.125,  0.125},
	{-0.125, -0.125,  0.125}, { 0.125, -0.125,  0.125},
	{-0.125,  0.125,  0.125}, { 0.125,  0.125,  0.125},
// 8 diagonal downward nudges
	{-0.125,  0.000, -0.125}, { 0.125,  0.000, -0.125},
	{ 0.000, -0.125, -0.125}, { 0.000,  0.125, -0.125},
	{-0.125, -0.125, -0.125}, { 0.125, -0.125, -0.125},
	{-0.125,  0.125, -0.125}, { 0.125,  0.125, -0.125},
};

qboolean CL_ClientMovement_Unstick(cl_clientmovement_state_t *s)
{
	int i;
	vec3_t neworigin;
	for (i = 0;i < NUMOFFSETS;i++)
	{
		VectorAdd(offsets[i], s->origin, neworigin);
		if (!CL_Move(neworigin, cl.playercrouchmins, cl.playercrouchmaxs, neworigin, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false).startsolid)
		{
			VectorCopy(neworigin, s->origin);
			return true;
		}
	}
	// if all offsets failed, give up
	return false;
}

void CL_ClientMovement_UpdateStatus(cl_clientmovement_state_t *s)
{
	vec3_t origin1, origin2;
	trace_t trace;

	// make sure player is not stuck
	CL_ClientMovement_Unstick(s);

	// set crouched
	if (s->q.crouch)
	{
		// wants to crouch, this always works..
		if (!s->crouched)
			s->crouched = true;
	}
	else
	{
		// wants to stand, if currently crouching we need to check for a
		// low ceiling first
		if (s->crouched)
		{
			trace = CL_Move(s->origin, cl.playerstandmins, cl.playerstandmaxs, s->origin, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false);
			if (!trace.startsolid)
				s->crouched = false;
		}
	}
	if (s->crouched)
	{
		VectorCopy(cl.playercrouchmins, s->mins);
		VectorCopy(cl.playercrouchmaxs, s->maxs);
	}
	else
	{
		VectorCopy(cl.playerstandmins, s->mins);
		VectorCopy(cl.playerstandmaxs, s->maxs);
	}

	// set onground
	VectorSet(origin1, s->origin[0], s->origin[1], s->origin[2] + 1);
	VectorSet(origin2, s->origin[0], s->origin[1], s->origin[2] - 2);
	trace = CL_Move(origin1, s->mins, s->maxs, origin2, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false);
	s->onground = trace.fraction < 1 && trace.plane.normal[2] > 0.7;

	// set watertype/waterlevel
	VectorSet(origin1, s->origin[0], s->origin[1], s->origin[2] + s->mins[2] + 1);
	s->waterlevel = WATERLEVEL_NONE;
	s->watertype = CL_Move(origin1, vec3_origin, vec3_origin, origin1, MOVE_NOMONSTERS, NULL, 0, true, false, NULL, false).startsupercontents & SUPERCONTENTS_LIQUIDSMASK;
	if (s->watertype)
	{
		s->waterlevel = WATERLEVEL_WETFEET;
		origin1[2] = s->origin[2] + (s->mins[2] + s->maxs[2]) * 0.5f;
		if (CL_Move(origin1, vec3_origin, vec3_origin, origin1, MOVE_NOMONSTERS, NULL, 0, true, false, NULL, false).startsupercontents & SUPERCONTENTS_LIQUIDSMASK)
		{
			s->waterlevel = WATERLEVEL_SWIMMING;
			origin1[2] = s->origin[2] + 22;
			if (CL_Move(origin1, vec3_origin, vec3_origin, origin1, MOVE_NOMONSTERS, NULL, 0, true, false, NULL, false).startsupercontents & SUPERCONTENTS_LIQUIDSMASK)
				s->waterlevel = WATERLEVEL_SUBMERGED;
		}
	}

	// water jump prediction
	if (s->onground || s->velocity[2] <= 0 || s->waterjumptime <= 0)
		s->waterjumptime = 0;
}

void CL_ClientMovement_Move(cl_clientmovement_state_t *s)
{
	int bump;
	double t;
	vec_t f;
	vec3_t neworigin;
	vec3_t currentorigin2;
	vec3_t neworigin2;
	vec3_t primalvelocity;
	trace_t trace;
	trace_t trace2;
	trace_t trace3;
	CL_ClientMovement_UpdateStatus(s);
	VectorCopy(s->velocity, primalvelocity);
	for (bump = 0, t = s->q.frametime;bump < 8 && VectorLength2(s->velocity) > 0;bump++)
	{
		VectorMA(s->origin, t, s->velocity, neworigin);
		trace = CL_Move(s->origin, s->mins, s->maxs, neworigin, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false);
		if (trace.fraction < 1 && trace.plane.normal[2] == 0)
		{
			// may be a step or wall, try stepping up
			// first move forward at a higher level
			VectorSet(currentorigin2, s->origin[0], s->origin[1], s->origin[2] + s->movevars_stepheight);
			VectorSet(neworigin2, neworigin[0], neworigin[1], s->origin[2] + s->movevars_stepheight);
			trace2 = CL_Move(currentorigin2, s->mins, s->maxs, neworigin2, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false);
			if (!trace2.startsolid)
			{
				// then move down from there
				VectorCopy(trace2.endpos, currentorigin2);
				VectorSet(neworigin2, trace2.endpos[0], trace2.endpos[1], s->origin[2]);
				trace3 = CL_Move(currentorigin2, s->mins, s->maxs, neworigin2, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false);
				//Con_Printf("%f %f %f %f : %f %f %f %f : %f %f %f %f\n", trace.fraction, trace.endpos[0], trace.endpos[1], trace.endpos[2], trace2.fraction, trace2.endpos[0], trace2.endpos[1], trace2.endpos[2], trace3.fraction, trace3.endpos[0], trace3.endpos[1], trace3.endpos[2]);
				// accept the new trace if it made some progress
				if (dr_fabs(trace3.endpos[0] - trace.endpos[0]) >= 0.03125 || dr_fabs(trace3.endpos[1] - trace.endpos[1]) >= 0.03125)
				{
					trace = trace2;
					VectorCopy(trace3.endpos, trace.endpos);
				}
			}
		}

		// check if it moved at all
		if (trace.fraction >= 0.001)
			VectorCopy(trace.endpos, s->origin);

		// check if it moved all the way
		if (trace.fraction == 1)
			break;

		//if (trace.plane.normal[2] > 0.7)
		//	s->onground = true;

		t -= t * trace.fraction;

		f = DotProduct(s->velocity, trace.plane.normal);
		VectorMA(s->velocity, -f, trace.plane.normal, s->velocity);
	}
	if (s->waterjumptime > 0)
		VectorCopy(primalvelocity, s->velocity);
}


void CL_ClientMovement_Physics_Swim(cl_clientmovement_state_t *s)
{
	vec_t wishspeed;
	vec_t f;
	vec3_t wishvel;
	vec3_t wishdir;

	// water jump only in certain situations
	// this mimics quakeworld code
	if (s->q.jump && s->waterlevel == 2 && s->velocity[2] >= -180)
	{
		vec3_t forward;
		vec3_t yawangles;
		vec3_t spot;
		VectorSet(yawangles, 0, s->q.viewangles[1], 0);
		AngleVectors(yawangles, forward, NULL, NULL);
		VectorMA(s->origin, 24, forward, spot);
		spot[2] += 8;
		if (CL_Move(spot, vec3_origin, vec3_origin, spot, MOVE_NOMONSTERS, NULL, 0, true, false, NULL, false).startsolid)
		{
			spot[2] += 24;
			if (!CL_Move(spot, vec3_origin, vec3_origin, spot, MOVE_NOMONSTERS, NULL, 0, true, false, NULL, false).startsolid)
			{
				VectorScale(forward, 50, s->velocity);
				s->velocity[2] = 310;
				s->waterjumptime = 2;
				s->onground = false;
				s->canjump = false;
			}
		}
	}

	if (!VectorLength2(s->q.move))
	{
		// drift towards bottom
		VectorSet(wishvel, 0, 0, -60);
	}
	else
	{
		// swim
		vec3_t forward;
		vec3_t right;
		vec3_t up;
		// calculate movement vector
		AngleVectors(s->q.viewangles, forward, right, up);
		VectorSet(up, 0, 0, 1);
		VectorMAMAM(s->q.move[0], forward, s->q.move[1], right, s->q.move[2], up, wishvel);
	}

	// split wishvel into wishspeed and wishdir
	wishspeed = VectorLength(wishvel);
	if (wishspeed)
		VectorScale(wishvel, 1 / wishspeed, wishdir);
	else
		VectorSet( wishdir, 0.0, 0.0, 0.0 );
	wishspeed = min(wishspeed, s->movevars_maxspeed) * 0.7;

	if (s->crouched)
		wishspeed *= 0.5;

	if (s->waterjumptime <= 0)
	{
		// water friction
		f = 1 - s->q.frametime * s->movevars_waterfriction * s->waterlevel;
		f = bound(0, f, 1);
		VectorScale(s->velocity, f, s->velocity);

		// water acceleration
		f = wishspeed - DotProduct(s->velocity, wishdir);
		if (f > 0)
		{
			f = min(s->movevars_wateraccelerate * s->q.frametime * wishspeed, f);
			VectorMA(s->velocity, f, wishdir, s->velocity);
		}

		// holding jump button swims upward slowly
		if (s->q.jump)
		{
			if (s->watertype & SUPERCONTENTS_LAVA)
				s->velocity[2] =  50;
			else if (s->watertype & SUPERCONTENTS_SLIME)
				s->velocity[2] =  80;
			else
			{
#ifndef NO_GAMEMODE
				if (gamemode == GAME_NEXUIZ)
					s->velocity[2] = 200;
				else
#endif
					s->velocity[2] = 100;
			}
		}
	}

	CL_ClientMovement_Move(s);
}

void CL_ClientMovement_Physics_Walk(cl_clientmovement_state_t *s)
{
	vec_t friction;
	vec_t wishspeed;
	vec_t addspeed;
	vec_t accelspeed;
	vec_t f;
	vec3_t forward;
	vec3_t right;
	vec3_t up;
	vec3_t wishvel;
	vec3_t wishdir;
	vec3_t yawangles;
	trace_t trace;

	// jump if on ground with jump button pressed but only if it has been
	// released at least once since the last jump
	if (s->q.jump && s->onground)// && s->canjump) // FIXME: canjump doesn't work properly
	{
		s->velocity[2] += s->movevars_jumpvelocity;
		s->onground = false;
		s->canjump = false;
	}

	// calculate movement vector
	VectorSet(yawangles, 0, s->q.viewangles[1], 0);
	AngleVectors(yawangles, forward, right, up);
	VectorMAM(s->q.move[0], forward, s->q.move[1], right, wishvel);

	// split wishvel into wishspeed and wishdir
	wishspeed = VectorLength(wishvel);
	if (wishspeed)
		VectorScale(wishvel, 1 / wishspeed, wishdir);
	else
		VectorSet( wishdir, 0.0, 0.0, 0.0 );
	wishspeed = min(wishspeed, s->movevars_maxspeed);
	if (s->crouched)
		wishspeed *= 0.5;

	// check if onground
	if (s->onground)
	{
		// apply edge friction
		f = dr_sqrt(s->velocity[0] * s->velocity[0] + s->velocity[1] * s->velocity[1]);
		friction = s->movevars_friction;
		if (f > 0 && s->movevars_edgefriction != 1)
		{
			vec3_t neworigin2;
			vec3_t neworigin3;
			// note: QW uses the full player box for the trace, and yet still
			// uses s->origin[2] + s->mins[2], which is clearly an bug, but
			// this mimics it for compatibility
			VectorSet(neworigin2, s->origin[0] + s->velocity[0]*(16/f), s->origin[1] + s->velocity[1]*(16/f), s->origin[2] + s->mins[2]);
			VectorSet(neworigin3, neworigin2[0], neworigin2[1], neworigin2[2] - 34);
			if (cls.protocol == PROTOCOL_QUAKEWORLD)
				trace = CL_Move(neworigin2, s->mins, s->maxs, neworigin3, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false);
			else
				trace = CL_Move(neworigin2, vec3_origin, vec3_origin, neworigin3, MOVE_NORMAL, NULL, SUPERCONTENTS_SOLID | SUPERCONTENTS_BODY | SUPERCONTENTS_PLAYERCLIP, true, true, NULL, false);
			if (trace.fraction == 1 && !trace.startsolid)
				friction *= s->movevars_edgefriction;
		}
		// apply ground friction
		f = 1 - s->q.frametime * friction * ((f < s->movevars_stopspeed) ? (s->movevars_stopspeed / f) : 1);
		f = max(f, 0);
		VectorScale(s->velocity, f, s->velocity);
		addspeed = wishspeed - DotProduct(s->velocity, wishdir);
		if (addspeed > 0)
		{
			accelspeed = min(s->movevars_accelerate * s->q.frametime * wishspeed, addspeed);
			VectorMA(s->velocity, accelspeed, wishdir, s->velocity);
		}
		s->velocity[2] -= cl_gravity.value * s->q.frametime;
		if (cls.protocol == PROTOCOL_QUAKEWORLD)
			s->velocity[2] = 0;
		if (VectorLength2(s->velocity))
			CL_ClientMovement_Move(s);
	}
	else
	{
		if (s->waterjumptime <= 0)
		{
			vec_t f;
			vec_t vel_straight;
			vec_t vel_z;
			vec3_t vel_perpend;

			// apply air speed limit
			wishspeed = min(wishspeed, s->movevars_maxairspeed);

			/*
			addspeed = wishspeed - DotProduct(s->velocity, wishdir);
			if (addspeed > 0)
			{
				accelspeed = min(s->movevars_accelerate * s->q.frametime * wishspeed, addspeed);
				VectorMA(s->velocity, accelspeed, wishdir, s->velocity);
			}
			*/

			vel_straight = DotProduct(s->velocity, wishdir);
			vel_z = s->velocity[2];
			VectorMA(s->velocity, -vel_straight, wishdir, vel_perpend);
			vel_perpend[2] -= vel_z;

			f = wishspeed - vel_straight;
			if(f > 0)
				vel_straight += min(f, s->movevars_accelerate * s->q.frametime * wishspeed) * s->movevars_airaccel_qw;
			if(wishspeed > 0)
				vel_straight += min(wishspeed, s->movevars_accelerate * s->q.frametime * wishspeed) * (1 - s->movevars_airaccel_qw);

			VectorM(1 - (s->q.frametime * (wishspeed / s->movevars_maxairspeed) * s->movevars_airaccel_sideways_friction), vel_perpend, vel_perpend);

			VectorMA(vel_perpend, vel_straight, wishdir, s->velocity);
			s->velocity[2] += vel_z;
		}
		s->velocity[2] -= cl_gravity.value * s->q.frametime;
		CL_ClientMovement_Move(s);
	}
}

void CL_ClientMovement_PlayerMove(cl_clientmovement_state_t *s)
{
	//Con_Printf(" %f", frametime);
	if (!s->q.jump)
		s->canjump = true;
	s->waterjumptime -= s->q.frametime;
	CL_ClientMovement_UpdateStatus(s);
	if (s->waterlevel >= WATERLEVEL_SWIMMING)
		CL_ClientMovement_Physics_Swim(s);
	else
		CL_ClientMovement_Physics_Walk(s);
}

void CL_ClientMovement_Replay(void)
{
	int i;
	double totalmovetime;
	cl_clientmovement_state_t s;

	// set up starting state for the series of moves
	memset(&s, 0, sizeof(s));
	VectorCopy(cl.entities[cl.playerentity].state_current.origin, s.origin);
	VectorCopy(cl.mvelocity[0], s.velocity);
	s.crouched = true; // will be updated on first move
	s.canjump = cl.movement_replay_canjump;
	//Con_Printf("movement replay starting org %f %f %f vel %f %f %f\n", s.origin[0], s.origin[1], s.origin[2], s.velocity[0], s.velocity[1], s.velocity[2]);

	// set up movement variables
	if (cls.protocol == PROTOCOL_QUAKEWORLD)
	{
		s.movevars_gravity = cl.qw_movevars_gravity;
		s.movevars_stopspeed = cl.qw_movevars_stopspeed;
		s.movevars_maxspeed = cl.qw_movevars_maxspeed;
		s.movevars_spectatormaxspeed = cl.qw_movevars_spectatormaxspeed;
		s.movevars_accelerate = cl.qw_movevars_accelerate;
		s.movevars_airaccelerate = cl.qw_movevars_airaccelerate;
		s.movevars_wateraccelerate = cl.qw_movevars_wateraccelerate;
		s.movevars_friction = cl.qw_movevars_friction;
		s.movevars_waterfriction = cl.qw_movevars_waterfriction;
		s.movevars_entgravity = cl.qw_movevars_entgravity;
		s.movevars_jumpvelocity = cl_movement_jumpvelocity.value;
		s.movevars_edgefriction = cl_movement_edgefriction.value;
		s.movevars_maxairspeed = cl_movement_maxairspeed.value;
		s.movevars_stepheight = cl_movement_stepheight.value;
		s.movevars_airaccel_qw = 1.0;
		s.movevars_airaccel_sideways_friction = 0.0;
	}
	else
	{
		s.movevars_gravity = sv_gravity.value;
		s.movevars_stopspeed = cl_movement_stopspeed.value;
		s.movevars_maxspeed = cl_movement_maxspeed.value;
		s.movevars_spectatormaxspeed = cl_movement_maxspeed.value;
		s.movevars_accelerate = cl_movement_accelerate.value;
		s.movevars_airaccelerate = cl_movement_airaccelerate.value < 0 ? cl_movement_accelerate.value : cl_movement_airaccelerate.value;
		s.movevars_wateraccelerate = cl_movement_wateraccelerate.value < 0 ? cl_movement_accelerate.value : cl_movement_wateraccelerate.value;
		s.movevars_friction = cl_movement_friction.value;
		s.movevars_waterfriction = cl_movement_waterfriction.value < 0 ? cl_movement_friction.value : cl_movement_waterfriction.value;
		s.movevars_entgravity = 1;
		s.movevars_jumpvelocity = cl_movement_jumpvelocity.value;
		s.movevars_edgefriction = cl_movement_edgefriction.value;
		s.movevars_maxairspeed = cl_movement_maxairspeed.value;
		s.movevars_stepheight = cl_movement_stepheight.value;
		s.movevars_airaccel_qw = cl_movement_airaccel_qw.value;
		s.movevars_airaccel_sideways_friction = cl_movement_airaccel_sideways_friction.value;
	}

	totalmovetime = 0;
	for (i = 0;i < cl.movement_numqueue - 1;i++)
		totalmovetime += cl.movement_queue[i].frametime;
	cl.movement_predicted = totalmovetime * 1000.0 >= cl_movement_minping.value && cls.servermovesequence && (cl_movement.integer
#ifndef NO_DEMO
		       	&& !cls.demoplayback
#endif
		       	&& cls.signon == SIGNONS && cl.stats[STAT_HEALTH] > 0 && !cl.intermission);
	if (cl.movement_predicted)
	{
		//Con_Printf("%f: ", cl.movecmd[0].time);

		// replay the input queue to predict current location
		// note: this relies on the fact there's always one queue item at the end

		for (i = 0;i < cl.movement_numqueue;i++)
		{
			s.q = cl.movement_queue[i];
			// if a move is more than 50ms, do it as two moves (matching qwsv)
			if (s.q.frametime > 0.05)
			{
				s.q.frametime *= 0.5;
				CL_ClientMovement_PlayerMove(&s);
			}
			CL_ClientMovement_PlayerMove(&s);
		}
	}
	else
	{
		// get the first movement queue entry to know whether to crouch and such
		s.q = cl.movement_queue[0];
	}
	CL_ClientMovement_UpdateStatus(&s);

	if (cl.movement_replay)
	{
		cl.movement_replay = false;
		// update interpolation timestamps if time has passed
		if (cl.movecmd[0].time != cl.movecmd[1].time)
		{
			cl.movement_time[1] = cl.movecmd[1].time;
			cl.movement_time[0] = cl.movecmd[0].time;
			cl.movement_time[2] = cl.time;
			VectorCopy(cl.movement_origin, cl.movement_oldorigin);
			//VectorCopy(s.origin, cl.entities[cl.playerentity].state_current.origin);
			//VectorSet(cl.entities[cl.playerentity].state_current.angles, 0, cl.viewangles[1], 0);
		}

		// update the interpolation target position and velocity
		VectorCopy(s.origin, cl.movement_origin);
		VectorCopy(s.velocity, cl.movement_velocity);
	}

	// update the onground flag if appropriate
	if (cl.movement_predicted)
	{
		// when predicted we simply set the flag according to the UpdateStatus
		cl.onground = s.onground;
	}
	else
	{
		// when not predicted, cl.onground is cleared by cl_parse.c each time
		// an update packet is received, but can be forced on here to hide
		// server inconsistencies in the onground flag
		// (which mostly occur when stepping up stairs at very high framerates
		//  where after the step up the move continues forward and not
		//  downward so the ground is not detected)
		//
		// such onground inconsistencies can cause jittery gun bobbing and
		// stair smoothing, so we set onground if UpdateStatus says so
		if (s.onground)
			cl.onground = true;
	}

	// react to onground state changes (for gun bob)
	if (cl.onground)
	{
		if (!cl.oldonground)
			cl.hitgroundtime = cl.movecmd[0].time;
		cl.lastongroundtime = cl.movecmd[0].time;
	}
	cl.oldonground = cl.onground;
}

void QW_MSG_WriteDeltaUsercmd(sizebuf_t *buf, usercmd_t *from, usercmd_t *to)
{
	int bits;

	bits = 0;
	if (to->viewangles[0] != from->viewangles[0])
		bits |= QW_CM_ANGLE1;
	if (to->viewangles[1] != from->viewangles[1])
		bits |= QW_CM_ANGLE2;
	if (to->viewangles[2] != from->viewangles[2])
		bits |= QW_CM_ANGLE3;
	if (to->forwardmove != from->forwardmove)
		bits |= QW_CM_FORWARD;
	if (to->sidemove != from->sidemove)
		bits |= QW_CM_SIDE;
	if (to->upmove != from->upmove)
		bits |= QW_CM_UP;
	if (to->buttons != from->buttons)
		bits |= QW_CM_BUTTONS;
	if (to->impulse != from->impulse)
		bits |= QW_CM_IMPULSE;

	MSG_WriteByte(buf, bits);
	if (bits & QW_CM_ANGLE1)
		MSG_WriteAngle16i(buf, to->viewangles[0]);
	if (bits & QW_CM_ANGLE2)
		MSG_WriteAngle16i(buf, to->viewangles[1]);
	if (bits & QW_CM_ANGLE3)
		MSG_WriteAngle16i(buf, to->viewangles[2]);
	if (bits & QW_CM_FORWARD)
		MSG_WriteShort(buf, (int)to->forwardmove);
	if (bits & QW_CM_SIDE)
		MSG_WriteShort(buf, (int)to->sidemove);
	if (bits & QW_CM_UP)
		MSG_WriteShort(buf, (int)to->upmove);
	if (bits & QW_CM_BUTTONS)
		MSG_WriteByte(buf, to->buttons);
	if (bits & QW_CM_IMPULSE)
		MSG_WriteByte(buf, to->impulse);
	MSG_WriteByte(buf, to->msec);
}

/*
==============
CL_SendMove
==============
*/
usercmd_t nullcmd; // for delta compression of qw moves
void CL_SendMove(void)
{
	int i, j, packetloss, maxusercmds;
	int bits;
	sizebuf_t buf;
	unsigned char data[1024];
	static double lastsendtime = 0;
	double packettime;
	double msectime;
	static double oldmsectime;

	// if playing a demo, do nothing
	if (!cls.netcon)
		return;

	// don't send too often or else network connections can get clogged by a high renderer framerate
	packettime = 1.0 / bound(10, cl_netinputpacketspersecond.value, 100);
	// quakeworld servers take only frametimes
	// predicted dp7 servers take current interpolation time
	// unpredicted servers take an echo of the latest server timestamp
	if (cls.protocol == PROTOCOL_QUAKEWORLD)
	{
		if (realtime < lastsendtime + packettime)
			return;
		cl.cmd.time = realtime;
	}
	else if (cl.movement_predicted)
	{
		if (realtime < lastsendtime + packettime)
			return;
		cl.cmd.time = cl.time;
	}
	else
	{
		// unpredicted movement should be sent immediately whenever a server
		// packet is received, to minimize ping times
		if (!cl.movement_needupdate && realtime < lastsendtime + packettime)
			return;
		cl.cmd.time = cl.mtime[0];
	}
	// don't let it fall behind if CL_SendMove hasn't been called recently
	// (such is the case when framerate is too low for instance)
	lastsendtime = bound(realtime, lastsendtime + packettime, realtime + packettime);
	// set the flag indicating that we sent a packet recently
	cl.movement_needupdate = false;


	buf.maxsize = sizeof(data);
	buf.cursize = 0;
	buf.data = data;

	// conditions for sending a move:
	// if the move advances time or if the game is paused (in which case time
	// is not advancing)
	if ((cl.cmd.time > cl.movecmd[0].time || cl.mtime[0] <= cl.mtime[1]) && cls.signon == SIGNONS)
	{
		// send the movement message
		// PROTOCOL_QUAKE        clc_move = 16 bytes total
		// PROTOCOL_QUAKEDP      clc_move = 16 bytes total
		// PROTOCOL_NEHAHRAMOVIE clc_move = 16 bytes total
		// PROTOCOL_DARKPLACES1  clc_move = 19 bytes total
		// PROTOCOL_DARKPLACES2  clc_move = 25 bytes total
		// PROTOCOL_DARKPLACES3  clc_move = 25 bytes total
		// PROTOCOL_DARKPLACES4  clc_move = 19 bytes total
		// PROTOCOL_DARKPLACES5  clc_move = 19 bytes total
		// PROTOCOL_DARKPLACES6  clc_move = 52 bytes total
		// PROTOCOL_DARKPLACES7  clc_move = 56 bytes total per move (can be up to 16 moves)
		// PROTOCOL_QUAKEWORLD   clc_move = 34 bytes total (typically, but can reach 43 bytes, or even 49 bytes with roll)

		// set button bits
		// LordHavoc: added 6 new buttons and use and chat buttons, and prydon cursor active button
		bits = 0;
		if (in_attack.state   & 3) bits |=   1;in_attack.state  &= ~2;
		if (in_jump.state     & 3) bits |=   2;in_jump.state    &= ~2;
		if (in_button3.state  & 3) bits |=   4;in_button3.state &= ~2;
		if (in_button4.state  & 3) bits |=   8;in_button4.state &= ~2;
		if (in_button5.state  & 3) bits |=  16;in_button5.state &= ~2;
		if (in_button6.state  & 3) bits |=  32;in_button6.state &= ~2;
		if (in_button7.state  & 3) bits |=  64;in_button7.state &= ~2;
		if (in_button8.state  & 3) bits |= 128;in_button8.state &= ~2;
		if (in_use.state      & 3) bits |= 256;in_use.state     &= ~2;
		if (key_dest != key_game || key_consoleactive) bits |= 512;
		if (cl_prydoncursor.integer) bits |= 1024;
		if (in_button9.state  & 3)  bits |=   2048;in_button9.state  &= ~2;
		if (in_button10.state  & 3) bits |=   4096;in_button10.state &= ~2;
		if (in_button11.state  & 3) bits |=   8192;in_button11.state &= ~2;
		if (in_button12.state  & 3) bits |=  16384;in_button12.state &= ~2;
		if (in_button13.state  & 3) bits |=  32768;in_button13.state &= ~2;
		if (in_button14.state  & 3) bits |=  65536;in_button14.state &= ~2;
		if (in_button15.state  & 3) bits |= 131072;in_button15.state &= ~2;
		if (in_button16.state  & 3) bits |= 262144;in_button16.state &= ~2;
		// button bits 19-31 unused currently
		// rotate/zoom view serverside if PRYDON_CLIENTCURSOR cursor is at edge of screen
		if (cl.cmd.cursor_screen[0] <= -1) bits |= 8;
		if (cl.cmd.cursor_screen[0] >=  1) bits |= 16;
		if (cl.cmd.cursor_screen[1] <= -1) bits |= 32;
		if (cl.cmd.cursor_screen[1] >=  1) bits |= 64;
		cl.cmd.buttons = bits;

		// set impulse
		cl.cmd.impulse = in_impulse;
		in_impulse = 0;

		// movement is set by input code (forwardmove/sidemove/upmove)

		// set viewangles
		VectorCopy(cl.viewangles, cl.cmd.viewangles);

		msectime = dr_floor(cl.cmd.time * 1000);
		cl.cmd.msec = (unsigned char)bound(0, msectime - oldmsectime, 255);
		// ridiculous value rejection (matches qw)
		if (cl.cmd.msec > 250)
			cl.cmd.msec = 100;
		oldmsectime = msectime;

		cls.movesequence++;
		if (cl_movement.integer)
			cl.cmd.sequence = cls.movesequence;
		else
			cl.cmd.sequence = 0;

		// set prydon cursor info
		CL_UpdatePrydonCursor();

		// always dump the first two messages, because they may contain leftover inputs from the last level
		if (cls.movesequence > 2)
		{
			// update the cl.movecmd array which holds the most recent moves
			for (i = CL_MAX_USERCMDS - 1;i >= 1;i--)
				cl.movecmd[i] = cl.movecmd[i-1];
			cl.movecmd[0] = cl.cmd;

			// set the maxusercmds variable to limit how many should be sent
			if (cls.protocol == PROTOCOL_QUAKEWORLD)
			{
				// qw uses exactly 3 moves
				maxusercmds = 3;
			}
			else
			{
				// configurable number of unacknowledged moves
				maxusercmds = bound(1, cl_netinputpacketlosstolerance.integer + 1, CL_MAX_USERCMDS);
				// when movement prediction is off, there's not much point in repeating old input as it will just be ignored
				if (!cl.cmd.sequence)
					maxusercmds = 1;
			}

			if (cls.protocol == PROTOCOL_QUAKEWORLD)
			{
				int checksumindex;

				CL_ClientMovement_InputQW();

				MSG_WriteByte(&buf, qw_clc_move);
				// save the position for a checksum byte
				checksumindex = buf.cursize;
				MSG_WriteByte(&buf, 0);
				// packet loss percentage
				for (j = 0, packetloss = 0;j < 100;j++)
					packetloss += cls.netcon->packetlost[j];
				MSG_WriteByte(&buf, packetloss);
				// write most recent 3 moves
				QW_MSG_WriteDeltaUsercmd(&buf, &nullcmd, &cl.movecmd[2]);
				QW_MSG_WriteDeltaUsercmd(&buf, &cl.movecmd[2], &cl.movecmd[1]);
				QW_MSG_WriteDeltaUsercmd(&buf, &cl.movecmd[1], &cl.movecmd[0]);
				// calculate the checksum
				buf.data[checksumindex] = COM_BlockSequenceCRCByteQW(buf.data + checksumindex + 1, buf.cursize - checksumindex - 1, cls.netcon->qw.outgoing_sequence);
				// if delta compression history overflows, request no delta
				if (cls.netcon->qw.outgoing_sequence - cl.qw_validsequence >= QW_UPDATE_BACKUP-1)
					cl.qw_validsequence = 0;
				// request delta compression if appropriate
				if (cl.qw_validsequence && !cl_nodelta.integer && cls.state == ca_connected
#ifndef NO_DEMO
					       	&& !cls.demorecording
#endif
				   )
				{
					cl.qw_deltasequence[cls.netcon->qw.outgoing_sequence & QW_UPDATE_MASK] = cl.qw_validsequence;
					MSG_WriteByte(&buf, qw_clc_delta);
					MSG_WriteByte(&buf, cl.qw_validsequence & 255);
				}
				else
					cl.qw_deltasequence[cls.netcon->qw.outgoing_sequence & QW_UPDATE_MASK] = -1;
			}
			else if (cls.protocol == PROTOCOL_QUAKE || cls.protocol == PROTOCOL_QUAKEDP || cls.protocol == PROTOCOL_NEHAHRAMOVIE)
			{
				CL_ClientMovement_Input((cl.movecmd[0].buttons & 2) != 0, false);

				// 5 bytes
				MSG_WriteByte (&buf, clc_move);
				MSG_WriteFloat (&buf, cl.movecmd[0].time); // last server packet time
				// 3 bytes
				for (i = 0;i < 3;i++)
					MSG_WriteAngle8i (&buf, cl.movecmd[0].viewangles[i]);
				// 6 bytes
				MSG_WriteCoord16i (&buf, cl.movecmd[0].forwardmove);
				MSG_WriteCoord16i (&buf, cl.movecmd[0].sidemove);
				MSG_WriteCoord16i (&buf, cl.movecmd[0].upmove);
				// 2 bytes
				MSG_WriteByte (&buf, cl.movecmd[0].buttons);
				MSG_WriteByte (&buf, cl.movecmd[0].impulse);
			}
			else if (cls.protocol == PROTOCOL_DARKPLACES2 || cls.protocol == PROTOCOL_DARKPLACES3)
			{
				CL_ClientMovement_Input((cl.movecmd[0].buttons & 2) != 0, false);

				// 5 bytes
				MSG_WriteByte (&buf, clc_move);
				MSG_WriteFloat (&buf, cl.movecmd[0].time); // last server packet time
				// 12 bytes
				for (i = 0;i < 3;i++)
					MSG_WriteAngle32f (&buf, cl.movecmd[0].viewangles[i]);
				// 6 bytes
				MSG_WriteCoord16i (&buf, cl.movecmd[0].forwardmove);
				MSG_WriteCoord16i (&buf, cl.movecmd[0].sidemove);
				MSG_WriteCoord16i (&buf, cl.movecmd[0].upmove);
				// 2 bytes
				MSG_WriteByte (&buf, cl.movecmd[0].buttons);
				MSG_WriteByte (&buf, cl.movecmd[0].impulse);
			}
			else if (cls.protocol == PROTOCOL_DARKPLACES1 || cls.protocol == PROTOCOL_DARKPLACES4 || cls.protocol == PROTOCOL_DARKPLACES5)
			{
				CL_ClientMovement_Input((cl.movecmd[0].buttons & 2) != 0, false);

				// 5 bytes
				MSG_WriteByte (&buf, clc_move);
				MSG_WriteFloat (&buf, cl.movecmd[0].time); // last server packet time
				// 6 bytes
				for (i = 0;i < 3;i++)
					MSG_WriteAngle16i (&buf, cl.movecmd[0].viewangles[i]);
				// 6 bytes
				MSG_WriteCoord16i (&buf, cl.movecmd[0].forwardmove);
				MSG_WriteCoord16i (&buf, cl.movecmd[0].sidemove);
				MSG_WriteCoord16i (&buf, cl.movecmd[0].upmove);
				// 2 bytes
				MSG_WriteByte (&buf, cl.movecmd[0].buttons);
				MSG_WriteByte (&buf, cl.movecmd[0].impulse);
			}
			else
			{
				usercmd_t *cmd;
				// FIXME: cl.movecmd[0].buttons & 16 is +button5, Nexuiz specific
				CL_ClientMovement_Input((cl.movecmd[0].buttons & 2) != 0, (cl.movecmd[0].buttons & 16) != 0);

				// send the latest moves in order, the old ones will be
				// ignored by the server harmlessly, however if the previous
				// packets were lost these moves will be used
				//
				// this reduces packet loss impact on gameplay.
				for (j = 0, cmd = &cl.movecmd[maxusercmds-1];j < maxusercmds;j++, cmd--)
				{
					// don't repeat any stale moves
					if (cmd->sequence && cmd->sequence < cls.servermovesequence)
						continue;
					// 5/9 bytes
					MSG_WriteByte (&buf, clc_move);
					if (cls.protocol != PROTOCOL_DARKPLACES6)
						MSG_WriteLong (&buf, cmd->sequence);
					MSG_WriteFloat (&buf, cmd->time); // last server packet time
					// 6 bytes
					for (i = 0;i < 3;i++)
						MSG_WriteAngle16i (&buf, cmd->viewangles[i]);
					// 6 bytes
					MSG_WriteCoord16i (&buf, cmd->forwardmove);
					MSG_WriteCoord16i (&buf, cmd->sidemove);
					MSG_WriteCoord16i (&buf, cmd->upmove);
					// 5 bytes
					MSG_WriteLong (&buf, cmd->buttons);
					MSG_WriteByte (&buf, cmd->impulse);
					// PRYDON_CLIENTCURSOR
					// 30 bytes
					MSG_WriteShort (&buf, (short)(cmd->cursor_screen[0] * 32767.0f));
					MSG_WriteShort (&buf, (short)(cmd->cursor_screen[1] * 32767.0f));
					MSG_WriteFloat (&buf, cmd->cursor_start[0]);
					MSG_WriteFloat (&buf, cmd->cursor_start[1]);
					MSG_WriteFloat (&buf, cmd->cursor_start[2]);
					MSG_WriteFloat (&buf, cmd->cursor_impact[0]);
					MSG_WriteFloat (&buf, cmd->cursor_impact[1]);
					MSG_WriteFloat (&buf, cmd->cursor_impact[2]);
					MSG_WriteShort (&buf, cmd->cursor_entitynumber);
				}
			}
		}
	}

	if (cls.protocol != PROTOCOL_QUAKEWORLD)
	{
		// ack the last few frame numbers
		// (redundent to improve handling of client->server packet loss)
		// for LATESTFRAMENUMS == 3 case this is 15 bytes
		for (i = 0;i < LATESTFRAMENUMS;i++)
		{
			if (cl.latestframenums[i] > 0)
			{
#ifndef NO_DEVELOPER
				if (developer_networkentities.integer >= 1)
					Con_Printf("send clc_ackframe %i\n", cl.latestframenums[i]);
#endif
				MSG_WriteByte(&buf, clc_ackframe);
				MSG_WriteLong(&buf, cl.latestframenums[i]);
			}
		}
	}

	// PROTOCOL_DARKPLACES6 = 67 bytes per packet
	// PROTOCOL_DARKPLACES7 = 71 bytes per packet

	if (cls.protocol != PROTOCOL_QUAKEWORLD)
	{
		// acknowledge any recently received data blocks
		for (i = 0;i < CL_MAX_DOWNLOADACKS && (cls.dp_downloadack[i].start || cls.dp_downloadack[i].size);i++)
		{
			MSG_WriteByte(&buf, clc_ackdownloaddata);
			MSG_WriteLong(&buf, cls.dp_downloadack[i].start);
			MSG_WriteShort(&buf, cls.dp_downloadack[i].size);
			cls.dp_downloadack[i].start = 0;
			cls.dp_downloadack[i].size = 0;
		}
	}

	// send the reliable message (forwarded commands) if there is one
	NetConn_SendUnreliableMessage(cls.netcon, &buf, cls.protocol);

	if (cls.netcon->message.overflowed)
	{
		Con_Print("CL_SendMove: lost server connection\n");
		CL_Disconnect();
		Host_ShutdownServer();
	}
}

/*
============
CL_InitInput
============
*/
void CL_InitInput (void)
{
	Cmd_AddCommand ("+moveup",IN_UpDown, "swim upward");
	Cmd_AddCommand ("-moveup",IN_UpUp, "stop swimming upward");
	Cmd_AddCommand ("+movedown",IN_DownDown, "swim downward");
	Cmd_AddCommand ("-movedown",IN_DownUp, "stop swimming downward");
	Cmd_AddCommand ("+left",IN_LeftDown, "turn left");
	Cmd_AddCommand ("-left",IN_LeftUp, "stop turning left");
	Cmd_AddCommand ("+right",IN_RightDown, "turn right");
	Cmd_AddCommand ("-right",IN_RightUp, "stop turning right");
	Cmd_AddCommand ("+forward",IN_ForwardDown, "move forward");
	Cmd_AddCommand ("-forward",IN_ForwardUp, "stop moving forward");
	Cmd_AddCommand ("+back",IN_BackDown, "move backward");
	Cmd_AddCommand ("-back",IN_BackUp, "stop moving backward");
	Cmd_AddCommand ("+lookup", IN_LookupDown, "look upward");
	Cmd_AddCommand ("-lookup", IN_LookupUp, "stop looking upward");
	Cmd_AddCommand ("+lookdown", IN_LookdownDown, "look downward");
	Cmd_AddCommand ("-lookdown", IN_LookdownUp, "stop looking downward");
	Cmd_AddCommand ("+strafe", IN_StrafeDown, "activate strafing mode (move instead of turn)");
	Cmd_AddCommand ("-strafe", IN_StrafeUp, "deactivate strafing mode");
	Cmd_AddCommand ("+moveleft", IN_MoveleftDown, "strafe left");
	Cmd_AddCommand ("-moveleft", IN_MoveleftUp, "stop strafing left");
	Cmd_AddCommand ("+moveright", IN_MoverightDown, "strafe right");
	Cmd_AddCommand ("-moveright", IN_MoverightUp, "stop strafing right");
	Cmd_AddCommand ("+speed", IN_SpeedDown, "activate run mode (faster movement and turning)");
	Cmd_AddCommand ("-speed", IN_SpeedUp, "deactivate run mode");
	Cmd_AddCommand ("+attack", IN_AttackDown, "begin firing");
	Cmd_AddCommand ("-attack", IN_AttackUp, "stop firing");
	Cmd_AddCommand ("+jump", IN_JumpDown, "jump");
	Cmd_AddCommand ("-jump", IN_JumpUp, "end jump (so you can jump again)");
	Cmd_AddCommand ("impulse", IN_Impulse, "send an impulse number to server (select weapon, use item, etc)");
	Cmd_AddCommand ("+klook", IN_KLookDown, "activate keyboard looking mode, do not recenter view");
	Cmd_AddCommand ("-klook", IN_KLookUp, "deactivate keyboard looking mode");
	Cmd_AddCommand ("+mlook", IN_MLookDown, "activate mouse looking mode, do not recenter view");
	Cmd_AddCommand ("-mlook", IN_MLookUp, "deactivate mouse looking mode");

	// LordHavoc: added use button
	Cmd_AddCommand ("+use", IN_UseDown, "use something (may be used by some mods)");
	Cmd_AddCommand ("-use", IN_UseUp, "stop using something");

	// LordHavoc: added 6 new buttons
	Cmd_AddCommand ("+button3", IN_Button3Down, "activate button3 (behavior depends on mod)");
	Cmd_AddCommand ("-button3", IN_Button3Up, "deactivate button3");
	Cmd_AddCommand ("+button4", IN_Button4Down, "activate button4 (behavior depends on mod)");
	Cmd_AddCommand ("-button4", IN_Button4Up, "deactivate button4");
	Cmd_AddCommand ("+button5", IN_Button5Down, "activate button5 (behavior depends on mod)");
	Cmd_AddCommand ("-button5", IN_Button5Up, "deactivate button5");
	Cmd_AddCommand ("+button6", IN_Button6Down, "activate button6 (behavior depends on mod)");
	Cmd_AddCommand ("-button6", IN_Button6Up, "deactivate button6");
	Cmd_AddCommand ("+button7", IN_Button7Down, "activate button7 (behavior depends on mod)");
	Cmd_AddCommand ("-button7", IN_Button7Up, "deactivate button7");
	Cmd_AddCommand ("+button8", IN_Button8Down, "activate button8 (behavior depends on mod)");
	Cmd_AddCommand ("-button8", IN_Button8Up, "deactivate button8");
	Cmd_AddCommand ("+button9", IN_Button9Down, "activate button9 (behavior depends on mod)");
	Cmd_AddCommand ("-button9", IN_Button9Up, "deactivate button9");
	Cmd_AddCommand ("+button10", IN_Button10Down, "activate button10 (behavior depends on mod)");
	Cmd_AddCommand ("-button10", IN_Button10Up, "deactivate button10");
	Cmd_AddCommand ("+button11", IN_Button11Down, "activate button11 (behavior depends on mod)");
	Cmd_AddCommand ("-button11", IN_Button11Up, "deactivate button11");
	Cmd_AddCommand ("+button12", IN_Button12Down, "activate button12 (behavior depends on mod)");
	Cmd_AddCommand ("-button12", IN_Button12Up, "deactivate button12");
	Cmd_AddCommand ("+button13", IN_Button13Down, "activate button13 (behavior depends on mod)");
	Cmd_AddCommand ("-button13", IN_Button13Up, "deactivate button13");
	Cmd_AddCommand ("+button14", IN_Button14Down, "activate button14 (behavior depends on mod)");
	Cmd_AddCommand ("-button14", IN_Button14Up, "deactivate button14");
	Cmd_AddCommand ("+button15", IN_Button15Down, "activate button15 (behavior depends on mod)");
	Cmd_AddCommand ("-button15", IN_Button15Up, "deactivate button15");
	Cmd_AddCommand ("+button16", IN_Button16Down, "activate button16 (behavior depends on mod)");
	Cmd_AddCommand ("-button16", IN_Button16Up, "deactivate button16");

	// LordHavoc: added bestweapon command
	Cmd_AddCommand ("bestweapon", IN_BestWeapon, "send an impulse number to server to select the first usable weapon out of several (example: 8 7 6 5 4 3 2 1)");

	Cvar_RegisterVariable(&cl_movement);
	Cvar_RegisterVariable(&cl_movement_minping);
	Cvar_RegisterVariable(&cl_movement_maxspeed);
	Cvar_RegisterVariable(&cl_movement_maxairspeed);
	Cvar_RegisterVariable(&cl_movement_stopspeed);
	Cvar_RegisterVariable(&cl_movement_friction);
	Cvar_RegisterVariable(&cl_movement_edgefriction);
	Cvar_RegisterVariable(&cl_movement_stepheight);
	Cvar_RegisterVariable(&cl_movement_airaccelerate);
	Cvar_RegisterVariable(&cl_movement_accelerate);
	Cvar_RegisterVariable(&cl_movement_jumpvelocity);
	Cvar_RegisterVariable(&cl_movement_airaccel_qw);
	Cvar_RegisterVariable(&cl_movement_airaccel_sideways_friction);
	Cvar_RegisterVariable(&cl_gravity);
	Cvar_RegisterVariable(&cl_slowmo);

	Cvar_RegisterVariable(&in_pitch_min);
	Cvar_RegisterVariable(&in_pitch_max);
	Cvar_RegisterVariable(&m_filter);

	Cvar_RegisterVariable(&cl_netinputpacketspersecond);
	Cvar_RegisterVariable(&cl_netinputpacketlosstolerance);

	Cvar_RegisterVariable(&cl_nodelta);
}

