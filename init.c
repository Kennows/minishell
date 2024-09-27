/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 19:21:02 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:56:57 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_command(t_command *cmd, t_command_table *table)
{
	cmd->argv = NULL;
	cmd->argc = 0;
	cmd->envp = &table->envp;
	cmd->type = WORD;
	cmd->token_start = NULL;
	cmd->token_end = NULL;
	cmd->pipe_in = NULL;
	cmd->pipe_out = NULL;
	cmd->redir_in_start = NULL;
	cmd->redir_in_end = NULL;
	cmd->redir_out_start = NULL;
	cmd->redir_out_end = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
}

void	ft_init_subtoken(t_sub_tok *subtoken, int quote)
{
	subtoken->prev = NULL;
	subtoken->next = NULL;
	subtoken->str = NULL;
	if (quote == '"')
		subtoken->quote = 2;
	else if (quote == '\'')
		subtoken->quote = 1;
	else
		subtoken->quote = 0;
}

void	ft_init_table(t_command_table *table)
{
	table->commands = NULL;
	table->files = NULL;
	table->envp = NULL;
	table->exit_status = 0;
	table->saved_stdin = dup(STDIN_FILENO);
	table->saved_stdout = dup(STDOUT_FILENO);
	table->last_pid = 0;
}
