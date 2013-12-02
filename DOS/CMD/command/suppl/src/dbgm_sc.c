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

ob(ject): suppl_calloc
su(bsystem): debug
ty(pe): 
sh(ort description): Perform a monitored \tok{calloc()} action
he(ader files): 
lo(ng description): If memory monitoring is enabled (SUPPL_LOG_MEMORY), the
	standard \tok{calloc()} function is replaced by this function via a
	macro. This function calls \tok{suppl_log_calloc()} to write the stats
	into the syslog.
pr(erequistes): 
va(lue): Return value of monitored \tok{calloc()}
re(lated to): suppl_dbg_change syslog
se(condary subsystems): 
in(itialized by): 
wa(rning): 
bu(gs): 
fi(le): dbgm_sc.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#include <stdlib.h>
#endif
#include <portable.h>

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id: dbgm_sc.c 1210 2006-06-17 03:25:06Z blairdude $";
#endif

#undef calloc
void *suppl_calloc(const size_t nelem, const size_t size)
{	void *p;

	chkHeap
	suppl_log_calloc(p = calloc(nelem, size), nelem, size);
	chkHeap

	return p;
}
