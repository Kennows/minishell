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

t_file	*ft_new_file(t_file *file, char **name, t_token_type type)
{
	file = malloc(sizeof(t_file));
	if (!file)
	{
		free(*name);
		write(2, "malloc failed while listing files\n", 34);
		return (NULL);
	}
	file->name = ft_expand(ft_strdup(*name));
	if (!file->name)
	{
		free(*name);
		write(2, "malloc failed while listing files\n", 34);
		return (NULL);
	}
	if (type == REDIR_IN || type == HERE_DOC)
		file->type = OPEN;
	else if (type == REDIR_OUT)
		file->type = CREATE;
	else if (type == APPEND)
		file->type = ADD_TO;
	file->next = NULL;
	file->prev = NULL;
	return (file);
}

void	ft_add_file_to_list(t_command_table *table, t_file *temp)
{
	t_file	*file;

	file = NULL;
	if (table->files == NULL)
		table->files = temp;
	else
	{
		file = table->files;
		while (file->next != NULL)
			file = file->next;
		file->next = temp;
		temp->prev = file;
	}
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

int	ft_handle_redir(t_command *cmd, t_lex **token, t_command_table *table)
{
	t_file	*temp;

	temp = NULL;
	if (!ft_redir_syntax_check(token, &cmd, &table))
		return (0);
	if ((*token)->type == HERE_DOC)
	{
		if (!ft_parse_heredoc(cmd, token, table))
		{
			ft_free_all(*token, cmd, table);
			return (0);
		}
		else
			return (1);
	}
	temp = ft_new_file(temp, &(*token)->next->str, (*token)->type);
	if (!temp)
	{
		ft_free_all(*token, cmd, table);
		return (0);
	}
	cmd->redir_out_file = temp;
	ft_add_file_to_list(table, temp);
	*token = ft_remove_redirection(cmd, *token);
	return (1);
}

int	ft_parse_heredoc(t_command *cmd, t_lex **token, t_command_table *table)
{
	t_file		*temp;
	char		*name;

	temp = NULL;
	name = ft_heredoc_name();
	if (!name)
	{
		write(2, "malloc failed while creating heredoc\n", 37); 
		return (0);
	}
	temp = ft_new_file(temp, &name, (*token)->type);
	if (!temp)
		return (0);
	if (!ft_create_heredoc((*token)->next->str, name))
	{
		free(name);
		free(temp->name);
		free(temp);
		return (0);
	}
	cmd->redir_in_file = temp;
	ft_add_file_to_list(table, temp);
	*token = ft_remove_redirection(cmd, *token);
	free(name);
	return (1);
}
