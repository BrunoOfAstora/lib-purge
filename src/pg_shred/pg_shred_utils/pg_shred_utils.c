#include "pg_shred_utils.h"
#include "pg_mem_utils.h"

#include "pg_zero.h"
#include "pg_guttman.h"
#include "pg_vsitr.h"
#include "pg_nist.h"
#include "pg_hmg.h"
#include "pg_dod.h"

pg_modules pgmod[] = 
{
	{ ZERO,    pg_shred_zero },
	{ GUTTMAN, pg_shred_guttman },
	{ VSITR,   pg_shred_vsitr },
	{ NIST,    pg_shred_nist },
	{ HMG,     pg_shred_hmg },
	{ DOD,     pg_shred_dod },
};

int pg_fwipe_init( pgwipe **pg_ctx, strategy strat, int fd )
{
	*pg_ctx = malloc( sizeof(pgwipe) );
	if( *pg_ctx == NULL )
		return 1;

	off_t size = lseek(fd, 0, SEEK_END);	

	if(size == (off_t) -1)
		return 2;

	if (strat < 0 || strat >= (int)(sizeof(pgmod) / sizeof(pgmod[0])))
		(*pg_ctx)->strat = &pgmod[NIST];

	(*pg_ctx)->strat = &pgmod[strat];
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

	if(pg_ctx->strat == NULL || pg_ctx->strat->wipe == NULL)
		return 3;

	return pg_ctx->strat->wipe(pg_ctx, buffsize);
}


int pg_fwipe_final( pgwipe *pg_ctx )
{
	fsync(pg_ctx->fd);

	if(pg_ctx)
		free(pg_ctx);

	return 0;	
}
//
//
//
//
//
//
void pg_fill_pattern(char *buf, size_t buffsize, const unsigned char *pattern, size_t psize)
{
	for(size_t i = 0; i < buffsize; i ++)
		buf[i] = pattern[i % psize];
}

int pg_random_passes( int fd, void *buf, off_t bl, off_t br, size_t size, unsigned int passes )
{
	for(unsigned int i = 0; i < passes; i++)
	{
		pg_memset(buf, pg_rand(256), size);
	
		if(pg_wblock_loop(fd, buf, bl, size) != 0)
			return 1;
	
		if(pg_wremainder(fd, buf, br) != 0)
			return 2;

		fsync(fd);
	}
	return 0;
}
