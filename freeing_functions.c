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

t_sub_tok	*ft_remove_subtoken(t_sub_tok *subtoken)
{
	t_sub_tok	*next;
	t_sub_tok	*prev;

	if (subtoken != NULL)
	{
		next = subtoken->next;
		prev = subtoken->prev;
		if (prev != NULL)
			prev->next = next;
		if (next != NULL)
			next->prev = prev;
		if (subtoken->str != NULL)
			free(subtoken->str);
		subtoken->str = NULL;
		free(subtoken);
		subtoken = NULL;
		if (next != NULL)
			return (next);
	}
	return (NULL);
}

void	ft_free_tokens(t_lex **tokens)
{
	t_lex	*temp;

	if (tokens && *tokens)
	{
		while ((*tokens)->prev != NULL)
			*tokens = (*tokens)->prev;
		while ((*tokens)->next != NULL)
		{
			temp = (*tokens)->next;
			if ((*tokens)->str)
				free((*tokens)->str);
			ft_remove_subtoken((*tokens)->subtoken);
			free(*tokens);
			*tokens = temp;
			(*tokens)->prev = NULL;
		}
		if ((*tokens)->str)
			free((*tokens)->str);
		ft_remove_subtoken((*tokens)->subtoken);
		free(*tokens);
	}
	temp = NULL;
	*tokens = NULL;
}	

void	ft_free_commands(t_command **cmd)
{
	int			i;
	t_command	*temp;

	if (cmd && *cmd != NULL)
	{
		while ((*cmd)->prev != NULL)
			*cmd = (*cmd)->prev;
		while (cmd && *cmd)
		{
			i = 0;
			temp = (*cmd)->next;
			if ((*cmd)->argv != NULL)
			{
				while ((*cmd)->argv[i] != NULL)
					free((*cmd)->argv[i++]);
				free((*cmd)->argv);
				(*cmd)->argv = NULL;
			}
			free(*cmd);
			*cmd = temp;
		}
		temp = NULL;
		*cmd = NULL;
	}
}

void	ft_free_files(t_file **files)
{
	t_file	*temp;

	if (files && *files != NULL)
	{
		while ((*files)->prev != NULL)
			*files = (*files)->prev;
		while (*files != NULL)
		{
			temp = (*files)->next;
			if ((*files)->type == HEREDOC)
				unlink((*files)->name);
			if ((*files)->name != NULL)
				free((*files)->name);
			free(*files);
			*files = temp;
		}
		*files = NULL;
	}
}

void	ft_free_all(t_lex *token, t_command *cmd, t_command_table *table)
{
	if (token)
		ft_free_tokens(&token);
	if (cmd != NULL)
		ft_free_commands(&cmd);
	if (table->files)
		ft_free_files(&table->files);
}
