/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:38:47 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/26 11:53:37 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_in_files(t_command *cmd)
{
	t_file	*current;
	int		fd;

	current = cmd->redir_in_start;
	while (current)
	{
		if (current->type != HEREDOC)
		{
			fd = open(current->name, O_RDONLY);
			if (fd < 0)
			{
				perror("minishell");
				return (1);
			}
			close (fd);
		}
		if (current == cmd->redir_in_end)
			current = NULL;
		else
			current = current->next;
	}
	return (0);
}

int	ft_check_out_files(t_command *cmd)
{
	t_file	*current;
	int		fd;

	current = cmd->redir_out_start;
	while (current)
	{
		if (current->type == ADD_TO)
			fd = open(current->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (current->type == CREATE)
			fd = open(current->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("minishell");
			return (1);
		}
		else
			close (fd);
		if (current == cmd->redir_out_end)
			current = NULL;
		else
			current = current->next;
	}
	return (0);
}

int	ft_check_files(t_command *cmd)
{
	if (cmd && (ft_check_in_files(cmd) || ft_check_out_files(cmd)))
		return (1);
	return (0);
}

void	ft_add_file_to_list(t_command_table *table, t_file *temp)
{
	t_file	*file;
	int		i;

	i = 0;
	file = NULL;
	if (table->files == NULL)
	{
		table->files = temp;
		table->files->index = 0;
	}
	else
	{
		file = table->files;
		while (file->next != NULL)
		{
			file = file->next;
			file->index = ++i;
		}
		file->next = temp;
		file->next->index = ++i;
		temp->prev = file;
	}
}
