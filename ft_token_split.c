/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:49:34 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/29 15:28:50 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_find_end(const char *str)
{
	int	i;

	i = 0;
	if ((str[i] == '<' && str[i + 1] == '<') || (str[i] == '>' \
			&& str[i + 1] == '>'))
		return (i + 2);
	if (str[i] == '<' || str[i] == '>' || str[i] == '|')
		return (i + 1);
	while (!ft_is_whitespace(str[i]) && str[i] != '\0' && str[i] != '<' \
			&& str[i] != '>' && str[i] != '|')
	{
		if (str[i] == '"')
			while (str[++i] != '"' && str[i] != '\0')
				continue ;
		else if (str[i] == '\'')
			while (str[++i] != '\'' && str[i] != '\0')
				continue ;
		if (str[i] == '\0')
		{
			write(2, "quotation syntax error\n", 23);
			return (-1);
		}
		i++;
	}
	return (i);
}

static char	**ft_allocate(char const *s)
{
	char		**new;
	int			strings;
	int			i;

	strings = 1;
	i = 0;
	while (s[i] != '\0')
	{
		if (!ft_is_whitespace(s[i]))
			strings++;
		if (ft_find_end(&s[i]) == -1)
			return (NULL);
		i += ft_find_end(&s[i]);
		while (ft_is_whitespace(s[i]) && s[i] != '\0')
			i++;
	}
	if (strings == 1)
		return (NULL);
	new = (char **)malloc(strings * sizeof(char *));
	if (!new)
	{
		write(2, "malloc failed while splitting input\n", 36);
		return (NULL);
	}
	return (new);
}

void	ft_free_array(char **arr)
{
	int	i;

	i = 0;
	if (arr != NULL)
	{
		while (arr[i] != NULL)
		{
			free(arr[i]);
			arr[i++] = NULL;
		}
		free(arr);
		arr = NULL;
	}
}

static char	**ft_help_split(char **dst, char const *s)
{
	char const	*end;
	int			i;

	i = 0;
	while (*s != '\0')
	{
		while (ft_is_whitespace(*s))
			s++;
		end = &s[ft_find_end(s)];
		if (*s != '\0')
		{
			dst[i] = malloc(((end - s) + 1) * sizeof(char));
			if (!dst[i])
			{
				ft_free_array(dst);
				write(2, "malloc failed while splitting input\n", 36);
				return (NULL);
			}
			ft_strlcpy(dst[i++], s, (end - s) + 1);
		}
		s = end;
	}
	dst[i] = NULL;
	return (dst);
}

char	**ft_token_split(char const *s)
{
	char	**new;

	if (!s)
		return (NULL);
	new = ft_allocate(s);
	if (!new)
		return (NULL);
	new = ft_help_split(new, s);
	if (!new)
		return (NULL);
	return (new);
}
