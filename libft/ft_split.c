/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 10:24:11 by nvallin           #+#    #+#             */
/*   Updated: 2024/07/17 17:31:57 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_allocate(char const *s, char c)
{
	char		**new;
	int			strings;
	int			i;

	strings = 1;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
			strings++;
		while (s[i] != c && s[i] != '\0')
			i++;
		while (s[i] == c && s[i] != '\0')
			i++;
	}
	if (strings == 1)
		return (NULL);
	new = (char **)malloc(strings * sizeof(char *));
	if (!new)
		return (NULL);
	return (new);
}

static char	*ft_mallocpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	if (src == NULL)
		return (NULL);
	i = 0;
	dst = (char *)malloc(n * sizeof(char));
	if (!dst)
		return (NULL);
	while (i + 1 < n)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static void	ft_free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
		free(arr[i++]);
	free(arr);
}

static char	**ft_help_split(char **dst, char const *s, char c)
{
	char const	*end;
	int			i;

	i = 0;
	while (*s != '\0')
	{
		while (*s == c)
			s++;
		end = s;
		while (*end != c && *end != '\0')
			end++;
		if (*s != '\0')
		{
			dst[i] = ft_mallocpy(dst[i], s, end - s + 1);
			if (!dst[i++])
			{
				ft_free_array(dst);
				return (NULL);
			}
		}
		s = end;
	}
	dst[i] = NULL;
	return (dst);
}

char	**ft_split(char const *s, char c)
{
	char	**new;

	if (!s)
		return (NULL);
	new = ft_allocate(s, c);
	if (!new)
		return (NULL);
	new = ft_help_split(new, s, c);
	if (!new)
		return (NULL);
	return (new);
}
