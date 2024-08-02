/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:32:32 by nvallin           #+#    #+#             */
/*   Updated: 2024/06/24 14:22:40 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;
	size_t	s_len;
	size_t	size;

	s_len = 0;
	i = 0;
	if (s == NULL)
		return (0);
	while (s[s_len] != '\0')
		s_len++;
	if (len > s_len - start)
		size = s_len - start;
	else
		size = len;
	if (start > s_len)
		size = 0;
	sub = malloc(size + 1);
	if (!sub)
		return (0);
	while (i < size && start < s_len && s[start] != '\0')
		sub[i++] = s[start++];
	sub[i] = '\0';
	return (sub);
}
