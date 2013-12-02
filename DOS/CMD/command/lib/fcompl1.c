/*      $Id: fcompl1.c 1287 2006-09-04 21:22:32Z blairdude $

        Complete a given filename

        This file bases on FILECOMP.C of FreeCOM v0.81 beta 1.

        $Log$
        Revision 1.4  2006/09/04 21:22:32  blairdude
        Got rid of all '//' comments

        Revision 1.3  2006/06/26 19:54:12  blairdude
        Long filename filename completion can be enabled with LFNFOR COMPLETE ON

        Revision 1.2  2004/02/01 13:52:17  skaus
        add/upd: CVS $id$ keywords to/of files

        Revision 1.1  2001/04/12 00:33:53  skaus
        chg: new structure
        chg: If DEBUG enabled, no available commands are displayed on startup
        fix: PTCHSIZE also patches min extra size to force to have this amount
           of memory available on start
        bugfix: CALL doesn't reset options
        add: PTCHSIZE to patch heap size
        add: VSPAWN, /SWAP switch, .SWP resource handling
        bugfix: COMMAND.COM A:\
        bugfix: CALL: if swapOnExec == ERROR, no change of swapOnExec allowed
        add: command MEMORY
        bugfix: runExtension(): destroys command[-2]
        add: clean.bat
        add: localized CRITER strings
        chg: use LNG files for hard-coded strings (hangForEver(), init.c)
                via STRINGS.LIB
        add: DEL.C, COPY.C, CBREAK.C: STRINGS-based prompts
        add: fixstrs.c: prompts & symbolic keys
        add: fixstrs.c: backslash escape sequences
        add: version IDs to DEFAULT.LNG and validation to FIXSTRS.C
        chg: splitted code apart into LIB\*.c and CMD\*.c
        bugfix: IF is now using error system & STRINGS to report errors
        add: CALL: /N

 */

#include "../config.h"

#include <assert.h>
#include <ctype.h>
//#include <dir.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

#include "../include/lfnfuncs.h"
#undef findfirst
#undef findnext
#include "../include/command.h"
#include "../strings.h"

#include "tcc2wat.h"

void complete_filename(char *str, unsigned charcount)
{
  /* variables found within code */
  struct ffblk file;
  #undef ffblk

  int found_dot = 0;
  int curplace = 0;
  int start;
  int count;
  int perfectmatch = 1;
  int makelower;
  char path[128];
  char fname[14];
  char maxmatch[13] = "";
  char directory[128];

  assert(str);

  /* expand current file name */
  count = charcount - 1;
  if (count < 0)
    makelower = count = 0;
  else
  {
    /* if last character is lower case, then make lookup lower case. */
    makelower = islower(str[count]);
  }

  while (count > 0 && str[count] != ' ')  /* find front of word */

    count--;

  if (str[count] == ' ')        /* if not at beginning, go forward 1 */

    count++;

  start = count;

  /* extract directory from word */
  strcpy(directory, &str[start]);
  curplace = strlen(directory) - 1;
  while (curplace >= 0 && directory[curplace] != '\\' &&
         directory[curplace] != ':')
  {
    directory[curplace] = 0;
    curplace--;
  }

  strcpy(path, &str[start]);

  /* look for a . in the filename */
  for (count = strlen(directory); path[count] != 0; count++)
    if (path[count] == '.')
    {
      found_dot = 1;
      break;
    }
  if (found_dot)
    strcat(path, "*");
  else
    strcat(path, "*.*");

  curplace = 0;                 /* current fname */

#ifdef FEATURE_LONG_FILENAMES
  if( lfncomplete ? lfnfindfirst( path, &file, FILE_SEARCH_MODE ) == 0 :
                    findfirst( path, ( struct ffblk * )&file, FILE_SEARCH_MODE )
                    == 0 )
#else
  if (FINDFIRST(path, &file, FILE_SEARCH_MODE) == 0)
#endif
  {                             /* find anything */

    do
    {
      if (file.ff_name[0] == '.' &&
          (!file.ff_name[1] ||
          (file.ff_name[1] == '.' && !file.ff_name[2]))) /* ignore . and .. */

        continue;

      strcpy(fname, file.ff_name);
      if (makelower)
        strlwr(fname);

      if (file.ff_attrib == FA_DIREC)
        strcat(fname, "\\");
      else
        strcat(fname, " ");

      if (!maxmatch[0] && perfectmatch)
        strcpy(maxmatch, fname);

      else
      {
        for (count = 0; maxmatch[count] && fname[count]; count++)
          if (maxmatch[count] != fname[count])
          {
            perfectmatch = 0;
            maxmatch[count] = 0;
            break;
          }
      }
    }
#ifdef FEATURE_LONG_FILENAMES
    while( lfncomplete ? lfnfindnext( &file ) == 0 :
                         findnext( ( struct ffblk * )&file ) == 0 );
#else
    while (FINDNEXT(&file) == 0);
#endif
    FINDSTOP(&file);

    strcpy(&str[start], directory);
    strcat(&str[start], maxmatch);

    if (!perfectmatch)
      beep();
  }
  else                          /* no match found */
    beep();
}
