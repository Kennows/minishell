/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:07:50 by nvallin           #+#    #+#             */
/*   Updated: 2024/01/04 16:19:51 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_iputchar(char c)
{
	int	ret;

	ret = write(1, &c, 1);
	return (ret);
}

int	ft_iputstr(char *str)
{
	int	i;
	int	ret;
	int	temp;

	i = 0;
	ret = 0;
	if (str != 0)
	{
		while (str[i] != '\0')
		{
			temp = ret;
			ret += ft_iputchar(str[i++]);
			if (ret < temp)
				return (-1);
		}
		return (ret);
	}
	return (ft_iputstr("(null)"));
}

int	ft_nbrlen(int n)
{
	int		nbr;
	int		len;

	nbr = n;
	len = 0;
	if (n == 0)
		len = 1;
	while (nbr != 0)
	{
		len++;
		nbr = nbr / 10;
	}
	if (n < 0)
		len++;
	return (len);
}

int	ft_iputnbr(int nbr)
{
	int		ret;
	int		n;
	int		len;
	char	str[12];

	len = ft_nbrlen(nbr);
	n = nbr;
	str[len--] = '\0';
	while (len != 0)
	{
		if (nbr < 0)
			str[len--] = '0' - n % 10;
		else
			str[len--] = n % 10 + '0';
		n = n / 10;
	}
	if (nbr < 0)
		str[len] = '-';
	else
		str[len] = n % 10 + '0';
	ret = ft_iputstr(str);
	return (ret);
}

int	ft_iputunsigned(unsigned int nbr)
{
	int					ret;
	unsigned int		n;
	int					len;
	char				str[11];

	len = ft_unbrlen(nbr);
	n = nbr;
	str[len--] = '\0';
	while (len != 0)
	{
		str[len--] = n % 10 + '0';
		n = n / 10;
	}
	str[len] = n % 10 + '0';
	ret = ft_iputstr(str);
	return (ret);
}
