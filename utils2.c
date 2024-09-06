/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:18:29 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/06 11:20:38 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

char	**ft_strarrdup(char **arr)
{
	char	**dup;
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (arr && arr[len])
		len++;
	printf("env len %d\n", len);
	dup = malloc(sizeof (char *) * (len + 1));
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = ft_strdup(arr[i]);
		if (!dup[i])
		{
			while (i >= 0)
				free(dup[i--]);
			return (NULL);
		}
		i++;
	}
	printf("i = %d\n", i);
	dup[i] = NULL;
	return (dup);
}

char	*ft_getenv(char *str, char **envp)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(str);
	while (envp[i])
	{
		if (ft_strncmp(str, envp[i], len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
