/*
	@(#)Segment.h	2.4
    	Copyright 1997 Willows Software, Inc. 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.


For more information about the Willows Twin Libraries.

	http://www.willows.com	

To send email to the maintainer of the Willows Twin Libraries.

	mailto:twin@willows.com 

*/
 

#ifndef Segment__h
#define Segment__h

typedef struct keySEGIMAGE
{   unsigned long conv;
    unsigned long targ;
} SEGIMAGE;

/*
 *	the following is used to describe normal 16 and 32 bit code and
 *	data segments. The native world calls an interface routine to 
 *	setup the call to the code. Either through an interpreter or
 *	through direct execution we can get to the following.  The
 *	DATAxx segments are to allow simple traps to catch executing data.
 */
#define TRANSFER_CODE16		1	/* 16bit code segment */
#define TRANSFER_DATA		2	/* 16bit data segment */
#define TRANSFER_CODE32		3	/* 32bit code segment */
#define TRANSFER_DATA32		4	/* 16bit data segment */

/*
 *	this corresponds to a native code segment, which has an alias
 *	CODE16 segment for it.  On transfers to it, we make the transition
 *	from the binary world to the native world.
 */
#define TRANSFER_CALLBACK 	5	/* alias for native code segment */
#define TRANSFER_NATIVE 	6	/* a real native segment */

/*
 *	the following segments are used to map native library addresses 
 *	to thunks that can be called by the application. 
 */
#define TRANSFER_BINARY		7	/* native code segment (pseudo-code) */
#define TRANSFER_RETURN		8	/* native code return (pseudo-code)  */

#endif /* Segment__h */
