/*
	Copyright (C) 2006  Serge "(515)" Ziryukin

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

#ifndef PNG_H
#define PNG_H

#ifndef NO_LIBPNG

qboolean PNG_OpenLibrary (void);
void PNG_CloseLibrary (void);

#else
#define PNG_OpenLibrary() false
#define PNG_CloseLibrary()
#endif

unsigned char* PNG_LoadImage (const unsigned char *f, int filesize, int matchwidth, int matchheight);

#endif

