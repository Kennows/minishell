/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:56:25 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:40:09 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_getnenv(char *str, char **env, int n, t_command_table *table)
{
	char	*temp;

	temp = NULL;
	if (*str == '?')
		*env = ft_itoa(table->exit_status);
	else
	{
		temp = malloc(sizeof(char) * n);
		if (!temp)
			return (0);
		ft_strlcpy(temp, str, n);
		if (ft_getenv(temp, table->envp))
		{
			*env = ft_strdup(ft_getenv(temp, table->envp));
			if (!*env)
			{
				write(2, "ft_strdup failed in ft_getenv\n", 30);
				free(temp);
				return (0);	
			}
		}
		free(temp);
	}	
	return (1);
}

int	ft_envlen(char *env)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

char	**ft_envpdup(char **envp)
{
	char	**dup;
	int		i;
	int		i2;

	i = -1;
	i2 = 0;
	dup = malloc(sizeof (char *) * (ft_arrlen(envp) + 1));
	if (!dup)
		return (NULL);
	while (++i < ft_arrlen(envp))
	{
		if (ft_strchr(envp[i], '='))
		{
			if (ft_envlen(envp[i]) == 1 && envp[i][0] == '_')
				continue ;
			dup[i2] = ft_strdup(envp[i]);
			if (!dup[i2++])
			{
				ft_free_array(dup);
				return (NULL);
			}
		}
	}
	dup[i2] = NULL;
	return (dup);
}

int	ft_envcmp(char *env, char *new)
{
	int	i;

	i = 0;
	while (env[i] && env[i] != '=' && env[i] == new[i])
		i++;
	if ((env[i] == '\0' && new[i] == '=') || (env[i] == new[i]) \
		|| (env[i] == '=' && new[i] == '\0'))
		return (0);
	else
		return (env[i] - new[i]);
}
