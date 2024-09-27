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
	int result;

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
	table->exit_status = ft_prepare_path(cmd, &cmd_path);
	if (table->exit_status != 1)
	{
		if (table->exit_status != 127)
			perror("minishell");
		return (table->exit_status);
	}
	if (set_redirections(cmd))
	{
		free(cmd_path);
		perror("minishell: redirection");
		return (-1);
	}
	if (execve(cmd_path, cmd->argv, *cmd->envp) == -1)
	{
		perror("Execution error");
		free(cmd_path);
		return (-1);
	}
	free(cmd_path);
	return (0);
}

int	fork_and_execute(t_command *cmd, t_command_table *table, \
						int pipe_in, int *pipe_fd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork error");
		return (-1);
	}
	if (pid == 0)
	{
		set_pipeline(pipe_in, pipe_fd);
		if (cmd->type == BUILT_IN)
			execute_builtin(cmd, table);
		else
			table->exit_status = execute_command(cmd, table);
		status = ft_cleanup(table);
		free(table);
		exit(status);
	}
	else
	{
		ft_ignore_signals();
		if (pipe_in != -1)
			close(pipe_in);
	}
	return (0);
}

void	wait_for_children(t_command_table *table)
{
	int	status;

	while (wait(&status) > 0)
		continue ;
	if (WIFEXITED(status))
		table->exit_status = WEXITSTATUS(status);
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
			dup2(table->saved_stdout, STDOUT_FILENO);
			dup2(table->saved_stdin, STDIN_FILENO);
			return (table->exit_status);
		}
		else if (fork_and_execute(cmd, table, pipe_in, pipe_fd) == -1)
			return (-1);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		pipe_in = pipe_fd[0];
		cmd = cmd->pipe_out;
	}
	wait_for_children(table);
	return (table->exit_status);
}
