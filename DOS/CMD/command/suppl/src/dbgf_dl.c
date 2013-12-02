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

ob(ject): suppl_log_del_list_
su(bsystem): debug
ty(pe): HL_
sy(nopsis): 
sh(ort description): Delete a name log list
he(ader files): 
lo(ng description): 
pr(erequistes): 
va(lue): 
re(lated to): suppl_log_chg_list_
se(condary subsystems): 
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
#endif
#include <portable.h>

#include "suppldbg.loc"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id: dbgf_dl.c 1210 2006-06-17 03:25:06Z blairdude $";
#endif

void suppl_log_del_list_(suppl_log_list_t *list)
{	suppl_log_list_t *h, *act;

	assert(list);

	chkHeap
	if((act = list->suppl_l_nxt) != 0) do {
		h = act->suppl_l_nxt;
		chkHeap
		free(act);
		chkHeap
	} while((act = h) != 0);

	list->suppl_l_nxt = 0;
	chkHeap
}
