#ifndef LIBPURGE_WP_H
#define LIBPURGE_WP_H

#include <stddef.h>
#include <stdint.h>

#include <sys/types.h>

#define BLOCK_SIZE(fsize, buffsize) ((fsize) / (buffsize))
#define BLOCK_REMA(fsize, buffsize) ((fsize) % (buffsize)) 

typedef enum
{
	ZERO,
	GUTTMAN,
	VSITR,
	NIST,
	HMG,
	DOD

}strategy;

typedef struct pg_modules
{
	const int strat;
	int (*wipe)(struct pgwipe *pg, size_t buffsize);

}pg_modules;

/*
pg_modules pgmod[] = 
{
	{ZERO, pg_shred_zero }
	{}
	{}
}
*/

typedef struct pgwipe
{
	off_t			fsize;
	int 			fd;

	const pg_modules *strat;

}pgwipe;

int pg_fwipe_init( pgwipe **pg_ctx, strategy strat, int fd );

int pg_fwipe_digest( pgwipe *pg, size_t buffsize );

int pg_fwipe_final( pgwipe *pg );



int pg_shred_zero(pgwipe *pg, size_t buffsize);

int pg_shred_guttman(pgwipe *pg, size_t buffsize);

int pg_shred_vsitr(pgwipe *pg, size_t buffsize);

int pg_shred_nist(pgwipe *pg, size_t buffsize);

int pg_shred_hmg(pgwipe *pg, size_t buffsize);

int pg_shred_dod(pgwipe *pg, size_t buffsize);

#endif
