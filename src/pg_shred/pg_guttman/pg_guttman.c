#include "pg_guttman.h"
#include "pg_random.h"
#include "pg_shred_utils.h"

int pg_shred_guttman(pgwipe *pg, size_t buffsize)
{
	if(pg == NULL)
		return 1;

	off_t bl = BLOCK_SIZE(pg->fsize, buffsize);
	off_t br = BLOCK_REMA(pg->fsize, buffsize);

	char *buf = malloc(buffsize);
	if(buf == NULL)
		return 2;

	if(pg_random_passes(pg->fd, buf, bl, br, buffsize, 4) != 0)
		goto end;	

	for(size_t i = 0; i < PG_GUTTMAN_PATTERN_COUNT; i++)
	{
		pg_fill_pattern(buf, buffsize, pg_guttman_patterns[i], 3);
		
		if(pg_wblock_loop(pg->fd, buf, bl, buffsize) != 0)
			goto end;

		if(pg_wremainder(pg->fd, buf, br) != 0)
			goto end;
	
		fsync(pg->fd);
	}

	if(pg_random_passes(pg->fd, buf, bl, br, buffsize, 4) != 0)
		goto end;	

	return 0;

end:
	if(buf)
		free(buf);

	return 3;
}
