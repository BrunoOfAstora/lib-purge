			//f_filler.c//


#include "internal_utils.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int fill_pattern( const char *file_name, unsigned char pattern, size_t buffer_size )
{
	
	if(buffer_size == 0)
	{
		fprintf(stderr, "Error: buffer_size can't be zero\n");
		return -1;
	}

	unsigned char *buffer = NULL;

	size_t block_file_size; 
	size_t remainder_block_file_size;

	off_t file_size;
 
	FILE *file = NULL;

	int return_code = -1;


	buffer = malloc(buffer_size * sizeof(unsigned char));

	if(buffer == NULL)
	{
		perror("\nMemory allocation failed\n");
		goto cleanup;
	}
	
	memset( buffer, pattern, buffer_size );
	
	file = fopen(file_name, "r+b");

	if(!file)
	{
		perror("Error While Opening File");
		goto cleanup;
	}

	if (fseeko( file, 0L, SEEK_END ) != 0 )
	{
		perror("Error: fseek could not reach the end of file");
		goto cleanup;
	}

	file_size = ftello(file);	
	
	if(file_size == -1L)
	{
		perror("Error: ftell failed");
		goto cleanup;
	}

	else if(file_size == 0)
	{
		perror("The file is empty. file size = 0");
		goto cleanup;
	}

	block_file_size = file_size / buffer_size;
	remainder_block_file_size = file_size % buffer_size;

	rewind(file);

	for( size_t i = 0; i < block_file_size; i++)
	{
		if(fwrite(buffer, buffer_size, 1, file) != 1)
		{
			perror("Error Writing full block");
			goto cleanup;
		}
	}

	if( remainder_block_file_size != 0 )
	{	
		if(fwrite(buffer, remainder_block_file_size, 1, file) != 1)
		{
			perror("Error writing remainder block");
			goto cleanup;
		}
	}

	if(fflush(file) != 0)
	{
		perror("Error Flushing File");
		goto cleanup;
	}
		
	if(fsync(fileno(file)) != 0)
	{
		perror("Error Syncing File");
		goto cleanup;
	}

	return_code = 0;

cleanup:
	purge_secure_zero_memory(buffer, buffer_size);
	
	if(file)
		fclose(file);
	
	if(buffer)
		free(buffer);

	return return_code;
}	

