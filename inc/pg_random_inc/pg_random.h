#ifndef PG_RANDOM_H
#define PG_RANDOM_H

#include <sys/random.h>

int pg_rand(const unsigned int limit);

#endif
