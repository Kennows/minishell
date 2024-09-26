/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:45:09 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/26 11:46:22 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file	*ft_new_heredoc_file(t_file **file, char **name)
{
	*file = malloc(sizeof(t_file));
	if (!*file)
	{
		write(2, "malloc failed while listing files\n", 34);
		free(*name);
		return (NULL);
	}
	(*file)->name = NULL;
	(*file)->type = HEREDOC;
	(*file)->next = NULL;
	(*file)->prev = NULL;
	(*file)->name = ft_strdup(*name);
	if (!(*file)->name)
	{
		free(*file);
		free(*name);
		write(2, "ft_strdup failed while creating heredoc file\n", 45);
		return (NULL);
	}
	return (*file);
}

t_file	*ft_new_file(t_file *file, t_lex *token, t_token_type type, \
		t_command_table *table)
{
	file = malloc(sizeof(t_file));
	if (!file)
	{
		write(2, "malloc failed while listing files\n", 34);
		return (NULL);
	}
	file->name = ft_expand_file_name(token->str, token, table);
	if (!file->name || file->name[0] == '\0')
	{
		if (file->name)
			free(file->name);
		free(file);
		return (NULL);
	}
	if (type == REDIR_IN)
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

t_lex	*ft_remove_redirection(t_command *cmd, t_lex **token)
{
	if (cmd->token_end == (*token)->next)
		cmd->token_end = (*token)->prev;
	if ((*token)->next->next == NULL)
	{
		if (cmd->token_start == *token)
			cmd->token_start = NULL;
		*token = ft_remove_token(&*token, cmd);
		*token = ft_remove_token(&*token, cmd);
		return (NULL);
	}
	else
	{
		*token = (*token)->next->next;
		if (cmd->token_start && cmd->token_start == (*token)->prev->prev)
		{
			if (cmd->token_end && (*token)->index <= cmd->token_end->index)
				cmd->token_start = *token;
			else
				cmd->token_start = NULL;
		}
		*token = (*token)->prev->prev;
		*token = ft_remove_token(&*token, cmd);
		*token = ft_remove_token(&*token, cmd);
		return (*token);
	}
}

int	ft_handle_redir(t_command *cmd, t_lex **token, t_command_table *table)
{
	t_file	*temp;

	temp = NULL;
	if (!ft_redir_syntax_check(&*token, &cmd, &table))
		return (0);
	if ((*token)->type == HERE_DOC)
	{
		if (!ft_parse_heredoc(cmd, token, table) || table->exit_status)
			return (0);
		else
			return (1);
	}
	temp = ft_new_file(temp, &*(*token)->next, (*token)->type, table);
	if (!temp)
		return (0);
	if ((*token)->type == REDIR_IN)
		cmd->redir_in_file = temp;
	else
		cmd->redir_out_file = temp;
	ft_add_file_to_list(table, temp);
	*token = ft_remove_redirection(cmd, &*token);
	return (1);
}
