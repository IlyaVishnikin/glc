#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "file.h"

bool
glc_file_is_exists(GlcFile* self,
				   GlcFileExitStatus* error)
{
	if (error) *error = self ? GLC_FILE_EXIT_STATUS_OK : GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
	return self ? access(self->path, F_OK) == 0 : false;
}

bool
glc_file_is_readable(GlcFile* self,
					 GlcFileExitStatus* error)
{
	if (error) *error = self ? GLC_FILE_EXIT_STATUS_OK : GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
	return self ? access(self->path, R_OK) == 0 : false;
}

bool
glc_file_is_writable(GlcFile* self,
					 GlcFileExitStatus* error)
{
	if (error) *error = self ? GLC_FILE_EXIT_STATUS_OK : GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
	return self ? access(self->path, W_OK) == 0 : false;
}

bool
glc_file_is_executable(GlcFile* self,
					   GlcFileExitStatus* error)
{
	if (error) *error = self ? GLC_FILE_EXIT_STATUS_OK : GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
	return self ? access(self->path, X_OK) == 0 : false;
}

time_t
glc_file_get_modify_time(GlcFile* self,
						 GlcFileExitStatus* error)
{
	struct stat file_stat;
	stat(self->path, &file_stat);
	if (error) *error = self ? GLC_FILE_EXIT_STATUS_OK : GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
	return file_stat.st_mtime : (time_t)(-1);
}

GlcFile*
glc_file_new(const char* path)
{
	if (!path) return NULL;

	GlcFile* self = malloc(sizeof(GlcFile));

	/* methods */
	self->is_exists   	= glc_file_is_exists;
	self->is_readable 	= glc_file_is_readable;
	self->is_writable 	= glc_file_is_writable;
	self->is_executable = glc_file_is_executable;

	self->get_modify_time = glc_file_get_modify_time;

	/* fields */
	self->path = strdup(path);

	return self;
}

void
glc_file_free(GlcFile* self)
{
	if (!self) return;

	free(self->path);
	free(self);
}
