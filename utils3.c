/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:51:09 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:51:50 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strprepend(char **str, char *prefix)
{
	int		i;
	int		i2;
	char	*new;

	i = -1;
	i2 = 0;
	new = malloc(sizeof(char) * (ft_strlen(*str) + ft_strlen(prefix) + 1));
	if (!new)
		return (0);
	while (prefix[++i] != '\0')
		new[i] = prefix[i];
	while (str[0][i2] != '\0')
		new[i++] = str[0][i2++];
	new[i] = '\0';
	free(str[0]);
	str[0] = new;
	return (1);
}

int	ft_strchr_index(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char)c && s[i] != '\0')
		i++;
	if (s[i] == (char)c)
		return (i);
	return (-1);
}
