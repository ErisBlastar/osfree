/*
 *
 */

#undef EOPNOTSUPP
#define	EOPNOTSUPP	45		/* Operation not supported */
#define	ENOTSUP		EOPNOTSUPP	/* Operation not supported */


int  ftruncate( int handle, long size );
char *realpath(const char *path, char *resolved_path);
