/*  $Revision: 1.3 $
**  Modified by Rich $alz <rsalz@osf.org> to be more portable to older
**  systems.
*/
/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the University of California, Berkeley and its contributors''
 * in the documentation or other materials provided with the distribution
 * and in all advertising materials mentioning features or use of this
 * software. Neither the name of the University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <getopt.h>

#ifdef __OS2__
#include <utils.h>
#endif

#define SYSLOG_NAMES

#include "syslog.h"

int pencode(char *);
char *getlogin(void);
int decode(char *, CODE *);
void bailout(char *, char *);
void usage(void);

/*
**  LOGGER -- read and log utility
**
**	This routine reads from an input and arranges to write the
**	result on the system log, along with a useful tag.
*/

char *getlogin(void)
{
	char *log;
#if __EMX__ || __OS2__
    log=getenv("USER");
#else
    log=getenv("LOGNAME");
#endif
    if (log==NULL) log="root";
    return log;
}

int main(int argc, char **argv)
{
	int pri = LOG_NOTICE;
	int ch, logflags = 0;
	char *tag, buf[1024];

	tag = NULL;
	while ((ch = getopt(argc, argv, "f:ip:st:")) != EOF)
		switch((char)ch) {
		case 'f':		/* file to log */
			if (freopen(optarg, "r", stdin) == NULL) {
				(void)fprintf(stderr, "logger: %s: %s.\n",
				    optarg, strerror(errno));
				exit(1);
			}
			break;
		case 'i':		/* log process id also */
			logflags |= LOG_PID;
			break;
		case 'p':		/* priority */
			pri = pencode(optarg);
			break;
		case 's':		/* log to standard error */
			logflags |= LOG_PERROR;
			break;
		case 't':		/* tag */
			tag = optarg;
			break;
		case '?':
		default:
			usage();
		}
	argc -= optind;
	argv += optind;

	/* setup for logging */
	openlog(tag ? tag : getlogin(), logflags, 0);
	(void) fclose(stdout);

	/* log input line if appropriate */
	if (argc > 0) {
		register char *p, *endp;
		int len;

		for (p = buf, endp = buf + sizeof(buf) - 2; *argv;) {
			len = strlen(*argv);
			if (p + len > endp && p > buf) {
				syslog(pri, "%s", buf);
				p = buf;
			}
			if (len > sizeof(buf) - 1)
				syslog(pri, "%s", *argv++);
			else {
				if (p != buf)
					*p++ = ' ';
				memmove(p, *argv++, len);
				*(p += len) = '\0';
			}
		}
		if (p != buf)
			syslog(pri, "%s", buf);
		exit(0);
	}

	/* main loop */
	while (fgets(buf, sizeof(buf), stdin) != NULL)
		syslog(pri, "%s", buf);

	exit(0);
}


/*
 *  Decode a symbolic name to a numeric value
 */
int pencode(char *s)
{
	char *save;
	int fac, lev;

	for (save = s; *s && *s != '.'; ++s);
	if (*s) {
		*s = '\0';
		fac = decode(save, facilitynames);
		if (fac < 0)
			bailout("unknown facility name: ", save);
		*s++ = '.';
	}
	else {
		fac = 0;
		s = save;
	}
	lev = decode(s, prioritynames);
	if (lev < 0)
		bailout("unknown priority name: ", save);
	return ((lev & LOG_PRIMASK) | (fac & LOG_FACMASK));
}

int decode(char *name, CODE *codetab)
{
	register CODE *c;

	if (isdigit(*name))
		return (atoi(name));

	for (c = codetab; c->c_name; c++)
		if (!stricmp(name, c->c_name))
			return (c->c_val);

	return (-1);
}

void bailout(char *msg, char *arg)
{
	fprintf(stderr, "logger: %s%s\n", msg, arg);
	exit(1);
}

void usage(void)
{
	fputs("logger: [-is] [-f file] [-p pri] [-t tag] [ message ... ]\n",
	    stderr);
	exit(1);
}
