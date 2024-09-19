/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:42:13 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:54:40 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_append_subtoken(t_lex *token, char *str, char q, int len)
{
	t_sub_tok	*new;

	new = malloc(sizeof(t_sub_tok));
	if (!new)
		return (0);
	ft_init_subtoken(new, q);
	new->str = ft_strldup(str, len);
	if (!new->str)
	{
		free(new);
		return (0);
	}
	if (token->subtoken == NULL)
		token->subtoken = new;
	else
	{
		while (token->subtoken->next != NULL)
			token->subtoken = token->subtoken->next;
		token->subtoken->next = new;
		token->subtoken->next->prev = token->subtoken;
		while (token->subtoken->prev != NULL)
			token->subtoken = token->subtoken->prev;
	}
	return (1);
}

int	ft_remove_quotes(char *str, t_lex *token, char q)
{
	int		quotes;
	char	*new;
	int		i;

	quotes = 0;
	i = 0;
	new = malloc(sizeof(char) * (ft_find_quote_end_index(str, q, 0)));
	if (!new)
		return (0);
	while (str[quotes + i] != '\0')
	{
		while (str[quotes + i] == q && quotes < 2)
			quotes++;
		if (quotes == 2)
			break ;
		if (str[quotes + i] != '\0')
			new[i] = str[quotes + i];
		if (str[quotes + i] != '\0')
			i++;
	}
	new[i] = '\0';
	i = ft_append_subtoken(token, new, q, i);
	free(new);
	return (i);
}

int	ft_handle_quotes(char *str, t_lex *token, int start)
{
	while (str[start] != '\0' && str[start] != '"' && str[start] != '\'')
		start++;
	if (start != 0)
	{
		if (!ft_append_subtoken(token, str, 0, start))
		{
			write(2, "error creating subtokens\n", 25);
			return (0);
		}
	}
	if (str[start] == '"' || str[start] == '\'')
	{
		if (!ft_remove_quotes(&str[start], token, str[start]))
		{
			write(2, "error removing quotes\n", 22);
			return (0);
		}
		start = ft_find_quote_end_index(str, str[start], start);
		start++;
	}
	if (str[start] != '\0')
		if (!ft_handle_quotes(&str[start], token, 0))
			return (0);
	return (1);
}
