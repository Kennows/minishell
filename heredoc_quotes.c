/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:42:13 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/28 19:01:36 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_remove_heredoc_quotes(char *str, char *new, char q, int *start)
{
	int	i;
	int	quotes;

	quotes = 0;
	i = -1;
	while (++i < *start)
		new[i] = str[i];
	while (str[i + quotes])
	{
		while (str[i + quotes] == q && quotes < 2)
		{
			quotes++;
			if (quotes == 2)
				*start = i;
		}
		if (str[i + quotes])
		{
			new[i] = str[i + quotes];
			i++;
		}
	}
	new[i] = '\0';
	return (new);
}

char	*ft_handle_heredoc_quotes(char *str, int start)
{
	char	*new;

	while (str[start] && str[start] != '"' && str[start] != '\'')
		start++;
	if (str[start] == '\0')
	{
		new = ft_strdup(str);
		free(str);
		return (new);
	}
	new = malloc(sizeof(char) * (ft_strlen(str) - 1));
	if (!new)
	{
		free(str);
		write(2, "malloc failed while removing quotes\n", 36);
		return (NULL);
	}
	if (str[start] == '"')
		new = ft_remove_heredoc_quotes(str, new, '"', &start);
	else if (str[start] == '\'')
		new = ft_remove_heredoc_quotes(str, new, '\'', &start);
	if (new[start] != '\0')
		new = ft_handle_heredoc_quotes(new, start);
	free(str);
	return (new);
}
