#ifndef LIBPURGE_H
#define LIBPURGE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Handler_Purge_Internal Handler_PG;

typedef struct PurgeOption
{
	const char *filename;
	unsigned char pattern;
	size_t buffer_size;

}Purge_opt;

int file_wipe(Handler_PG *handler, const Purge_opt *pg_opt);

void purge_secure_zero_memory(void *ptr, size_t size);

Handler_PG *pg_create(void);

void pg_free(Handler_PG *pg_create);


#ifdef __cplusplus
}
#endif

#endif
