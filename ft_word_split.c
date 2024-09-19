/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:49:34 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/29 15:28:50 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		while (s[i] != '\0' && (!ft_is_whitespace(s[i])))
			i++;
		while (ft_is_whitespace(s[i]) && s[i] != '\0')
			i++;
	}
	new = (char **)malloc(strings * sizeof(char *));
	if (!new)
	{
		write(2, "malloc failed while word splitting\n", 35);
		return (NULL);
	}
	return (new);
}

int	ft_help_split(char ***dst, char const *s)
{
	char const	*end;
	int			i;

	i = 0;
	while (*s != '\0')
	{
		while (ft_is_whitespace(*s))
			s++;
		end = s;
		while (*end != '\0' && !ft_is_whitespace(*end))
			end++;
		if (*s != '\0')
		{
			dst[0][i] = malloc(((end - s) + 1) * sizeof(char));
			if (!dst[0][i])
			{
				write(2, "malloc failed while word splitting\n", 35);
				return (0);
			}
			ft_strlcpy(dst[0][i++], s, (end - s) + 1);
		}
		s = end;
	}
	dst[0][i] = NULL;
	return (1);
}

int	ft_word_split(char ***dest, char const *s)
{
	char	**new;

	if (!s)
		return (0);
	new = ft_allocate(s);
	if (!new)
		return (0);
	if (!ft_help_split(&new, s))
	{
		ft_free_array(new);
		return (0);
	}
	*dest = new;
	return (1);
}
