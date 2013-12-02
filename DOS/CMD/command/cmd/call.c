/* $Id: call.c 1291 2006-09-05 01:44:33Z blairdude $
 *  CALL.C - batch file call command.
 *
 *  Comments:
 *
 * 16 Jul 1998 (Hans B Pufal)
 *   started.
 *
 * 16 Jul 1998 (John P Price)
 *   Seperated commands into individual files.
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 * 04-Aug-1998 (Hans B Pufal)
 * - added lines to initialize for pointers (HBP004)  This fixed the
 *   lock-up that happened sometimes when calling a batch file from
 *   another batch file.
 *
 * 10-Aug-1998 ska
 * - changed: initialize bcontext with function
 *
 */

#include "../config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/batch.h"
#include "../include/cmdline.h"
#include "../include/command.h"
#include "../err_fcts.h"
#include "../include/kswap.h"

static int optS = 0;		/* force to swap out FreeCOM during call */
static int optN = 0;		/* force to NOT swap (superceeds optS) */

optScanFct(opt_call)
{
  (void)arg;
  switch(ch) {
  case 'S': return optScanBool(optS);
  case 'N': return optScanBool(optN);
  case 'Y': return optScanBool(tracemode);
  }
  optErr();
  return E_Useage;
}

/*
 * Perform CALL command.
 *
 * Allocate a new batch context and add it to the current chain.
 * Call parsecommandline passing in our param string
 * If No batch file was opened then remove our newly allocted
 * context block.
 */
int cmd_call (char * param) {
	struct bcontext *n = newBatchContext();
	int ec;

        (void)param;
	if (n == 0) {
		/* Not in a batch file */
		return 1;
	}

	optS = optN = 0;

	if((ec = leadOptions(&param, opt_call, 0)) != E_None)
		return ec;

	if(swapOnExec != ERROR) {
		if(optS)
			swapOnExec = TRUE;
		if(optN)
			swapOnExec = FALSE;
	}

	parsecommandline(param, FALSE);
	if(swapOnExec != ERROR)
		swapOnExec = FALSE;

	if (bc->bfile == 0
	 && bc->bfnam == 0) {    /* Wasn't a batch file so remove context */
		bc = bc->prev;
		free(n);
	}

	return 0;
}
