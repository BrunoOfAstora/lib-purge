#define _POSIX_C_SOURCE_200809L

#ifndef LIBPURGE_H
#define LIBPURGE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>


//Error Codes:
typedef enum
{
	PG_SUCCESS 			=	 0,
	PG_MALLOC_ERR 		= 	-1,
	PG_OPEN_FILE_ERR 	= 	-2,
	PG_VERIFY_DIFF_ERR 	= 	-3,
 	PG_AVAIL_SPACE_ERR 	= 	-4,
	PG_VERIFY_ERR 		= 	-5,
	PG_INIT_ERR 		= 	-6,
	PG_EMPTY_BUFF_ERR 	= 	-7,
	PG_UNLINK_ERR 		= 	-8,
	PG_SYNC_ERR 		= 	-9,
	PG_FLUSH_ERR 		= 	-10,
	PG_EMPTY_FILE 		= 	-11,
	PG_FWRITE_ERR 		= 	-12,
	PG_FREAD_ERR 		= 	-13,
	PG_FSET_ERR			= 	-14,
	PG_FEND_ERR			=	-15,
	PG_MEMSET_ERR		=	-16,
	PG_INVALID_FSIZE	=	-17,
	PG_READ_ERR			=	-18,
	PG_WRITE_ERR		=	-19,
	PG_SEEK_ERR			=	-20

}flcn_error_t;


//Flush modes
#define PG_FLUSH_FILE 	0x01
#define PG_FLUSH_FS   	0x02
#define PG_FLUSH_DATA	0x03
#define PG_FLUSH_ALL 	0x04

typedef struct Pg_handler pg_handler;

typedef struct Pg_options
{

	int 			file_pg;
	unsigned int 	passes;	
	unsigned char 	pattern;
	off_t			buffer_s;
	bool			rand_pass;

}pg_opt;

int _verifyws(off_t buffer_s, const pg_opt *pg_opt, pg_handler *handler);

struct Pg_handler *pg_init(void);
void pg_free(struct Pg_handler *pg_init);

int pg_securezeromemory(void *handler, size_t size);

int fwipe(pg_handler *handler, const pg_opt *pg_opt, const char *fname, bool verify, bool flush, unsigned int flush_opt);

#endif
