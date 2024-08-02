/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:58:52 by nvallin           #+#    #+#             */
/*   Updated: 2024/02/05 14:28:16 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	int		i;
	int		i2;
	char	*new;

	i = 0;
	i2 = 0;
	if (s1 == NULL || s2 == NULL)
		return (0);
	while (s1[i] != '\0')
		i++;
	while (s2[i2] != '\0')
		i2++;
	new = malloc(i + i2 + 1);
	if (!new)
		return (0);
	i = 0;
	i2 = 0;
	while (s1[i2] != '\0')
		new[i++] = s1[i2++];
	i2 = 0;
	while (s2[i2] != '\0')
		new[i++] = s2[i2++];
	new[i] = '\0';
	return (new);
}
