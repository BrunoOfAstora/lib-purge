#include "pg_mem_utils.h"

bool pg_memcmp(void *blk1, void *blk2, off_t size)
{
	const volatile unsigned char *cpy_blk1 = ( const volatile unsigned char * ) blk1;
	const volatile unsigned char *cpy_blk2 = ( const volatile unsigned char * ) blk2;

	off_t i = 0;

	while ( i < size)
	{
		flag |= cpy_blk1[i] ^ cpy_blk2[i];
		i++;
	}

	return flag == 0;
}


