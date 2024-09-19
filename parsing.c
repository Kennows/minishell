/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:15:17 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:41:06 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_parse_redirections(t_command *cmd, t_command_table *table)
{
	t_lex	*current_token;

	current_token = cmd->token_start;
	while (current_token != NULL && cmd->token_end != NULL && \
		current_token->index <= cmd->token_end->index)
	{
		if (current_token->type == WORD || current_token->type == NUMBER || \
			current_token->type == BUILT_IN)
			current_token = current_token->next;
		else if (!ft_handle_redir(cmd, &current_token, table))
			return (0);
	}
	cmd->argc = 0;
	cmd->argv = NULL;
	if (cmd->next != NULL)
		if (!ft_parse_redirections(cmd->next, table))
			return (0);
	return (1);
}

t_command	*ft_parse_pipes(t_command_table *table, t_command *head, \
		t_lex **tokens, t_command *prev)
{
	head = malloc(sizeof(t_command));
	if (!head)
	{
		write(2, "malloc failed while parsing pipes\n", 34);
		ft_free_tokens(&*tokens);
		return (NULL);
	}
	ft_init_command(head, table);
	if (!ft_pipe_syntax_check(&*tokens, &head))
		return (NULL);
	head->prev = prev;
	if ((*tokens)->next != NULL && (*tokens)->next->type == PIPE)
	{
		*tokens = (*tokens)->next;
		*tokens = ft_remove_token(&*tokens, head);
		head->next = ft_parse_pipes(table, head->next, &*tokens, \
				head);
		if (!head->next)
		{
			free(head);
			return (NULL);
		}
		head->pipe_out = head->next;
		head->next->pipe_in = head;
	}
	return (head);
}

t_command_table	*ft_create_cmd_table(t_command_table *table, char **envp)
{
	char	*shell;

	table = malloc(sizeof(t_command_table));
	if (!table)
	{
		write(2, "malloc failed while creating command table\n", 43);
		return (NULL);
	}
	ft_init_table(table);
	shell = getcwd(NULL, 0);
	if (!shell || !ft_strprepend(&shell, "SHELL=") || \
		!ft_strcombine(&shell, "/minishell"))
		write(2, "error creating SHELL env while creating cmd table\n", 50);
	else
	{
		table->envp = ft_create_envp(&*table->envp, envp, &*shell);
		if (table->envp)
			return (table);
		write(2, "error creating envp while creating command table\n", 49);
	}	
	free(table);
	free(shell);
	return (NULL);
}

int	ft_add_commands(t_command_table **table, t_lex **tokens)
{
	t_command		*command;

	command = NULL;
	(*table)->commands = ft_parse_pipes(*table, command, &*tokens, NULL);
	if (!(*table)->commands)
		return (0);
	command = (*table)->commands;
	if (!ft_parse_redirections(command, *table))
		return (0);
	if (!ft_parse_cmds_and_args(command))
	{
		write(2, "error parsing commands and arguments\n", 37);
		ft_free_all((*table)->commands->token_start, command, *table);
		return (0);
	}
	return (1);
}
