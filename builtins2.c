/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:07:16 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:56:14 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_command *cmd)
{
	int	i;
	int	i2;
	int	nl;

	i = 0;
	nl = 1;
	while (cmd->argc > 1 && cmd->argv[++i] && \
			!ft_strncmp(cmd->argv[i], "-n", 2))
	{
		i2 = 1;
		while (cmd->argv[i][i2] == 'n')
			i2++;
		if (cmd->argv[i][i2] != '\0')
			break ;
		nl = 0;
	}
	while (cmd->argv[i] && i > 0)
	{
		printf("%s", cmd->argv[i++]);
		if (cmd->argv[i] != NULL)
			printf(" ");
	}
	if (nl)
		printf("\n");
	return (0);
}

int	ft_cleanup(t_command_table *table)
{
	int	status;

	status = 0;
	if (table)
	{
		status = table->exit_status;
		if (table->files)
			ft_free_files(&table->files);
		if (table->commands)
			ft_free_commands(&table->commands);
		if (table->envp)
			ft_free_array(&*table->envp);
		table->envp = NULL;
		close(table->saved_stdin);
		close(table->saved_stdout);
		rl_clear_history();
	}
	return (status);
}

int	ft_exit(t_command *cmd, t_command_table *table)
{
	int	status;

	status = 0;
	if (table)
	{
		if (cmd->argc > 2)
		{
			write(2, "minishell: exit: too many arguments\n", 36);
			table->exit_status = 1;
			return (1);
		}
		if (cmd->argc == 2)
		{
			if (ft_isnumber(cmd->argv[1]))
				status = ft_atoi(cmd->argv[1]);
			else
			{
				ft_print_exit_warning(cmd->argv[1]);
				status = 2;
			}
		}
		table->exit_status = status;
		ft_cleanup(table);
	}
	return (-1);
}
