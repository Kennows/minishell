/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:18:29 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:46:39 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_arrlen(char **arr)
{
	int	len;

	len = 0;
	while (arr && arr[len] != NULL)
		len++;
	return (len);
}

int	ft_strcombine(char **dest, char *src)
{
	int		i;
	int		i2;
	char	*new;

	if (src == NULL || *dest == NULL)
		return (0);
	i = ft_strlen(*dest);
	i2 = ft_strlen(src);
	new = malloc((sizeof(char) * (i + i2 + 1)));
	if (!new)
		return (0);
	ft_strlcpy(new, *dest, i + 1);
	i2 = 0;
	while (src[i2] != '\0')
		new[i++] = src[i2++];
	new[i] = '\0';
	free(*dest);
	*dest = new;
	return (1);
}

int	ft_find_quote_end_index(const char *s, char q, int start)
{
	while (s[start] != q && s[start] != '\0')
		start++;
	if (s[start] == q)
	{
		start++;
		while (s[start] != q && s[start] != '\0')
			start++;
	}
	return (start);
}

char	*ft_strldup(const char *s1, size_t len)
{
	size_t	i;
	char	*dup;

	i = 0;
	dup = malloc(len + 1 * sizeof(char));
	if (!dup)
		return (NULL);
	while (i < len && s1[i] != '\0')
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
