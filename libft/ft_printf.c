/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:53:38 by nvallin           #+#    #+#             */
/*   Updated: 2024/01/04 16:20:09 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_vspecifier(const char c, va_list a)
{
	int		ret;

	if (c == 'c')
		ret = ft_iputchar(va_arg(a, int));
	else if (c == 's')
		ret = ft_iputstr(va_arg(a, char *));
	else if (c == 'p')
		ret = ft_iputpointer(va_arg(a, unsigned long));
	else if (c == 'd' || c == 'i')
		ret = ft_iputnbr(va_arg(a, int));
	else if (c == 'u')
		ret = ft_iputunsigned(va_arg(a, unsigned int));
	else if (c == 'x')
		ret = ft_iputhexlow(va_arg(a, unsigned int));
	else if (c == 'X')
		ret = ft_iputhexup(va_arg(a, unsigned int));
	else if (c == '%')
		ret = write(1, "%", 1);
	else
		ret = -1;
	return (ret);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		ret;
	int		temp;

	va_start(args, format);
	ret = 0;
	while (*format != '\0')
	{
		temp = 0;
		if (*format == '%' && *++format != '\0')
			temp = ft_vspecifier(*format++, args);
		else if (*format != '\0')
			temp = write(1, &*format++, 1);
		if (temp == -1)
		{
			va_end(args);
			return (-1);
		}
		ret += temp;
	}
	va_end(args);
	return (ret);
}
