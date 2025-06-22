#define _POSIX_C_SOURCE 200809L

#include <stddef.h>
#ifndef INTERNAL_UTILS_H
#define INTERNAL_UTILS_H

int fill_pattern(const char *file_name, unsigned char pattern, size_t buffer_size);

void purge_secure_zero_memory(void *ptr, size_t size);

#endif
