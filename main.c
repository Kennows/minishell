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

int g_sig = 0;

void	ft_minishell(t_command_table *table)
{
	t_lex	*tokens;
	char	*cmd;

	while (1)
	{
		ft_set_sig_handler(1);
		cmd = readline("minishell$ ");
		ft_set_sig_handler(0);
		if (cmd == NULL)
			return ;
		add_history(cmd);
		tokens = ft_tokenize(cmd);
		free(cmd);
		if (!tokens || !ft_add_commands(&table, &tokens))
			continue ;
		if (run_commands(table) == -1)
			break ;
		dup2(table->saved_stdout, STDOUT_FILENO);
		dup2(table->saved_stdin, STDIN_FILENO);
		ft_free_files(&table->files);
		ft_free_commands(&table->commands);
		table->files = NULL;
		table->commands = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_command_table	*table;
	int				status;

	status = 0;
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
		status = ft_cleanup(table);
		free(table);
		printf("exit\n");
	}
	return (status);
}
