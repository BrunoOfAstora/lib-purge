#include "libpurge_wp.h"
#include <unistd.h>




int pg_shred_zero(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;

	off_t bl = BLOCK_SIZE(pg->fsize, buffsize);
	off_t br = BLOCK_REMA(pg->fsize, buffsize);

	for(off_t i = 0; i < bl; i++)
		if((write(pg->fd, 0, 1)) == -1)
			return 2;

	for(off_t i = 0; i < br; i++);
		if((write(pg->fd, 0, 1)) == -1)
			return 3;

	return 0;
}

int pg_shred_guttman(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;

	return 0;

}

int pg_shred_vsitr(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;

	return 0;
}

int pg_shred_nist(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;
	
	return 0;
}

int pg_shred_hmg(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;
	
	return 0;
}

int pg_shred_dod(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;

	return 0;
}

