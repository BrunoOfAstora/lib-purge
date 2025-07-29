#include "libpurge_wp.h"
#include "libpurge_mem.h"
#include <stdlib.h>
#include <unistd.h>

int pg_shred_zero(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;

	off_t bl = BLOCK_SIZE(pg->fsize, buffsize);
	off_t br = BLOCK_REMA(pg->fsize, buffsize);

	char *buf = malloc(buffsize);
	if(buf == NULL)
		return 2;
	
	pg_memset(buf, '0', buffsize);

	for(off_t i = 0; i < bl; i++)
	{
		if((write(pg->fd, buf, buffsize)) != (ssize_t)buffsize)
			goto end;
	}

	fsync(pg->fd);

	for(off_t i = 0; i < br; i++)
	{
		if((write(pg->fd, buf, br)) != (ssize_t)buffsize)
			goto end;
	}

	fsync(pg->fd);

	free(buf);

end:
	free(buf);
	return 3;
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

