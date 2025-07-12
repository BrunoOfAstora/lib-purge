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
		perror("malloc error in Handler_PG struct");
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
		printf("error: the free space available is 0\n");
		return FLCN_AVAIL_SPACE_ERR;
	}	

	unsigned char *buffer = malloc(pg_opt->buffer_size);
	if(!buffer)
	{
		perror("error: error while alocating memory in fs_wipe function\n");
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
	
	remove("_flcn-purge-freespace_");

	return FLCN_SUCCESS;
}

int _verify_shred(FILE *cmp_file, unsigned char cmp_pattern, int buffsize, off_t filesize)
{
	rewind(cmp_file);	
	
	char *cmp_file_buffer = malloc(buffsize);
	if(!cmp_file_buffer)
	{
		perror("malloc error in _verify_shred function, could not initialize verify function\n");
		return FLCN_MALLOC_ERR;
	}

	char *cmp_pattern_buffer = malloc(buffsize);
	if(!cmp_file_buffer)
	{
		printf("malloc error in _verify_shred function, could not initialize verify function\n");
		return FLCN_MALLOC_ERR;
	}

	memset(cmp_pattern_buffer, cmp_pattern, buffsize);

	off_t block_file_size = filesize / buffsize;

	for(off_t i = 0; i < block_file_size; i++)
	{
		if((fread(cmp_file_buffer, buffsize, 1, cmp_file)) != 1)
		{
			printf("fread error in the verify function!\n");
			break;
		}
		
		if((memcmp(cmp_file_buffer, cmp_pattern_buffer, buffsize)) != 0)
		{
			printf("the file was not shredded properly, the file checking resulted in blocks of memory that were not defined in pattern\n");
			free(cmp_pattern_buffer);
			free(cmp_file_buffer);

			return FLCN_VERIFY_DIFF_ERR;
		}
	}
	
	free(cmp_pattern_buffer);
	free(cmp_file_buffer);
	
	rewind(cmp_file);

	return FLCN_SUCCESS;
} 


//Funtion to wipe entire files with a pattern, consequently, wiping the memory space where this file was originally.
int file_wipe( Handler_PG *handler, const Purge_opt *pg_opt )
{
	int return_code = -1;

//	struct stat buf;
//	int f_status = 0;
		
	if(pg_opt == NULL || pg_opt->filename == NULL || handler == NULL)
	{
		fprintf(stderr, "Error: You need to specify the parameters and the file name\n");
		return -1;
	}	

	handler->buffer = malloc(pg_opt->buffer_size);

	if(handler->buffer == NULL)
	{
		perror("\nMemory allocation failed\n");
		goto cleanup;
	}
	
	memset( handler->buffer, pg_opt->pattern, pg_opt->buffer_size );
	
	handler->file_to_encrypt = fopen(pg_opt->filename, "r+b");
	if(!handler->file_to_encrypt)
	{
		perror("Error While Opening File");
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
		perror("Failed to lock file");
		goto cleanup;
	}

	if (fseeko( handler->file_to_encrypt, 0L, SEEK_END ) != 0 )
	{
		perror("Error: fseek could not reach the end of file");
		goto cleanup;
	}

	handler->filesize = ftello(handler->file_to_encrypt);	
	
	if(handler->filesize == -1L)
	{
		perror("Error: ftell failed");
		goto cleanup;
	}

	else if(handler->filesize == 0)
	{
		perror("The file is empty. file size = 0");
		goto cleanup;
	}

	size_t b_fsize	= handler->filesize / pg_opt->buffer_size;
	size_t r_bfsize	= handler->filesize % pg_opt->buffer_size;

	rewind(handler->file_to_encrypt);

	int passes = pg_opt->num_passes;

	if(passes <= 0)
	{
		perror("Passes can't be 0 or lower");
		goto cleanup;	
	}
	

	while(passes > 0)
	{
		rewind(handler->file_to_encrypt);
		
		for( size_t i = 0; i < b_fsize; i++)
		{	
			
			if(fwrite(handler->buffer, pg_opt->buffer_size, 1, handler->file_to_encrypt) != 1)
			{
				perror("Error Writing full block");
				goto cleanup;
			}
		}
		
		if( r_bfsize != 0 )
		{	
			if(fwrite(handler->buffer, r_bfsize, 1, handler->file_to_encrypt) != 1)
			{
				perror("Error writing remainder block");
				goto cleanup;
			}
		}
		passes--;
	}


	if(fflush(handler->file_to_encrypt) != 0)
	{
		perror("Error Flushing File");
		goto cleanup;
	}
		
	if(fsync(fileno(handler->file_to_encrypt)) != 0)
	{
		perror("Error Syncing File");
		goto cleanup;
	}

	
	printf("Verifying...\n");
	int ver = _verify_shred(handler->file_to_encrypt, pg_opt->pattern, 256,handler->filesize);
	
	if(ver == 0)
	{
		printf("Verification: Success.\n");
	}
	else
	{
		printf("Verification: Failed.\n");
	}

	return_code = 0;

cleanup:
	purge_secure_zero_memory(handler->buffer, pg_opt->buffer_size);

	flock(file_desc, LOCK_UN);
	
	if(handler->file_to_encrypt)
		fclose(handler->file_to_encrypt);

	return return_code;
}	

