#ifndef PG_SHRED_UTILS_H
#define PG_SHRED_UTILS_H

#include "pg_shred.h"
#include "pg_random.h"

#include <unistd.h>
#include <stdlib.h>

static inline int pg_wblock_loop(int fd, const void *buf, off_t bl, size_t size)
{
	for(off_t i = 0; i < bl; i++)
	{
		if((write(fd, buf, size)) != (ssize_t)size)
			return 1;
	}
	return 0;
}

static inline int pg_wremainder(int fd, const void *buf, off_t br)
{
	if(br > 0)
	{
		if((write(fd, buf, br)) != (ssize_t)br)
			return 1;
	}

	return 0;
}

void pg_fill_pattern(char *buf, size_t buffsize, const unsigned char *pattern, size_t psize);

int pg_random_passes( int fd, void *buf, off_t bl, off_t br, size_t size, unsigned int passes );

#endif
