/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 16:23:21 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/11 11:34:48 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	chr;
	size_t	dlen;
	size_t	slen;

	chr = 0;
	dlen = 0;
	slen = 0;
	while (dst != (void *)0 && dst[dlen] != '\0' && dlen < dstsize)
		dlen++;
	while (src != ((void *)0) && src[slen] != '\0')
		slen++;
	i = dlen;
	while (chr < (dstsize - dlen - 1) && src[chr] != '\0' && dlen < dstsize)
	{
		dst[i] = src[chr];
		i++;
		chr++;
	}
	if (dstsize != 0 && dlen < dstsize)
		dst[i] = '\0';
	if (dstsize <= dlen)
		return (dstsize + slen);
	return (dlen + slen);
}
