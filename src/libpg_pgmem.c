#include "libpurge_mem.h"

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


bool pg_strcmp(const char *str1, const char *str2, size_t size)
{

	if(str1 == NULL || str2 == NULL)
		return false;

	
	volatile unsigned char flag = 0;

	size_t i = 0;

	while(i < size)
	{
		flag |= (unsigned char)(str1[i] ^ str2[i]);
		i++;
	}

	return flag == 0;

}


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


