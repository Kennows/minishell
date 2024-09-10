/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:18:29 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/07 20:21:13 by nvallin          ###   ########.fr       */
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
	while (arr && arr[len])
		len++;
	return (len);
}

char	*ft_strcombine(char *dest, char *src)
{
	int		i;
	int		i2;
	char	*new;

	if (src == NULL)
	{
		if (dest != NULL)
			free(dest);
		return (NULL);
	}
	i = ft_strlen(dest);
	i2 = ft_strlen(src);
	new = malloc((sizeof(char) * (i + i2 + 1)));
	if (!new)
	{
		free(dest);
		return (NULL);
	}
	ft_strlcpy(new, dest, i + 1);
	i2 = 0;
	while (src[i2] != '\0')
		new[i++] = src[i2++];
	new[i] = '\0';
	free(dest);
	return (new);
}