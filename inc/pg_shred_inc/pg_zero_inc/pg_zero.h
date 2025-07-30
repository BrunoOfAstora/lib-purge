#ifndef PG_ZERO_H
#define PG_ZERO_H

#include "pg_shred.h"

#include <stdlib.h>
#include <unistd.h>

int pg_shred_zero(pgwipe *pg, size_t buffsize);

#endif
