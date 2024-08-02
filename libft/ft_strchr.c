/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:22:27 by nvallin           #+#    #+#             */
/*   Updated: 2023/11/09 13:12:31 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	char	*search;

	search = (char *)s;
	while (*search != (char)c && *search != '\0')
		search++;
	if (*search == (char)c)
		return (search);
	return (0);
}
