#include "pg_mem_utils.h"

bool pg_memcmp(void *blk1, void *blk2, off_t size)
{
	if(blk1 == NULL || blk2 == NULL)
		return false;

	const unsigned char *cpy_blk1 = ( const unsigned char * ) blk1;
	const unsigned char *cpy_blk2 = ( const unsigned char * ) blk2;

	volatile unsigned char flag = 0;

	off_t i = 0;

	while ( i < size)
	{
		flag |= cpy_blk1[i] ^ cpy_blk2[i];
		i++;
	}

	return flag == 0;
}


