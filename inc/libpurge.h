#define _POSIX_C_SOURCE 200809L

#ifndef LIBPURGE_H
#define LIBPURGE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FLCN_SUCCESS 0
#define FLCN_MALLOC_ERR -1
#define FLCN_OPEN_FILE_ERR -2
#define FLCN_VERIFY_DIFF_ERR -3

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Handler_Purge_Internal Handler_PG;

typedef struct PurgeOption
{
	const char *filename;
	unsigned char pattern;
	size_t buffer_size;
	int num_passes;
	bool random_pass;

}Purge_opt;

void purge_secure_zero_memory(void *ptr, size_t size);

int file_wipe(Handler_PG *handler, const Purge_opt *pg_opt);

int wipe_free_space();

int purge_scrub_metadata();

int purge_timestomp();

Handler_PG *pg_create(void);

void pg_free(Handler_PG *pg_create);

#ifdef __cplusplus
}
#endif

#endif
