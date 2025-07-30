#include "pg_mem_utils.h"

bool pg_memset(void *blk, unsigned char pattern, size_t size)
{
	if(blk == NULL)
		return false;

	volatile unsigned char *cpy_blk = (unsigned char *)blk;

	size_t i = 0;

	while(i < size)
	{
		cpy_blk[i] = pattern;
		i++;
	}

	return true;
}
