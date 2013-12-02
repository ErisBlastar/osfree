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
/* $RCSfile$
   $Locker$	$Name$	$State$

ob(ject): eno_fstrdup
su(bsystem): error
ty(pe): HL
sy(nopsis): 
sh(ort description): as Strdup(), but assigns errno
he(ader files): 
lo(ng description): as Strdup(), but assigns errno = ENOMEM on failure
pr(erequistes): 
va(lue): NULL: \tok{\para{fnam} == NULL} or on out-of-memory
 \item else: dynamically allocated block of memory
re(lated to): strdup, Strdup, eno_malloc
se(condary subsystems): farmem
xr(ef): 
im(port): 
fi(le): 
in(itialized by): 
wa(rning): 
bu(gs): 
co(mpilers): 

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#include <string.h>
#endif
#include <portable.h>
#include <fmemory.h>

#include "eno.loc"

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id: enofsdup.c 1210 2006-06-17 03:25:06Z blairdude $";
#endif

#ifndef _MICROC_

char *eno_fstrdup(const char far * const s)
{
	DBG_ENTER("eno_fstrdup", Suppl_error)
	DBG_ARGUMENTS( ("s=%Fs", s) )

	if(s) {
		char *h = _fdupstr(s);
		if(h)
			DBG_RETURN_S(h)
		eno_set( ENOMEM);
	}

	DBG_RETURN_S(0)
}

#endif
