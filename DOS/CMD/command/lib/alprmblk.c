/*	$Id: alprmblk.c 986 2004-06-29 21:57:21Z skaus $

	Allocate a permanent block of memory.
	These blocks are system memory blocks, if swapping is enabled;
	otherwise "normal" ones.

	$Log$
	Revision 1.3  2004/06/29 21:57:20  skaus
	fix: /LOW option

	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files
	
	Revision 1.1  2001/04/12 00:33:52  skaus
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

#include <suppl.h>

#include "../include/command.h"
#include "../include/kswap.h"
#include "../include/misc.h"

unsigned allocPermBlk(const unsigned size, const unsigned mode)
{	assert(size);
	if(swapOnExec == ERROR)
		return allocMemBlk(size, mode);
	return allocSysBlk(size, mode);
}
