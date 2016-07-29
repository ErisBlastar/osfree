/**************************************************************************
 *
 *  Copyright 2008, Roger Brown
 *
 *  This file is part of Roger Brown's Toolkit.
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*
 * $Id: emtypes.h 1938 2012-01-04 18:12:44Z rogerb $
 */


#ifndef EMTYPES_H
#define EMTYPES_H

#ifndef SOMEEMRegisterData_VA_STUBS
    #define SOMEEMRegisterData_VA_EXTERN
#endif /* SOMEEMRegisterData_VA_STUBS */

#ifndef __cplusplus
    #include <emregdat.h>
    #include <event.h>
    #include <clientev.h>
    #include <eventmsk.h>
#else
    #include <emregdat.xh>
    #include <event.xh>
    #include <clientev.xh>
    #include <eventmsk.h>
#endif

typedef void SOMLINK EMRegProc(SOMEEvent SOMSTAR,void *);
typedef void SOMLINK EMMethodProc(SOMObject SOMSTAR,SOMEEvent SOMSTAR, void *);
typedef void SOMLINK EMMethodProcEv(SOMObject SOMSTAR,Environment *,SOMEEvent SOMSTAR,void *);

#endif /* EMTYPES_H */
