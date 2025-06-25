#include <string.h>
#include <stddef.h>

#include "internal_utils.h"

#if defined(__STDC_LIB_EXT1)

	void purge_secure_zero_memory(void *ptr, size_t size)
	{
		memset_s(ptr, size, 0, size);
	}
		

/* #elif defined(_WIN32) //For future Windows compatbility

	void purge_secure_zero_memory(void *ptr, size_t size)
	{
		SecureZeroMemory(ptr, size);
	}
*/

#else

	void purge_secure_zero_memory(void *ptr, size_t size)
	{
		volatile unsigned char *p = ptr;
		while(size--)
		{
		*p++ = 0;
		}
	}

#endif
