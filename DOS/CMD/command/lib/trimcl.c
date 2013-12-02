/*	$Id: trimcl.c 771 2004-02-01 13:55:39Z skaus $

 *  Combines ltrimcl() & rtrimcl()

	$Log$
	Revision 1.2  2004/02/01 13:52:17  skaus
	add/upd: CVS $id$ keywords to/of files

	Revision 1.1  2001/04/29 11:33:51  skaus
	chg: default heap size (tools\ptchsize) set to 6KB
	chg: error displaying functions centralized into lib\err_fcts.src
	add: displayError()
	chg: all errors are displayed through functions void error_*()
	bugfix: somtimes error messages are not displayed (see displayError())
	bugfix: docommand(): type:file must pass ":file" to TYPE
	bugfix: error_sfile(): string _SFILE_
	bugfix: error message on empty redirection
	bugfix: comma and semicolon ';' are recognized as argument seperators
		of internal commands
	
	
 */

#include "../config.h"

#include <assert.h>

#include "../include/cmdline.h"

char *trimcl(char *str)
{ assert(str);
  rtrimcl(str);
  return ltrimcl(str);
}
