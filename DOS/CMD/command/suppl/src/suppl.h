/*
    This file is part of SUPPL - the supplemental library for DOS
    Copyright (C) 1996-2000 Steffen Kaiser

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* $Id: suppl.h 1210 2006-06-17 03:25:06Z blairdude $
   $Locker$	$Name$	$State$

	Declarations for the supplemental functions.

	This header file is part of the "portable" subsystem
	and will be removed, or moved to another purpose.

*/

#ifndef __SUPPL_H
#define __SUPPL_H

#include <stdio.h>

#include <supl_def.h>

#include <portable.h>

#define itemsof(array)	(sizeof(array) / sizeof(array[0]))
#define NIL(type)		((type)0)

/* length of the longmath numbers. */
#define LSIZE sizeof(dword)

#ifndef _MICROC_

#define longsubi(a,b) ((a) -= (int)(b))
#define longsubu(a,b) ((a) -= (unsigned)(b))
#define longaddi(a,b) ((a) += (int)(b))
#define longaddu(a,b) ((a) += (unsigned)(b))
#define longmuli(a,b) ((a) *= (int)(b))
#define longmulu(a,b) ((a) *= (unsigned)(b))
#define longdivi(a,b) ((a) /= (int)(b))
#define longdivu(a,b) ((a) /= (unsigned)(b))
#define longseti(a,b) ((a) = (int)(b))
#define longsetu(a,b) ((a) = (unsigned)(b))
#define longshln(a,c) ((a) <<= (c))
#define longshrn(a,c) ((a) >>= (c))
int longcmp(long d1, long d2);
#define longcmp_(a,b) ((signed long)(a) - (signed long)(b))
#define longsgn(a)    ((signed long)(a))
#define longclr(a)	  ((a) = 0)

#define poked(seg,ofs,value)	(*(dword far*)MK_FP(seg,ofs) = (value))
#define pokedd(seg,ofs,hi,lo)	(*(dword far*)MK_FP(seg,ofs) = ((dword)(hi)<<16)|(lo))
#define peekd(segm,ofs,value)	((value) = *(dword far*)MK_FP(segm,ofs))
int peekdd(word segm, word ofs, word *valHi, word *valLo);

typedef char far *fartbl;		/* far table */
#define dpeekc(fartable,idx)	(*(((char far *)fartable) + (idx)))
#define dpeekb(fartable,idx)	(*(((byte far *)fartable) + (idx)))
#define dpeekw(fartable,idx)	(*(word far*)(((byte far *)fartable) + (idx)))

#define _fmemcmpi _fmemicmp
#define _fstrcmpi _fstricmp
#define memcmpi memicmp

#ifdef _PAC_NOCLIB_
int memicmp(const void *s1, const void *s2, unsigned len);
void *memccpy(void *s1, const void *s2, int ch, size_t len);
#endif

#else		/* defined(_MICROC_) */

#define longcmp_ longcmp
#define longsgn(a) (longtst(a) && ((int*)(a))[1])

#define fartbl dword		/* pointer to far table */
//char dpeekc(fartbl fartable, int idx);
#define dpeekc(tbl,idx) (char)dpeekb((tbl), (idx))
int dpeekb(fartbl fartable, int idx);
word dpeekw(fartbl fartable, int idx);
int memicmp(const void *s1, const void *s2, unsigned len);
void *memccpy(void *s1, const void *s2, int ch, size_t len);

#endif		/* !defined(_MICROC_) */



#define putcm fputcm
int fputmc(int ch, int cnt, FILE *f);
/*	Emit the same character several times into the stream
*/


word DOSalloc(const word length, const int mode);
/* Allocate a chunk of memory directly via the DOS API.
	length is the number of paragraphes that shall be allocated.
	mode is the allocation strategy to be used.

	If mode specifies to use high memory, the high memory will be
	automatically made accessable.

	If the bit 0x10 of mode is set, length is ignored and the size of
	the largest unallocated block is returned, which can be allocated
	later.

	This function does not alter the allocation strategy permanently.

	Return:
		0: Allocation failed
		else:	if 0x10 of mode was clear: segment address of allocated memory
				if 0x10 of mode was set: the largest unallocated block
*/

int DOSresize(const word segm, const word length);
/* Resize a chunk of memory directly via the DOS API.
	segm is the segment to resize, it must be allocated via the DOS API
		or a comparable function, like DOSalloc(), allocBlk().
	length is the new number of paragraphes.

	If to resize the segment fails, the block of memory is not relocated
	into a larger space of the DOS memory.

	Return:
		0: resize failed
		!0: resize OK
*/

int DOSfree(const word segm);
/* Free a chunk of memory directly via the DOS API.
	segm is the segment to resize, it must be allocated via the DOS API
		or a comparable function, like DOSalloc(), allocBlk().

	Return:
		0: free OK
		!0: DOS error code
*/

