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

	void Eseek(FILE * const fp, dword [*] pos, int whence)

	As Fseek(), but terminate program on failure.

ob(ject): Eseek
su(bsystem): error
ty(pe): S
sh(ort description): Seek a FILE stream with an (dword) offset
lo(ng description): Seeks a FILE streams with an (dword) offset. On failure
	the program is terminated with the error message: "Cannot access
	file \"%s\""
pr(erequistes): 
re(lated to): Eopen Ecreate Fseek
se(condary subsystems): supplio
in(itialized by): 
wa(rning): 
bu(gs): 
va(lue): none
fi(le): eeseek.c

*/

#include "initsupl.loc"

#ifndef _MICROC_
#endif
#include <portable.h>
#include "msgs.h"
#include "supplio.h"

#include "suppldbg.h"

#ifdef RCS_Version
static char const rcsid[] = 
	"$Id: eeseek.c 1210 2006-06-17 03:25:06Z blairdude $";
#endif

void Eseek(FILE * const fp, dword iM(*) pos, int whence)
{	DBG_ENTER("Eseek", Suppl_error)

	if(Fseek(fp, pos, whence))
		Esuppl_accessFile(Fxinfo(fp)->sea_fnam);

	DBG_EXIT
}
