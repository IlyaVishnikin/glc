#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

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
	if (!self)
	{
		if (error) *error = GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
		return (time_t)(-1);
	}

	if (!self->is_exists(self, NULL))
	{
		if (error) *error = GLC_FILE_EXIT_STATUS_FILE_NOT_EXISTS;
		return (time_t)(-1);
	}

	struct stat file_stat;
	stat(self->path, &file_stat);
	if (error) *error = GLC_FILE_EXIT_STATUS_OK;
	return file_stat.st_mtime;
}

char*
glc_file_get_folder(GlcFile* self,
					GlcFileExitStatus* error)
{
	if (!self)
	{
		if (error) *error = GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
		return NULL;
	}

	if (error) *error = GLC_FILE_EXIT_STATUS_OK;
	for (int i = strlen(self->path)-1; i >= 0; i--)
		if (self->path[i] == GLC_FILE_PATH_SEPARATOR)
		{
			char folder_path[i+2];
			strncpy(folder_path, self->path, i+1);
			folder_path[i+1] = '\0';
			return strdup(folder_path);
		}

	return NULL;
}

char*
glc_file_get_owner(GlcFile* self,
				   GlcFileExitStatus* error)
{
	if (!self)
	{
		if (error) *error = GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
		return NULL;
	}

	if (!self->is_exists(self, NULL))
	{
		if (error) *error = GLC_FILE_EXIT_STATUS_FILE_NOT_EXISTS;
		return NULL;
	}

	if (error) *error = GLC_FILE_EXIT_STATUS_OK;
	struct stat file_stat;
	stat(self->path, &file_stat);
	struct passwd* pwd = getpwuid(file_stat.st_uid); 
	return pwd ? strdup(pwd->pw_name) : NULL;
}

char*
glc_file_get_group(GlcFile* self,
				   GlcFileExitStatus* error)
{
	if (!self)
	{
		if (error) *error = GLC_FILE_EXIT_STATUS_SELF_IS_NULL;
		return NULL;
	}
	if (!self->is_exists(self, NULL))
	{
		if (error) *error = GLC_FILE_EXIT_STATUS_FILE_NOT_EXISTS;
		return NULL;
	}

	if (error) *error = GLC_FILE_EXIT_STATUS_OK;
	struct stat file_stat;
	stat(self->path, &file_stat);
	struct group* group = getgrgid(file_stat.st_gid);
	return group ? strdup(group->gr_name) : NULL;
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
	self->get_folder 	  = glc_file_get_folder;
	self->get_owner 	  = glc_file_get_owner;
	self->get_group 	  = glc_file_get_group;

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