//word allocBlk(const unsigned length, const int allocMode);
#define allocBlk(length,allocMode) DOSalloc(BLK_byte2para(length), allocMode)
/* Allocates a block of memory using the DOSalloc() function.
	The only difference is the �length� parameter:
		Here it is the requested length of the block in bytes, thus,
		the block can contain only 0..65535 bytes (up to 64KB).
*/

#ifdef _MICROC_
word resizeBlk(const word segm, const unsigned length);
#else
#define resizeBlk(segm,length) DOSresize(segm, BLK_byte2para(length))
#endif
/* Resizes the segment using the DOSresize() function.
	The only difference is the �length� parameter:
		Here it is the requested length of the block in bytes, thus,
		the block can contain only 0..65535 bytes (up to 64KB).
*/

//int freeBlk(const word segm);
#define freeBlk DOSfree
/* Free the segment using the DOSfree() function.
	No differences, only for completeness.
*/

unsigned BLK_byte2para(const unsigned bytes);
/* Return the number of paragraphes that will be needed to store �bytes�
	bytes in.

	If bytes == 0, the return value is zero, too.

*/

int addu(unsigned *u1, unsigned u2);
/* Performs the operation:
		*u1 += u2;
	and returns, if an overflow occured.

	Return:	0: no overflow
			!0: overflow
*/
#ifdef __WATCOMC__
#ifdef FARDATA
#pragma aux addu = \
	"add word ptr es:[bx], ax"	\
	"sbb ax, ax"	\
	parm [es bx] [ax]	\
	value [ax]	\
	modify [es bx]
#else
#pragma aux addu = \
	"add word ptr [bx], ax"	\
	"sbb ax, ax"	\
	parm [bx] [ax]	\
	value [ax]	\
	modify [bx]
#endif
#endif

int invokeDOS(nM(IREGS *prp));
/* Invoke DOS using intr(0x21, prp).

	Return:
		 0: DOS returned with Carry flag cleared
		!0: AX value (DOS error code) if DOS returned with Carry flag set
*/

#ifndef _CLIB_
char *stpcat(char * const dst, const char * const src);
/* Append src to dst and return the address of the '\0' character of dst
*/
#endif


#ifdef __WATCOMC__
char *stpcpy(char * const dst, const char * const src);
/*	Copy src into dst
	Return pointer to '\0' terminator of dst
*/

/* different names */
struct ffblk {
    char        ff_reserved[21];
    byte        ff_attrib;
    unsigned    ff_ftime;
    unsigned    ff_fdate;
    long        ff_fsize;
    char        ff_name[13];
};

#endif	/* WATCOMC */


#ifdef _PAC_NOCLIB_
char *stpcpy(char * const dst, const char * const src);
/* Copy src onto dst, Return a pointer to the '\0' character of dst
*/

int getdisk(void);
/*	Return the current working drive. 0 == A:, 1 == B: ...
*/

int setdisk(void);
/*	Change the current working drive. 0 == A:, 1 == B: ...
	Returns the number of available drive letters (not necessary
	currently accessable).
*/

/*
 *	Pacific does not offer any OS version information
 */
unsigned osversion(void);
/*	Return: _osmajor | (_osminor << 8)
*/

/*
 *	No findfirst()
 */
struct ffblk {
    char        ff_reserved[21];
    byte        ff_attrib;
    unsigned    ff_ftime;
    unsigned    ff_fdate;
    long        ff_fsize;
    char        ff_name[13];
};

int findfirst(const char * const pattern, struct ffblk *ff, int attrib);
/*	Initiate file search
	Return:	0 on success
*/

int findnext(struct ffblk *ff);
/*	Next file search
	Return:	0 on success
*/

void far *getdta(void);
/*	Return current working DTA
*/

void setdta(void far *dta);
/*	Set current working DTA
*/

unsigned _psp_(void);
/*	Return the current PSP
*/

void ctrlbrk(int (*fct)(void));
/*	Set new ^Break handler
*/

#endif	/* _PAC_NOCLIB_ */

#ifdef COMPILE
#undef COMPILE
#endif

#ifdef _MICROC_
#define COMPILE
#endif
#ifdef _PAC_NOCLIB_
#define COMPILE
#endif
#ifdef __WATCOMC__
#define COMPILE
#endif
#ifdef _TC_EARLY_
#define COMPILE
#endif

#ifdef COMPILE
#undef COMPILE

char *_getdcwd(int drive, char *buf, unsigned length);
/*	Return the fully-qualified path of drive in the format:
		?:\...\...\...
	If buf == NULL, a buffer of length bytes is allocated.

	Return:
		NULL: On failure
		else: on success (buf or allocated buffer)
*/
#endif

int getbootdisk(void);
/*	as getdisk() but returns the boot drive
	Return:
		1 == A:, 2 == B:
		0: failure
*/

#endif
