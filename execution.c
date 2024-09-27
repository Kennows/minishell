/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:32:47 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/26 11:56:57 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_command *cmd, t_command_table *table)
{
	int	result;

	table->exit_status = ft_check_files(cmd);
	if (table->exit_status)
		return (1);
	table->exit_status = set_redirections(cmd);
	if (table->exit_status)
	{
		perror("minishell: redirection");
		return (table->exit_status);
	}
	result = ft_builtin(cmd, table);
	return (result);
}

int	execute_command(t_command *cmd, t_command_table *table)
{
	char	*cmd_path;

	cmd_path = NULL;
	table->exit_status = ft_check_files(cmd);
	if (table->exit_status)
		return (1);
	table->exit_status = ft_prepare_path(cmd, &cmd_path);
	if (table->exit_status != 0 && table->exit_status != 127)
		perror("minishell");
	if (table->exit_status != 0 || !cmd->argv)
		return (table->exit_status);
	if (set_redirections(cmd))
	{
		free(cmd_path);
		perror("minishell: redirection");
		return (1);
	}
	if (execve(cmd_path, cmd->argv, *cmd->envp) == -1)
	{
		perror("Execution error");
		free(cmd_path);
		return (1);
	}
	free(cmd_path);
	return (0);
}

void	ft_child_process(t_command *cmd, t_command_table *table, \
							int pipe_in, int *pipe_fd)
{
	int		status;	

	ft_set_sig_handler(1);
	set_pipeline(pipe_in, pipe_fd);
	if (cmd->type == BUILT_IN)
		execute_builtin(cmd, table);
	else
		table->exit_status = execute_command(cmd, table);
	status = ft_cleanup(table);
	free(table);
	exit(status);
}

int	fork_and_execute(t_command *cmd, t_command_table *table, \
						int pipe_in, int *pipe_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork error");
		return (1);
	}
	if (pid == 0)
		ft_child_process(cmd, table, pipe_in, pipe_fd);
	else
	{
		table->last_pid = pid;
		ft_ignore_signals();
		if (pipe_in != -1)
			close(pipe_in);
	}
	return (0);
}

int	run_commands(t_command_table *table)
{
	t_command	*cmd;
	int			pipe_fd[2];
	int			pipe_in;

	pipe_in = -1;
	cmd = table->commands;
	while (cmd)
	{
		if (prepare_pipes(cmd, pipe_fd) == -1)
			return (2);
		if (cmd == table->commands && !cmd->pipe_out && cmd->type == BUILT_IN)
		{
			if (execute_builtin(cmd, table) == -1)
				return (-1);
			return (table->exit_status);
		}
		else if (fork_and_execute(cmd, table, pipe_in, pipe_fd) != 0)
			return (1);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		pipe_in = pipe_fd[0];
		cmd = cmd->pipe_out;
	}
	wait_for_children(table);
	return (table->exit_status);
}
