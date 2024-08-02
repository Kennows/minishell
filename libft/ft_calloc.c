/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:05:15 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/11 11:11:03 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char		*result;
	size_t		mem;
	size_t		i;

	mem = count * size;
	if (size != 0 && count != mem / size)
		return (0);
	if (count != 0 && size != mem / count)
		return (0);
	result = malloc(mem);
	if (result == (void *)0)
		return (0);
	i = 0;
	while (i < mem)
	{
		result[i] = '\0';
		i++;
	}
	return (result);
}
