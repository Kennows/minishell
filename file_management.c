#include "minishell.h"

int	open_file(t_file *file)
{
	int	fd;

	if (file->type == OPEN)
		fd = open(file->name, O_RDONLY);
	else if (file->type == CREATE)
		fd = open(file->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file->name, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd < 0)
		return (-1);
	else
		return (fd);
}

int	get_fd(t_file *file)
{
	int	fd;

	if (file->type == OPEN)
	{
		if (access(file->name, F_OK) != 0)
			return (-1); //file does not exist
		else if (access(file->name, R_OK) != 0)
			return (-1); //cannot read
		else
			fd = open_file(file);
	}
	else if (file->type == CREATE)
	{
		if (access(file->name, F_OK) == 0 && access(file->name, W_OK) != 0)
		       return (-1); //no permission error	
		fd = open_file(file);
	}
	else
	{
		if (access(file->name, F_OK) == 0 && access(file->name, W_OK) != 0)
			return (-1); //no permission error
		fd = open_file(file);
	}
	return (fd);
}
