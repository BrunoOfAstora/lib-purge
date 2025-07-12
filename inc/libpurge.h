#define _POSIX_C_SOURCE 200809L

#ifndef LIBPURGE_H
#define LIBPURGE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
	FLCN_SUCCESS = 0,
	FLCN_MALLOC_ERR = -1,
	FLCN_OPEN_FILE_ERR = -2,
	FLCN_VERIFY_DIFF_ERR = -3,
 	FLCN_AVAIL_SPACE_ERR = -4,
	FLCN_VERIFY_ERR = -5,
	FLCN_INIT_ERR = -6,
	FLCN_EMPTY_BUFF_ERR = -7,
	FLCN_UNLINK_ERR = -8,
	FLCN_SYNC_ERR = -9,
	FLCN_FLUSH_ERR = -10,
	FLCN_EMPTY_FILE = -11,
	FLCN_FWRITE_ERR = -12,
	FLCN_FREAD_ERR = -13

}flcn_error_t;

#ifdef __cplusplus
extern "C"{
#endif

typedef struct Handler_Purge_Internal Handler_PG;

typedef struct PurgeOption
{
	const char *partpath;
	const char *filename;
	unsigned char pattern;
	size_t buffer_size;
	int num_passes;
	bool random_pass;

}Purge_opt;

void purge_secure_zero_memory(void *ptr, size_t size);

int file_wipe(Handler_PG *handler, const Purge_opt *pg_opt);

int wipe_free_space(const Purge_opt *pg_opt);

int purge_scrub_metadata();

int purge_timestomp();

Handler_PG *pg_create(void);

void pg_free(Handler_PG *pg_create);

#ifdef __cplusplus
}
#endif

#endif
