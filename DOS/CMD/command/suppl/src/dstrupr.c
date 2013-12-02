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

	char *StrUpr(char s[])

	Upcase the string s[] using the toUpper() function.
	s may be NULL.

	Return:
		s[]

ob(ject): StrUpr
su(bsystem): nls
ty(pe): 
sh(ort description): Upcase the string using the DOS NLS
lo(ng description): Upcases the string using the DOS NLS information.
	The string is overwritten,
pr(erequistes): 
re(lated to): 
se(condary subsystems): dynstr
in(itialized by): 
wa(rning): 
bu(gs): 
va: \para{s}
fi(le): dstrupr.c

*/

#include "initsupl.loc"

#include <portable.h>
#include "dynstr.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id: dstrupr.c 1289 2006-09-04 22:12:50Z blairdude $";
#endif

char *StrUpr(char *s)
{	char *p;

	DBG_ENTER("StrUpr", Suppl_nls)

	if((p = s) != 0)
		while((*p = toUpper(*p)) != NUL)
			++p;
	DBG_RETURN_S( s)
}
