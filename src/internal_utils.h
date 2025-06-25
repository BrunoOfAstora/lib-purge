#define _POSIX_C_SOURCE 200809L

#ifndef INTERNAL_UTILS_H
#define INTERNAL_UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Handler_Purge_Internal Handler_PG;

typedef struct PurgeOption
{
	const char *filename;
	unsigned char pattern;
	size_t buffer_size;

}Purge_opt;


int file_wipe( Handler_PG *handler, const Purge_opt *pg_opt );

int wipe_free_space();

int purge_scrub_metadata();

int purge_timestomp();

void purge_secure_zero_memory(void *ptr, size_t size);

#endif
