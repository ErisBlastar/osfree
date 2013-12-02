/* error.h -- Defines used when calling the error handlers. */

/* Copyright (C) 1988,1990,1992 Free Software Foundation, Inc.

   This file is part of GNU Finger.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

#if !defined (_ERROR_H_)
#define _ERROR_H_

/* Classes of error. */
#define FATAL 0			/* Stops the program. */
#define ERROR 1			/* Stops the function.  */
#define WARNING 2		/* Makes noise, and informs. */

void handle_error (), file_error ();

#endif /* _ERROR_H_ */
