/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:02:34 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/28 19:27:01 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lex	*ft_remove_token(t_lex *token)
{
	t_lex	*prev;
	t_lex	*next;
	t_lex	*current;

	if (token)
	{
		prev = token->prev;
		next = token->next;
		if (prev != NULL)
			prev->next = next;
		if (next != NULL)
			next->prev = prev;
		current = token;
		while (current->next != NULL)
		{
			current = current->next;
			current->index--;
		}
		if (token->str != NULL)
			free(token->str);
		free(token);
		token = NULL;
		return (next);
	}
	return (NULL);
}

void	ft_free_tokens(t_lex **tokens)
{
	t_lex	*temp;

	if (*tokens)
	{
		while ((*tokens)->prev != NULL)
			*tokens = (*tokens)->prev;
		while ((*tokens)->next != NULL)
		{
			temp = (*tokens)->next;
			if ((*tokens)->str)
				free((*tokens)->str);
			free(*tokens);
			*tokens = temp;
			(*tokens)->prev = NULL;
		}
		if ((*tokens)->str)
			free((*tokens)->str);
		free(*tokens);
	}
	temp = NULL;
	*tokens = NULL;
}	

void	ft_free_commands(t_command *cmd)
{
	int			i;
	t_command	*temp;

	while (cmd != NULL)
	{
		i = 0;
		temp = cmd->next;
		if (cmd->argv)
		{
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		free(cmd);
		cmd = temp;
	}
	temp = NULL;
	cmd = NULL;
}

void	ft_free_files(t_file *files)
{
	t_file	*temp;

	if (files)
	{
		while (files->prev != NULL)
			files = files->prev;
		while (files)
		{
			temp = files->next;
			if (files->name)
				free(files->name);
			free(files);
			files = temp;
		}
	}
}

void	ft_free_all(t_lex *token, t_command *cmd, t_command_table *table)
{
	ft_free_tokens(&token);
	ft_free_commands(cmd);
	ft_free_files(&*table->files);
	free(table);
}
