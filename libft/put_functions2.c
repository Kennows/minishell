/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_functions2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 15:51:34 by nvallin           #+#    #+#             */
/*   Updated: 2024/01/04 16:20:28 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_unbrlen(unsigned int n)
{
	unsigned int	nbr;
	int				len;

	nbr = n;
	len = 0;
	if (n == 0)
		len = 1;
	while (nbr != 0)
	{
		len++;
		nbr = nbr / 10;
	}
	return (len);
}

int	ft_iputhexlow(unsigned int n)
{
	int				ret;
	int				i;
	unsigned int	nbr;
	char			hex[9];

	nbr = n;
	i = 7;
	hex[8] = '\0';
	if (nbr == 0)
		hex[i--] = '0';
	while (nbr != 0)
	{
		if (nbr % 16 > 9)
			hex[i--] = nbr % 16 + 87;
		else
			hex[i--] = nbr % 16 + '0';
		nbr /= 16;
	}
	i++;
	ret = ft_iputstr(&hex[i]);
	return (ret);
}

int	ft_iputhexup(unsigned int n)
{
	int				ret;
	int				i;
	unsigned int	nbr;
	char			hex[9];

	nbr = n;
	i = 7;
	hex[8] = '\0';
	if (nbr == 0)
		hex[i--] = '0';
	while (nbr != 0)
	{
		if (nbr % 16 > 9)
			hex[i--] = nbr % 16 + 55;
		else
			hex[i--] = nbr % 16 + '0';
		nbr /= 16;
	}
	i++;
	ret = ft_iputstr(&hex[i]);
	return (ret);
}

int	ft_iputpointer(unsigned long n)
{
	int				ret;
	int				i;
	int				temp;
	char			hex[17];

	ret = ft_iputstr("0x");
	if (ret != 2)
		return (-1);
	i = 15;
	hex[16] = '\0';
	if (n == 0)
		hex[i--] = '0';
	while (n != 0)
	{
		if (n % 16 > 9)
			hex[i--] = n % 16 + 87;
		else
			hex[i--] = n % 16 + '0';
		n /= 16;
	}
	temp = ft_iputstr(&hex[++i]);
	if (temp == -1)
		return (-1);
	ret += temp;
	return (ret);
}
