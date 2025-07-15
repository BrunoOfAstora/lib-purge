#include <stddef.h>
#include <string.h>
#include "libpurge.h"

#if defined(__STDC_LIB_EXT1)

int pg_securezeromemory(void *ptr, size_t size)
{
	if((memset_s(ptr, size, 0, size)) != 0)
		return PG_MEMSET_ERR;

	return PG_SUCCESS;
}

#else

int pg_securezeromemory(void *ptr, size_t size)
{
	volatile unsigned char *p = ptr;
	if(p == NULL)
		return PG_INIT_ERR;

	while(size--)
		*p++ = 0;

	return PG_SUCCESS;
}

#endif
