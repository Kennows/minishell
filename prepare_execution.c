/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:41:39 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/26 11:56:37 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_get_path(char ***path_arr, t_command *cmd)
{
	char	*path;
	int		i;

	i = 0;
	path = ft_getenv("PATH", *cmd->envp);
	if (path == NULL)
	{
		if (!ft_array_append(&path_arr[0], "/"))
			return (0);
		return (1);
	}
	path_arr[0] = ft_split(path, ':');
	if (path_arr[0] == NULL)
		return (0);
	while (path_arr[0][i])
	{
		if (!ft_strcombine(&path_arr[0][i], "/"))
		{
			ft_free_array(path_arr[0]);
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_access(t_command *cmd, char **cmd_path, char **path_arr)
{
	int	i;

	i = -1;
	while (path_arr[++i] != NULL)
	{
		if (*cmd_path)
			free(*cmd_path);
		*cmd_path = NULL;
		*cmd_path = ft_strjoin(path_arr[i], cmd->argv[0]);
		if (*cmd_path && (access(*cmd_path, F_OK) != 0 && \
							access(*cmd_path, X_OK) != 0))
			continue ;
		break ;
	}
	if (path_arr[i] && !*cmd_path)
		write(2, "ft_strjoin failed while getting path\n", 37);
	else if (!path_arr[i] && *cmd_path)
	{
		free(*cmd_path);
		*cmd_path = NULL;
	}
	ft_free_array(path_arr);
	if (*cmd_path)
		return (1);
	return (0);
}

int	ft_prepare_path(t_command *cmd, char **cmd_path)
{
	char	**path_arr;

	if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.')
	{
		if (access(cmd->argv[0], F_OK) != 0 || access(cmd->argv[0], X_OK) != 0)
			return (0);
		*cmd_path = ft_strdup(cmd->argv[0]);
		if (*cmd_path)
			return (1);
		write(2, "ft_strdup failed while creating cmd_path\n", 41);
		return (0);
	}
	path_arr = NULL;
	if (!ft_get_path(&path_arr, cmd))
	{
		write(2, "error getting path\n", 19);
		return (0);
	}
	if (!check_access(cmd, &*cmd_path, &*path_arr))
	{
		write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
		write(2, ": command not found\n", 20);
		return (127);
	}
	return (1);
}

int	set_redirections(t_command *cmd)
{
	int		fd;
	t_file	*redir_out;

	if (cmd->redir_in_file)
	{
		fd = open(cmd->redir_in_file->name, O_RDONLY);
		if (fd < 0)
			return (1);
		if (cmd->type != BUILT_IN)
			dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->redir_out_file)
	{
		redir_out = cmd->redir_out_file;
		if (redir_out->type == ADD_TO)
			fd = open(redir_out->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(redir_out->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			return (1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (0);
}

int	prepare_pipes(t_command *cmd, int *pipe_fd)
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (cmd->pipe_out && pipe(pipe_fd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	return (0);
}
