/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:36:45 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/28 19:11:34 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_getnenv(char *str, char **env, int n, char **envp)
{
	char	*temp;

	temp = malloc(sizeof(char) * n);
	if (!temp)
		return (0);
	ft_strlcpy(temp, str, n);
	if (ft_getenv(temp, envp))
	{
		*env = ft_strdup(ft_getenv(temp, envp));
		if (!*env)
		{
			free(temp);
			return (0);
		}
	}
	free(temp);
	return (1);
}

char	*ft_replace_var(char *str, int start, int end, char **envp)
{
	char	*env;
	char	*new;

	env = NULL;
	if (end - start > 1)
	{
		if (!ft_getnenv(str + start + 1, &env, end - start, envp))
		{
			free(str);
			return (NULL);
		}
		str = ft_str_replace(str, env, start, end);
		if (!str)
			return (NULL);
		end = (start + ft_strlen(env));
		free(env);
	}
	if (str[end] == '\0')
		return (str);
	new = ft_handle_env(str, end, envp);
	free(str);
	return (new);
}

int	ft_find_env_start(char *str, int i)
{
	static int	quoted = 0;

	while (str[i] && str[i] != '$')
	{
		if (str[i] == '"' && quoted == 0)
			quoted = 1;
		else if (str[i] == '"' && quoted == 1)
			quoted = 0;
		if (str[i] == '\'' && quoted == 0)
			while (str[++i] != '\'')
				continue ;
		i++;
	}
	return (i);
}

char	*ft_handle_env(char *str, int start, char **envp)
{
	int		end;
	char	*new;

	end = 0;
	new = ft_strdup(str);
	if (!new)
		return (NULL);
	start = ft_find_env_start(new, start);
	if (new[start] == '$')
	{
		end = start + 1;
		while (ft_isalnum(new[end]))
			end++;
		new = ft_replace_var(new, start, end, envp);
		if (!new)
			write(2, "malloc failed during variable expansion\n", 40);
	}
	return (new);
}

char	*ft_expand(char *str, char **envp)
{
	char	*new;

	new = NULL;
	if (str)
	{
		new = ft_handle_env(str, 0, envp);
		if (!new)
			return (NULL);
		new = ft_handle_quotes(new, 0);
		if (!new)
			return (NULL);
		return (new);
	}
	return (NULL);
}
