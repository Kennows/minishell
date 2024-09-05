/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:15:17 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/29 14:21:45 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*ft_parse_redirections(t_command *cmd, t_command_table *table)
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
		{
		//	ft_free_all(current_token, cmd, table);
			return (NULL);
		}
	}
	if (!ft_init_argv(cmd, table))
		return (NULL);
	if (cmd->next != NULL)
	{
		cmd->next = ft_parse_redirections(cmd->next, table);
		if (!cmd->next)
			return (NULL);
	}
	return (cmd);
}

t_command	*ft_parse_cmd_and_args(t_command *cmd)
{
	int		i;

	i = -1;
	while (++i < cmd->argc && cmd->token_start)
	{
		cmd->argv[i] = ft_expand(cmd->token_start->str, *cmd->envp);
		if (!cmd->argv[i])
			return (NULL);
		cmd->token_start = ft_remove_token(cmd->token_start);
	}
	ft_command_type(cmd);
	if (cmd->next == NULL)
		return (cmd);
	cmd->next = ft_parse_cmd_and_args(cmd->next);
	if (!cmd->next)
		return (NULL);
	return (cmd);
}

t_command	*ft_parse_pipes(t_command_table *table, t_command *head, \
		t_lex *tokens, t_command *prev)
{
	head = malloc(sizeof(t_command));
	if (!head)
	{
		write(2, "malloc failed while parsing pipes\n", 34);
		ft_free_tokens(&tokens);
		return (NULL);
	}
	ft_init_command(head, table);
	if (!ft_pipe_syntax_check(&tokens, &head))
		return (NULL);
	head->prev = prev;
	if (tokens->next != NULL)
	{
		ft_remove_token(tokens->next);
		head->next = ft_parse_pipes(table, head->next, tokens->next, \
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
	table = malloc(sizeof(t_command_table));
	if (!table)
	{
		write(2, "malloc failed while creating command table\n", 43);
		return (NULL);
	}
	table->commands = NULL;
	table->files = NULL;
	table->envp = ft_strarrdup(envp);
	if (!table->envp)
	{
		free(table);
		write(2, "error creating envp\n", 20);
		return (NULL);
	}
	return (table);
}

int	ft_add_commands(t_command_table **table, t_lex *tokens)
{
	t_command		*command;

	command = NULL;
	(*table)->commands = ft_parse_pipes(*table, command, tokens, NULL);
	if (!(*table)->commands)
		return (0);
	puts("pipes handled");
	command = (*table)->commands;
	command->envp = &(*table)->envp;
	command = ft_parse_redirections(command, *table);
	if (!command)
		return (0);
	puts("redirections handled");
	command = ft_parse_cmd_and_args(command);
	if (!command)
	{
		command = (*table)->commands;
		write(2, "malloc failed while parsing cmds and args\n", 42);
		ft_free_all(command->token_start, command, *table);
		return (0);
	}
	puts("cmds handled");
	puts("");
	return (1);
}
