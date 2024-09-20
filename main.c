/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:22:21 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/07 20:02:59 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute(t_command_table *table)
{
	t_command	*cmd;

	cmd = table->commands;
	while (cmd != NULL)
	{
		if (cmd->type == BUILT_IN)
			table->exit_status = ft_builtin(cmd, table);
		if (table->exit_status)
			return (table->exit_status);
		cmd = cmd->next;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_lex			*tokens;
	t_command_table	*table;
	char			*cmd;

	if (argc != 1 || argv[1])
	{
		write(2, "please execute with no arguments!\n", 34);
		return (0);
	}
	table = NULL;
	table = ft_create_cmd_table(table, envp);
	if (!table)
		return (0);
	while (1)
	{
		ft_set_sig_handler();
		cmd = readline("minishell$ ");
		if (cmd == NULL)
		{
			printf("exit\n");
			return (0);		
		}
		if (!ft_strncmp(cmd, "exit", 5))
		{
			free(cmd);
			if (table->envp)
				ft_free_array(table->envp);
			free(table);
			rl_clear_history();
			printf("exit\n");
			return (0);
		}
		add_history(cmd);
		tokens = ft_tokenize(cmd);
		free(cmd);
		if (!tokens || !ft_add_commands(&table, &tokens))
			continue ;
		if (ft_execute(table))
			printf("(%d)", table->exit_status);
		ft_free_files(&table->files);
		ft_free_commands(&table->commands);
		table->files = NULL;
		table->commands = NULL;
	}
	return (0);
}
