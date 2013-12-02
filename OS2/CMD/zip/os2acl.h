/*
  Copyright (c) 1990-1999 Info-ZIP.  All rights reserved.

  See the accompanying file LICENSE, version 1999-Oct-05 or later
  (the contents of which are also included in zip.h) for terms of use.
  If, for some reason, both of these files are missing, the Info-ZIP license
  also may be found at:  ftp://ftp.cdrom.com/pub/infozip/license.html
*/
/* os2acl.h
 *
 * Author:  Kai Uwe Rommel <rommel@ars.de>
 * Created: Fri Mar 29 1996
 */

/* $Id: os2acl.h,v 1.1 2004/08/21 14:49:09 prokushev Exp $ */

/*
 * $Log: os2acl.h,v $
 * Revision 1.1  2004/08/21 14:49:09  prokushev
 * * Another set of changes from my local tree
 *
 * Revision 1.1  2004/08/16 06:26:44  prokushev
 * * Another part of files
 *
 * Revision 1.1  1996/03/30 09:35:00  rommel
 * Initial revision
 *
 */

#ifndef _OS2ACL_H
#define _OS2ACL_H

#define ACL_BUFFERSIZE 4096

int acl_get(char *server, const char *resource, char *buffer);
int acl_set(char *server, const char *resource, char *buffer);

#endif /* _OS2ACL_H */

/* end of os2acl.h */
