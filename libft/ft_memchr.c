/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 14:57:48 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/11 11:18:01 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	size_t			i;

	if (n > 0)
	{
		i = n;
		str = (unsigned char *)s;
		while (*str != (unsigned char)c)
		{
			i--;
			if (i == 0)
				break ;
			str++;
		}
		if (*str == (unsigned char)c)
			return ((void *)str);
	}
	return (0);
}
