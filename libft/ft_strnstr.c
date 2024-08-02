/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:02:37 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/11 11:29:03 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	i2;
	char	*result;

	i = 0;
	i2 = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (i < len && haystack[i] != '\0' && needle[i2] != '\0')
	{
		if (haystack[i++] == needle[i2])
		{
			if (i2++ == 0)
				result = (char *)&haystack[i - 1];
		}
		else if (i2 != 0)
		{
			i -= i2;
			i2 = 0;
		}
	}
	if (needle[i2] == '\0')
		return (result);
	return (0);
}
