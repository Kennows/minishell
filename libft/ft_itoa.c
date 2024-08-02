/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 10:27:25 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/11 11:15:40 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intlen(int n)
{
	int		d;
	int		len;

	d = n;
	len = 0;
	if (n == 0)
		len = 1;
	while (d != 0)
	{
		len++;
		d = d / 10;
	}
	if (n < 0)
		len++;
	return (len);
}

char	*ft_itoa(int n)
{
	int			d;
	int			len;
	char		*str;

	len = ft_intlen(n);
	str = malloc((len + 1) * sizeof(char));
	if (!str)
		return (0);
	d = n;
	str[len--] = '\0';
	while (len != 0)
	{
		if (n < 0)
			str[len--] = '0' - d % 10;
		else
			str[len--] = d % 10 + '0';
		d = d / 10;
	}
	if (n < 0)
		str[len] = '-';
	else
		str[len] = d % 10 + '0';
	return (str);
}
