/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 17:15:17 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/01 17:09:54 by nvallin          ###   ########.fr       */
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
		if (current_token->type == REDIR_IN)
			current_token = ft_parse_redirect_in(cmd, current_token, &table->files);
		else if (current_token->type == REDIR_OUT)
			current_token = ft_parse_redirect_out(cmd, current_token, &table->files);
		else if (current_token->type == APPEND)
			current_token = ft_parse_append(cmd, current_token, &table->files);
		else if (current_token->type == HERE_DOC)
			current_token = ft_parse_heredoc(cmd, current_token, &table->files);
		else
			current_token = current_token->next;
	}
	if (cmd->token_end && cmd->token_start)
		cmd->argc = (cmd->token_end->index - cmd->token_start->index) + 1;
	else
		cmd->argc = 1;	
	if (cmd->next != NULL)
		cmd->next = ft_parse_redirections(cmd->next, table);
	return (cmd);
}

t_command	*ft_parse_cmd_and_args(t_command *cmd)
{
	t_lex	*current_token;
	t_lex	*temp;
	int		i;

	i = -1;
	current_token = cmd->token_start;
	if (current_token != NULL)
		cmd->type = current_token->type;
	cmd->argv = malloc(sizeof(char *) * (cmd->argc + 1));
	if (!cmd->argv)
		return (NULL);
	while (++i < cmd->argc && current_token)
	{
		cmd->argv[i] = ft_strdup(current_token->str);
		if (!cmd->argv[i])
			return (NULL);
		temp = current_token->next;
		ft_remove_token(current_token);
		current_token = temp;
	}
	cmd->argv[i] = NULL;
	if (cmd->next != NULL)
		cmd->next = ft_parse_cmd_and_args(cmd->next);
	return (cmd);
}

t_command	*ft_parse_pipes(t_command *head, t_lex *tokens, t_command *prev)
{
	head = malloc(sizeof(t_command));
	if (!head)
		return (0);
	if (tokens->type == PIPE)
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
	head->token_start = tokens;
	while (tokens->next != NULL && tokens->next->type != PIPE)
		tokens = tokens->next;
	head->token_end = tokens;
	ft_init_command(head);
	head->prev = prev;
	if (tokens->next != NULL)
	{
		if (tokens->next->next == NULL)
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		ft_remove_token(tokens->next);
		head->next = ft_parse_pipes(head->next, tokens->next, head);
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

t_command_table	*ft_add_commands(t_lex *tokens)
{
	t_command_table	*table;
	t_command		*command;

	command = NULL;
	table = NULL;
	table = malloc(sizeof(t_command_table));
	if (!table)
		return (NULL);
	table->commands = NULL;
	table->files = NULL;
	table->commands = ft_parse_pipes(command, tokens, NULL);
	if (!table->commands)
		return (NULL);
	puts("pipes handled");
	command = table->commands;
	command = ft_parse_redirections(command, table);
	puts("redirections handled");
	command = ft_parse_cmd_and_args(command);
	puts("cmds handled");
	puts("");
	return (table);
}
