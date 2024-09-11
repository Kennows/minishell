/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:07:16 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/10 15:07:23 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_command *cmd)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	while (cmd->argv[i])
	{
		if (i == 1 && !ft_strncmp(cmd->argv[1], "-n", 2))
		{
			nl = 1;
			while (cmd->argv[1][nl] == 'n')
				nl++;
			if (cmd->argv[1][nl] == '\0')
			{
				nl = 0;
				i++;
			}
		}
		else if (i > 1)
			printf(" ");
		printf("%s", cmd->argv[i++]);
	}
	if (nl)
		printf("\n");
	return (0);
}

int	ft_exit(t_command_table *table)
{
	ft_free_files(&table->files);
	ft_free_commands(&table->commands);
	ft_free_array(table->envp);
	free(table);
	rl_clear_history();
	printf("exit\n");
	return (-123);
}