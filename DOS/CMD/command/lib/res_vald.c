/* $Id: res_vald.c 1301 2006-09-11 00:07:22Z blairdude $

	Returns whether the passed-in file is a probably
	valid resource file.

	0 -> is valid
	1 -> no such file
	2 -> is a device
	3 -> not valid

*/

#include "../config.h"

#include <assert.h>
#include <fcntl.h>
#include <io.h>

#include "../include/command.h"
#include "../include/misc.h"
#include "../include/openf.h"
#include "../include/resource.h"
#include "../include/strings.typ"

/* Is called only, if the Strings resource had been found,
	hence, returning TRUE alsways. */

static int test_fct (res_majorid_t major,
		     res_minorid_t minor,
		     long length,
		     FILE* f,
		     void *const arg) {
	(void)major, (void)length, (void)f, (void)arg;
	assert(major == RES_ID_STRINGS);
	return minor == STRING_RESOURCE_MINOR_ID? 1 : 2;
}

int validResFile(const char * const fnam)
{	int fd;

	assert(fnam);

	if((fd = open(fnam, O_RDONLY | O_BINARY)) < 0)
		return 1;

	if(isadev(fd)) {
		close(fd);
		return 2;
	}

	close(fd);
	if(enumFileResources(fnam
	 , RES_ID_STRINGS, test_fct, (void*)0) != 1)
		return 3;

	return 0;
}
