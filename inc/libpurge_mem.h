#ifndef LIBPURGE_MEM_H
#define LIBPURGE_MEM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/types.h>

bool pg_memcmp(void *blk1, void *blk2, off_t size);

bool pg_strcmp(const char *str1, const char *str2, size_t size);

bool pg_memset( void *blk, unsigned char pattern, size_t size );

#endif
