//libpg_file_handling.c//
#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE

#include "libpurge.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct Pg_handler
{
	off_t			filesize;

	unsigned char 	*buffer;

	int 			filedesc;

}pg_handler;

//"Constructor"
struct Pg_handler *pg_init(void)
{
	struct Pg_handler *pg_init = malloc(sizeof(struct Pg_handler));

	if(!pg_init)
		return NULL;
	
	pg_init->filesize = 0;
	pg_init->buffer   = NULL;
	pg_init->filedesc = 0;

	return pg_init;
}

//"Destructor"
void pg_free(pg_handler *pg_init)
{
	if(pg_init)
	{
		if(pg_init->buffer)
		{
			free(pg_init->buffer);
		}

	free(pg_init);
	}

	return;
}



/*
 *	#######################
 *	#Verify Wipe Integrity#
 *	#######################
 */

int _verifyws(off_t buffer_s, const pg_opt *pg_opt, pg_handler *handler )
{
	int ret_code = PG_INIT_ERR;

	if((lseek(pg_opt->file_pg, 0, SEEK_SET)) == -1)
	{
		ret_code = PG_FSET_ERR;
		goto ret;
	}

	char *cmp_buffer = malloc(buffer_s);
	if(!cmp_buffer)
	{
		ret_code = PG_MALLOC_ERR;
		goto ret;
	}
	
	if((memset(cmp_buffer, pg_opt->pattern, buffer_s)) == NULL)
	{
		ret_code = PG_MEMSET_ERR;
		goto ret;
	}

	off_t bf_size = handler->filesize / buffer_s;
	off_t rf_size = handler->filesize % buffer_s;

	if(bf_size < 0)
	{
		ret_code = PG_INVALID_FSIZE;
		goto ret;
	}

	unsigned char *new_buffer = malloc(buffer_s);
	if(!new_buffer)
	{
		ret_code = PG_MALLOC_ERR;
		goto ret;
	}

	memset(new_buffer, pg_opt->pattern, buffer_s);

	for(off_t i = 0; i < bf_size; i++)
	{
		if((read(pg_opt->file_pg, cmp_buffer, buffer_s)) == -1)
		{
			ret_code = PG_READ_ERR;
			goto ret;
		}

		if((memcmp(cmp_buffer, new_buffer, buffer_s)) != 0)
		{
			ret_code = PG_VERIFY_DIFF_ERR;
			goto ret;
		}	
	}
	
	if(rf_size != 0 && rf_size > 0)
	{
		if((read(pg_opt->file_pg, cmp_buffer, rf_size)) == -1)
		{
			ret_code = PG_READ_ERR;
			goto ret;
		}

		if((memcmp(cmp_buffer, new_buffer, rf_size)) != 0)
		{
			ret_code = PG_VERIFY_DIFF_ERR;
			goto ret;
		}
	}

	ret_code = PG_SUCCESS;

ret:
//	free(new_buffer);
	free(cmp_buffer);

	return ret_code;	
}

/*
 *	################
 *	#Flush Options##
 *	################
 */
int pg_flush(int fd, unsigned int flush_opt)
{
	if(flush_opt & PG_FLUSH_FILE)
		if((fsync(fd)) != 0)	
			return PG_FLUSH_ERR;

	if(flush_opt & PG_FLUSH_FS)
		if((syncfs(fd)) != 0)
			return PG_FLUSH_ERR;

	if(flush_opt & PG_FLUSH_DATA)
		if((fdatasync(fd)) != 0)
			return PG_FLUSH_ERR;

	if(flush_opt & PG_FLUSH_ALL)
		sync(); 

return PG_SUCCESS;
}


/*
 *	####################
 *	#File wipe Function#
 * 	####################
 */

int fwipe(pg_handler *handler, const pg_opt *pg_opt, const char * fname, bool verify, bool flush, unsigned int flush_opt)
{
	int ret_code = PG_INIT_ERR;
	
	if(handler == NULL || pg_opt == NULL)
		goto ret;


	if((handler->filesize = lseek(pg_opt->file_pg, 0, SEEK_END)) == -1)
	{
		ret_code = PG_INVALID_FSIZE;
		goto ret;
	}

	off_t ebuffer_s = 0;

		off_t block_s = handler->filesize / pg_opt->buffer_s;	
	off_t remainder_s = handler->filesize % pg_opt->buffer_s;

	if(pg_opt->buffer_s > handler->filesize)
		ebuffer_s = handler->filesize;

	else
		ebuffer_s = pg_opt->buffer_s;


	handler->buffer = malloc(ebuffer_s);
	if(handler->buffer == NULL)
	{
		ret_code = PG_MALLOC_ERR;
		goto ret;
	}


	if((memset(handler->buffer, pg_opt->pattern, ebuffer_s)) == NULL)
	{
		ret_code = PG_MEMSET_ERR;
		goto ret;
	}
	

	if(flush == true)
		if((pg_flush(pg_opt->file_pg, flush_opt)) != PG_SUCCESS)
		{
			ret_code = PG_FLUSH_ERR;
			goto ret;
		}

//
//
//
//
//
//
	
	if((lseek(pg_opt->file_pg, 0, SEEK_SET)) == -1)
	{
		ret_code = PG_INVALID_FSIZE;
		goto ret;
	}

	unsigned int passes = pg_opt->passes;
	if(passes <= 0)
	{
		ret_code = PG_INVALID_FSIZE;
		goto ret;
	}

	while(passes > 0)
	{
		if((lseek(pg_opt->file_pg, 0, SEEK_SET)) == -1)
		{
			ret_code = PG_INVALID_FSIZE;
			goto ret;
		}
		
		for(off_t i = 0; i < block_s; i++)
		{
			if((write(pg_opt->file_pg, handler->buffer, ebuffer_s)) == -1)
			{
				ret_code = PG_WRITE_ERR;
				goto ret;
			}
		}

		if(remainder_s > 0)
		{
			if((write(pg_opt->file_pg, handler->buffer, remainder_s)) == -1)
			{
				ret_code = PG_WRITE_ERR;
				goto ret;
			}
		}	
	
		passes--;
	}

	if(flush)
		if((pg_flush(pg_opt->file_pg, flush_opt)) != PG_SUCCESS)
		{
			ret_code = PG_FLUSH_ERR;
			goto ret;
		}

	if(verify == true)
		if((_verifyws(ebuffer_s, pg_opt, handler)) != PG_SUCCESS)
		{
			ret_code = PG_VERIFY_DIFF_ERR;
			goto ret;
		}

	ret_code = PG_SUCCESS;

ret:
	
	pg_securezeromemory(handler->buffer, ebuffer_s);

//	lock(handler->filedesc, LOCK_UN);

	if(pg_opt->file_pg)
		close(pg_opt->file_pg);


	if((unlink(fname)) != 0)
	{
		ret_code = PG_UNLINK_ERR;
		goto ret;
	}
	
	return ret_code;
}
