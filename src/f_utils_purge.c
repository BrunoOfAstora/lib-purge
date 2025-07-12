			//f_utils_purge.c//

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>   
#include <sys/statvfs.h>   
#include <unistd.h>
#include <sys/file.h>
#include <fcntl.h>
#include "libpurge.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct Handler_Purge_Internal
{
	
	off_t filesize;
	unsigned char *buffer;
	FILE *file_to_encrypt;
	
}Handler_PG;


//Handler "Constructor"
Handler_PG *pg_create(void)
{
	Handler_PG *pg_init = malloc(sizeof(struct Handler_Purge_Internal));
	if(!pg_init)
	{
		return NULL;
	}

	pg_init->filesize = 0;
	pg_init->buffer = NULL;
	pg_init->file_to_encrypt = NULL;
	
	return pg_init;
}

//Handler free
void pg_free(Handler_PG *pg_init)
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

int wipe_free_space(const Purge_opt *pg_opt)
{
	struct statvfs st;

	unsigned long f_space_avail = 0;

	unsigned long stat = statvfs(pg_opt->partpath, &st); //The partpath (partition path) is the path of partition where you'll wipe
		
	if((f_space_avail = st.f_bavail * st.f_frsize) == 0)
	{
<<<<<<< HEAD
		printf("error: the free space available is 0\n");
=======
>>>>>>> origin/main
		return FLCN_AVAIL_SPACE_ERR;
	}	

	unsigned char *buffer = malloc(pg_opt->buffer_size);
	if(!buffer)
	{
<<<<<<< HEAD
		perror("error: error while alocating memory in fs_wipe function\n");
=======
>>>>>>> origin/main
		return FLCN_MALLOC_ERR;
	}

	memset(buffer, pg_opt->pattern, pg_opt->buffer_size);

	int fd;
	
	fd = open("_flcn-purge-freespace_", O_CREAT | O_APPEND | O_RDWR, 0777);
	unsigned long i = 0;

	//calculate free space and remaider space
	while(i < f_space_avail)
	{
		write(fd, buffer, pg_opt->buffer_size);
		i += pg_opt->buffer_size;
	}
<<<<<<< HEAD
	
=======
//add tempfile	
>>>>>>> origin/main
	remove("_flcn-purge-freespace_");

	return FLCN_SUCCESS;
}

int _verify_shred(FILE *cmp_file, unsigned char cmp_pattern, int buffsize, off_t filesize)
{
	int return_code = -6;

	rewind(cmp_file);	

	char *cmp_file_buffer = malloc(buffsize);
	if(!cmp_file_buffer)
		return FLCN_MALLOC_ERR;

	char *cmp_pattern_buffer = malloc(buffsize);
	if(!cmp_pattern_buffer)
		return FLCN_MALLOC_ERR;

	memset(cmp_pattern_buffer, cmp_pattern, buffsize);

	off_t block_file_size = filesize / buffsize;

	for(off_t i = 0; i < block_file_size; i++)
	{
		if((fread(cmp_file_buffer, buffsize, 1, cmp_file)) != 1)
		{
			return_code = FLCN_FREAD_ERR;
			break;
		}
		
		if((memcmp(cmp_file_buffer, cmp_pattern_buffer, buffsize)) != 0)
		{
			free(cmp_pattern_buffer);
			free(cmp_file_buffer);

			return FLCN_VERIFY_DIFF_ERR;
		}
	}
	
	free(cmp_pattern_buffer);
	free(cmp_file_buffer);
	
	rewind(cmp_file);
	
	return_code = FLCN_SUCCESS;
	return return_code;
} 


//Funtion to wipe entire files with a pattern, consequently, wiping the memory space where this file was originally.
int file_wipe( Handler_PG *handler, const Purge_opt *pg_opt )
{
	int return_code = -1;

//	struct stat buf;
//	int f_status = 0;
		
	if(pg_opt == NULL || pg_opt->filename == NULL || handler == NULL)
		return FLCN_INIT_ERR;
	

	handler->buffer = malloc(pg_opt->buffer_size);

	if(handler->buffer == NULL)
	{	
		return_code = FLCN_EMPTY_BUFF_ERR;
		goto cleanup;
	}
	
	memset( handler->buffer, pg_opt->pattern, pg_opt->buffer_size );
	
	handler->file_to_encrypt = fopen(pg_opt->filename, "r+b");
	if(!handler->file_to_encrypt)
	{
		return_code = FLCN_OPEN_FILE_ERR;
		goto cleanup;
	}

	if((unlink(pg_opt->filename)) != 0)
	{
		return_code = FLCN_UNLINK_ERR;
		goto cleanup;
	}

	if(fsync(fileno(handler->file_to_encrypt)) != 0)
	{
		return_code = FLCN_SYNC_ERR;
		goto cleanup;
	}

	if((unlink(pg_opt->filename)) != 0)
	{
		perror("failed to unlink the file from filesystem");
		goto cleanup;
	}

	if(fsync(fileno(handler->file_to_encrypt)) != 0)
	{
		perror("Error Syncing File");
		goto cleanup;
	}

	int file_desc = fileno(handler->file_to_encrypt);
	if(flock(file_desc, LOCK_EX) != 0)
	{
		goto cleanup;
	}

	if (fseeko( handler->file_to_encrypt, 0L, SEEK_END ) != 0 )
	{
		goto cleanup;
	}

	handler->filesize = ftello(handler->file_to_encrypt);	
	
	if(handler->filesize == -1L)
	{
		goto cleanup;
	}

	else if(handler->filesize == 0)
	{
		return_code = FLCN_EMPTY_FILE;
		goto cleanup;
	}

	size_t b_fsize	= handler->filesize / pg_opt->buffer_size;
	size_t r_bfsize	= handler->filesize % pg_opt->buffer_size;

	rewind(handler->file_to_encrypt);

	int passes = pg_opt->num_passes;
	if(passes <= 0)
	{
		goto cleanup;	
	}
	

	while(passes > 0)
	{
		rewind(handler->file_to_encrypt);
		
		for( size_t i = 0; i < b_fsize; i++)
		{	
			
			if(fwrite(handler->buffer, pg_opt->buffer_size, 1, handler->file_to_encrypt) != 1)
			{
				goto cleanup;
			}
		}
		
		if( r_bfsize != 0 )
		{	
			if(fwrite(handler->buffer, r_bfsize, 1, handler->file_to_encrypt) != 1)
			{
				goto cleanup;
			}
		}
		passes--;
	}


	if(fflush(handler->file_to_encrypt) != 0)
	{
		return_code = FLCN_FLUSH_ERR;
		goto cleanup;
	}
		
	if(fsync(fileno(handler->file_to_encrypt)) != 0)
	{
		return_code = FLCN_SYNC_ERR;
		goto cleanup;
	}

	
	int ver = _verify_shred(handler->file_to_encrypt, pg_opt->pattern, pg_opt->buffer_size,handler->filesize);
	
	if(ver == 0)
		return_code = FLCN_SUCCESS;			
	
	else
		return_code = FLCN_VERIFY_ERR;

cleanup:
	purge_secure_zero_memory(handler->buffer, pg_opt->buffer_size);

	flock(file_desc, LOCK_UN);
	
	if(handler->file_to_encrypt)
		fclose(handler->file_to_encrypt);

	return return_code;
}	

