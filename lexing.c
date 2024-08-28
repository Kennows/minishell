/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:57:08 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/28 19:27:40 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_token_type(t_lex *token)
{
	if (!ft_strncmp(token->str, "<", 2))
		token->type = REDIR_IN;
	else if (!ft_strncmp(token->str, ">", 2))
		token->type = REDIR_OUT;
	else if (!ft_strncmp(token->str, "<<", 3))
		token->type = HERE_DOC;
	else if (!ft_strncmp(token->str, ">>", 3))
		token->type = APPEND;
	else if (!ft_strncmp(token->str, "|", 2))
		token->type = PIPE;
	else if (!ft_strncmp(token->str, "echo", 5) || \
			!ft_strncmp(token->str, "cd", 3) || \
			!ft_strncmp(token->str, "pwd", 4) || \
			!ft_strncmp(token->str, "export", 7) || \
			!ft_strncmp(token->str, "unset", 6) || \
			!ft_strncmp(token->str, "env", 4) || \
			!ft_strncmp(token->str, "exit", 5))
		token->type = BUILT_IN;
	else if (ft_isnumber(token->str))
		token->type = NUMBER;
	else
		token->type = WORD;
}

t_lex	*ft_new_token(char *str, int index)
{
	t_lex	*head;

	head = malloc(sizeof(t_lex));
	if (!head)
	{
		write(2, "malloc failed while creating tokens\n", 36);
		return (NULL);
	}
	head->str = ft_strdup(str);
	if (!head->str)
	{
		free(head);
		write(2, "malloc failed while creating tokens\n", 36);
		return (NULL);
	}
	ft_token_type(head);
	head->index = index;
	head->next = NULL;
	head->prev = NULL;
	return (head);
}

t_lex	*ft_add_token(t_lex *tokens, char *str)
{
	t_lex	*current;
	int		i;

	i = 0;
	if (tokens == NULL)
	{
		tokens = ft_new_token(str, i);
		if (!tokens)
			return (NULL);
	}
	else
	{
		current = tokens;
		while (current->next != NULL)
		{
			current = current->next;
			i++;
		}
		current->next = ft_new_token(str, ++i);
		if (!current->next)
			ft_free_tokens(&tokens);
		else
			current->next->prev = current;
	}
	return (tokens);
}

t_lex	*ft_tokenize(char *cmd)
{
	t_lex	*tokens;
	char	**temp;
	int		i;

	tokens = NULL;
	i = 0;
	temp = ft_token_split(cmd, ' ');
	if (!temp)
		return (NULL);
	while (temp[i] != NULL)
	{
		tokens = ft_add_token(tokens, temp[i]);
		free(temp[i++]);
		if (!tokens)
		{
			while (temp[i] != NULL)
				free(temp[i++]);
			free(temp);
			return (NULL);
		}
	}
	free(temp);
	return (tokens);
}
