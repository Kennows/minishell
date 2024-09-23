/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:22:21 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/23 15:32:42 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute(t_command_table *table)
{
	t_command	*cmd;
	int		fd;

	cmd = table->commands;
	while (cmd != NULL)
	{
		if (cmd->redir_in_file != NULL)
		{
			fd = get_fd(cmd->redir_in_file);
			dup2(fd, 0);
			close(fd);
		}
		if (cmd->redir_out_file != NULL)
		{
			fd = get_fd(cmd->redir_out_file);
			dup2(fd, 1);
			close(fd);
		}
		table->exit_status = run_commands(cmd, table);
		if (table->exit_status != 0)
			return (table->exit_status);
		
		cmd = cmd->next;
	}
	return (0);
}

void	ft_minishell(t_command_table *table)
{
	t_lex	*tokens;
	char	*cmd;

	while (1)
	{
		ft_set_sig_handler();
		if (table->exit_status)
			printf("(%d)", table->exit_status);
		cmd = readline("minishell$ ");
		if (cmd == NULL)
		{
			printf("exit\n");
			return ;
		}
		add_history(cmd);
		tokens = ft_tokenize(cmd);
		free(cmd);
		if (!tokens || !ft_add_commands(&table, &tokens))
			continue ;
		if (ft_execute(table) == -1)
			return ;
		ft_free_files(&table->files);
		ft_free_commands(&table->commands);
		table->files = NULL;
		table->commands = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_command_table	*table;

	if (argc != 1 || argv[1])
	{
		write(2, "please execute with no arguments!\n", 34);
		return (0);
	}
	table = NULL;
	table = ft_create_cmd_table(table, envp);
	if (table)
	{
		ft_minishell(table);
		if (table->envp)
			ft_free_array(table->envp);
		ft_free_files(&table->files);
		free(table);
		rl_clear_history();
	}
	return (0);
}
