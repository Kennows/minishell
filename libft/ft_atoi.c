/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:17:40 by nvallin           #+#    #+#             */
/*   Updated: 2024/06/15 17:36:51 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	size_t	result;
	int		sign;
	size_t	max;

	result = 0;
	sign = 1;
	max = 0 - 1;
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str > 47 && *str < 58)
	{
		if (result > max / 10 - *str - 48 && sign == -1)
			return (0);
		if (result > max / 10 - *str - 48 && sign == 1)
			return (-1);
		result = result * 10 + *str - 48;
		str++;
	}
	return (result * sign);
}
