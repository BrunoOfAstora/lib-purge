#include "pg_mem_utils.h"
#include "pg_shred_utils.h"
#include "pg_random.h"
#include "pg_zero.h"

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
	if(pg_wblock_loop(pg->fd, buf, bl, buffsize) != 0)
		goto end;	

	fsync(pg->fd);

	if(pg_wremainder(pg->fd, buf, br) != 0)
		goto end;	

	fsync(pg->fd);

	free(buf);
	return 0;

end:
	free(buf);
	return 3;
}
