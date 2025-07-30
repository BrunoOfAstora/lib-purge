#include "pg_random.h"

int pg_rand(const unsigned int limit)
{
	unsigned int pg_rand_val = 0;

	if(getrandom(&pg_rand_val, sizeof(pg_rand_val), 0) != sizeof(pg_rand_val))
	return -1;

	unsigned int pg_rand_final = pg_rand_val % limit;

	return pg_rand_final;
}


