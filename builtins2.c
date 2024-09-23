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

int	ft_exit(t_command_table *table)
{
	ft_free_files(&table->files);
	ft_free_commands(&table->commands);
	ft_free_array(&*table->envp);
	table->envp = NULL;
	rl_clear_history();
	printf("exit\n");
	return (-1);
}
