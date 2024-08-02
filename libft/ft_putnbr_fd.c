/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:09:09 by nvallin           #+#    #+#             */
/*   Updated: 2024/01/04 16:19:08 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*static int	ft_nbrlen(int n)
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
*/
void	ft_putnbr_fd(int n, int fd)
{
	int			d;
	int			len;
	char		str[12];

	len = ft_nbrlen(n);
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
	ft_putstr_fd(str, fd);
}
