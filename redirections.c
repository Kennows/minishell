/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:45:09 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/01 17:06:51 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file	*ft_new_file(t_file *file, char *name, t_file_type type)
{
	file = malloc(sizeof(t_file));
	if (!file)
		return (NULL);
	file->name = ft_strdup(name);
	if (!file->name)
		return (NULL);
	file->type = type;
	file->next = NULL;
	file->prev = NULL;
	return (file);
}

t_lex	*ft_remove_redirection(t_command *cmd, t_lex *current_token)
{
	if (cmd->token_end == current_token->next)
		cmd->token_end = current_token->prev;
	if (current_token->next->next == NULL)
	{
		if (cmd->token_start == current_token)
			cmd->token_start = NULL;
		ft_remove_token(current_token->next);
		ft_remove_token(current_token);
		return (NULL);
	}
	else
	{
		current_token = current_token->next->next;
		if (cmd->token_start == current_token->prev->prev)
		{
			if (cmd->token_end && current_token->index <= cmd->token_end->index)
				cmd->token_start = current_token;
			else
				cmd->token_start = NULL;
		}
		ft_remove_token(current_token->prev);
		ft_remove_token(current_token->prev);
		return (current_token);
	}
}

t_lex	*ft_parse_redirect_out(t_command *cmd, t_lex *current_token, t_file **file)
{
	t_file	*temp;

	temp = NULL;
	if (current_token->next == NULL || \
		cmd->token_end->index <= current_token->index || \
		current_token->next->type != WORD)
		write(2, "minishell:  syntax error near unexpected token `>'\n", 50);
	temp = ft_new_file(temp, current_token->next->str, CREATE);
	if (!temp)
		return (NULL);
	cmd->redir_out_file = temp;
	if (*file == NULL)
		*file = temp;
	else
	{
		while ((*file)->next != NULL)
			*file = (*file)->next;
		(*file)->next = temp;
		temp->prev = *file;
	}
	current_token = ft_remove_redirection(cmd, current_token);
	return (current_token);
}

t_lex	*ft_parse_append(t_command *cmd, t_lex *current_token, t_file **file)
{
	t_file	*temp;

	temp = NULL;
	if (current_token->next == NULL || \
		cmd->token_end->index <= current_token->index || \
		current_token->next->type != WORD)
		write(2, "minishell:  syntax error near unexpected token `>>'\n", 51);
	temp = ft_new_file(temp, current_token->next->str, ADD_TO);
	if (!temp)
		return (NULL);
	cmd->redir_out_file = temp;
	if (*file == NULL)
		*file = temp;
	else
	{
		while ((*file)->next != NULL)
			*file = (*file)->next;
		(*file)->next = temp;
		temp->prev = *file;
	}
	current_token = ft_remove_redirection(cmd, current_token);
	return (current_token);
}

t_lex	*ft_parse_redirect_in(t_command *cmd, t_lex *current_token, t_file **file)
{
	t_file	*temp;

	temp = NULL;
	if (current_token->next == NULL || \
		cmd->token_end->index <= current_token->index || \
		current_token->next->type != WORD)
		write(2, "minishell:  syntax error near unexpected token `<'\n", 50);
	temp = ft_new_file(temp, current_token->next->str, OPEN);
	if (!temp)
		return (NULL);
	cmd->redir_in_file = temp;
	if (*file == NULL)
		*file = temp;
	else
	{
		while ((*file)->next != NULL)
			*file = (*file)->next;
		(*file)->next = temp;
		temp->prev = *file;
	}
	current_token = ft_remove_redirection(cmd, current_token);
	return (current_token);
}

t_lex	*ft_parse_heredoc(t_command *cmd, t_lex *current_token, t_file **file)
{
	t_file		*temp;
	char		*name;

	temp = NULL;
	if (current_token->next == NULL || \
		cmd->token_end->index <= current_token->index || \
		current_token->next->type != WORD)
		write(2, "minishell:  syntax error near unexpected token `<<'\n", 50);
	name = ft_heredoc_name();
	if (!name)
		return (NULL);
	temp = ft_new_file(temp, name, OPEN);
	if (!temp)
		return (NULL);
	ft_create_heredoc(current_token->next->str, name);
	cmd->redir_in_file = temp;
	if (*file == NULL)
		*file = temp;
	else
	{
		while ((*file)->next != NULL)
			*file = (*file)->next;
		(*file)->next = temp;
		temp->prev = *file;
	}
	current_token = ft_remove_redirection(cmd, current_token);
	free(name);
	return (current_token);
}
