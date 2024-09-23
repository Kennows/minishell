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

int	ft_handle_heredoc_quotes(char **str, int start)
{
	char	*new;

	while (str[0][start] && str[0][start] != '"' && str[0][start] != '\'')
		start++;
	if (str[0][start] == '\0')
		return (1);
	new = malloc(sizeof(char) * (ft_strlen(str[0]) - 1));
	if (!new)
	{
		free(str[0]);
		str[0] = NULL;
		write(2, "malloc failed while removing quotes\n", 36);
		return (0);
	}
	if (str[0][start] == '"')
		new = ft_remove_heredoc_quotes(str[0], new, '"', &start);
	else if (str[0][start] == '\'')
		new = ft_remove_heredoc_quotes(str[0], new, '\'', &start);
	if (new[start] != '\0')
		if (!ft_handle_heredoc_quotes(&new, start))
			return (0);
	free(str[0]);
	str[0] = new;
	return (1);
}
