#ifndef __GLC_FILES_FILE_H__
#define __GLC_FILES_FILE_H__

#include <time.h>
#include <stdbool.h>

#ifndef GLC_FILE_PATH_SEPARATOR
	#ifdef _WIN32
		#define GLC_FILE_PATH_SEPARATOR '\\'
	#else
		#define GLC_FILE_PATH_SEPARATOR '/'
	#endif
#endif

typedef enum _GlcFileExitStatus GlcFileExitStatus;
enum _GlcFileExitStatus
{
	GLC_FILE_EXIT_STATUS_OK,
	GLC_FILE_EXIT_STATUS_SELF_IS_NULL,
	GLC_FILE_EXIT_STATUS_FILE_NOT_EXISTS,
	GLC_FILE_EXIT_STATUS_FILE_NOT_READABLE,
	GLC_FILE_EXIT_STATUS_FILE_NOT_WRITABLE,
};

typedef struct _GlcFile GlcFile;
struct _GlcFile
{
	char* path; /* path to the file with that the structure GlcFile associated */

	bool (*is_exists)  	 (GlcFile* self, GlcFileExitStatus* error); /* checks that file exists */
	bool (*is_readable)	 (GlcFile* self, GlcFileExitStatus* error); /* checks that file is readble */
	bool (*is_writable)  (GlcFile* self, GlcFileExitStatus* error); /* checks that file is writable */
	bool (*is_executable)(GlcFile* self, GlcFileExitStatus* error); /* checks taht file is executable */

	time_t (*get_modify_time)(GlcFile* self, GlcFileExitStatus* error); /* return file creation time */
	char*  (*get_folder)	 (GlcFile* self, GlcFileExitStatus* error); /* return path to the folder that contains current file */
	char*  (*get_owner)		 (GlcFile* self, GlcFileExitStatus* error); /* return file owner */
	char*  (*get_group)		 (GlcFile* self, GlcFileExitStatus* error); /* return file group */
	char*  (*get_text)		 (GlcFile* self, GlcFileExitStatus* error); /* return text that file contains */
	size_t (*get_size)		 (GlcFile* self, GlcFileExitStatus* error); /* return size of the file in bytes */
	size_t (*get_lines_count)(GlcFile* self, GlcFileExitStatus* error);

	void (*clear)(GlcFile* self, 					 			      GlcFileExitStatus* error); /* clear all content in the file */
	void (*puts) (GlcFile* self, const char* string, int line_number, GlcFileExitStatus* error); /* adds string into the file */
	void (*gets) (GlcFile* self, 					 int line_number, GlcFileExitStatus* error); /* return string at @line_number position in the file */

	void (*remove)(GlcFile* self, GlcFileExitStatus* error); /* remove file. If file is not exists that nothing happens */
	void (*create)(GlcFile* self, GlcFileExitStatus* error); /* create file. If file already exists than nothing happens */
	void (*close) (GlcFile* self, GlcFileExitStatus* error); /* close connection with file */
};

GlcFile* glc_file_new(const char* path);
void 	 glc_file_free(GlcFile* self);

#endif
