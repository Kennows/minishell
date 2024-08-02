/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 16:02:46 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/11 11:32:58 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_trimstart(char const *s1, char const *set)
{
	unsigned int		start;
	unsigned int		i;

	start = 0;
	i = 0;
	while (set[i] != '\0' && s1[start] != '\0')
	{
		if (s1[start] == set[i])
		{
			start++;
			i = 0;
		}
		else
			i++;
	}
	return (start);
}

static unsigned int	ft_trimend(char const *s1, char const *set)
{
	unsigned int	end;
	unsigned int	i;

	end = ft_strlen(s1);
	i = 0;
	while (set[i] != '\0' && end != 0)
	{
		if (s1[end - 1] == set[i])
		{
			end--;
			i = 0;
		}
		else
			i++;
	}
	return (end);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int		start;
	unsigned int		end;
	unsigned int		i;
	char				*new;

	if (s1 == NULL || set == NULL)
		return (0);
	start = ft_trimstart(s1, set);
	end = ft_trimend(s1, set);
	i = 0;
	if (start > end)
		start = end;
	new = malloc(end - start + 1);
	if (!new)
		return (0);
	while (start < end)
		new[i++] = s1[start++];
	new[i] = '\0';
	return (new);
}
