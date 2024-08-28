/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 19:21:02 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/28 19:22:19 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_command(t_command *cmd)
{
	cmd->argv = NULL;
	cmd->pipe_in = NULL;
	cmd->pipe_out = NULL;
	cmd->redir_in_file = NULL;
	cmd->redir_out_file = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
}

int	ft_init_argv(t_command *cmd, t_command_table *table)
{
	int	i;

	i = 0;
	if (cmd->token_end && cmd->token_start)
		cmd->argc = (cmd->token_end->index - cmd->token_start->index) + 1;
	else
		cmd->argc = 1;
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	if (!cmd->argv)
	{
		write(2, "malloc failed while creating argv\n", 34);
		ft_free_all(cmd->token_start, cmd, table);
		return (0);
	}
	while (i <= cmd->argc)
		cmd->argv[i++] = NULL;
	return (1);
}
