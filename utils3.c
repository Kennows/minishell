/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:51:09 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:51:50 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_redir_env_error(char *str)
{
	int	i;

	i = ft_find_env_start(str, 0);
	write(2, "minishell: ", 11);
	while (str[i])
		write(2, &str[i++], 1);
	write(2, ": ambiguous redirect\n", 21);
}

void	ft_print_heredoc_warning(char *delimiter)
{
	int	i;

	i = 0;
	write(2, "minishell: warning: here-document delimited by end-of-file ", 59);
	write(2, "(wanted `", 9);
	while (delimiter[i])
		write(2, &delimiter[i++], 1);
	write(2, "')\n", 3);
}

void	ft_print_exit_warning(char *arg)
{
	int	i;

	i = 0;
	write(2, "minishell: exit: ", 18);
	while (arg[i])
		write(2, &arg[i++], 1);
	write(2, ": numeric argument required\n", 28);
}

void	set_pipeline(int pipe_in, int *pipe_fd)
{
	if (pipe_in != -1)
	{
		dup2(pipe_in, STDIN_FILENO);
		close(pipe_in);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}

void	wait_for_children(t_command_table *table)
{
	int	status;
	int	status_last;
	int	i;

	if (waitpid(-1, &i, WNOHANG) == 0)
	{
		waitpid(table->last_pid, &status_last, 0);
		while (wait(&status) > 0)
			continue ;
		if (WIFEXITED(status_last))
			table->exit_status = WEXITSTATUS(status_last);
	}
}
