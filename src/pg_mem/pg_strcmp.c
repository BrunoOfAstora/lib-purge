#include "pg_mem_utils.h"

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
