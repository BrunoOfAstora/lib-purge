#include "libpurge_wp.h"
#include <unistd.h>
#include <stdlib.h>
//#include <fnctl.h>

int pg_fwipe_init( pgwipe **pg_ctx, strategy strat, int fd )
{
	*pg_ctx = malloc( sizeof(pgwipe) );
	if( *pg_ctx == NULL )
		return 1;

	off_t size = lseek(fd, 0, SEEK_END);

	if(size == (off_t) -1)
		return 2;

	(*pg_ctx)->fsize = size;
	(*pg_ctx)->fd 	 = fd;

	lseek(fd, 0, SEEK_SET);

	return 0;
}

int pg_fwipe_digest( pgwipe *pg_ctx, size_t buffsize )
{
	if(pg_ctx == NULL)
		return 1;

	if(buffsize == 0)
		return 2;
	
	return 0;
}

int pg_fwipe_final( pgwipe *pg_ctx )
{

	return 0;	

}


