/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 01:34:25 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/19 01:34:25 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_replace_env_value(char ***envp, char *new)
{
	int	i;

	i = 0;
	while (envp[0][i] && ft_envcmp(envp[0][i], new))
		i++;
	if (envp[0][i])
	{
		free(envp[0][i]);
		envp[0][i] = ft_strdup(new);
		if (!envp[0][i])
		{
			while (envp[0][++i])
				free(envp[0][i]);
			ft_free_array(envp[0]);
			write(2, "malloc failed while replacing env value\n", 40);
			return (1);
		}
	}
	else
	{
		if (!ft_array_append(&envp[0], new))
			return (1);
	}
	return (0);
}

int	ft_is_env_quoted(char *str, int start)
{
	int	i;
	int	d_quoted;
	int	s_quoted;

	i = 0;
	d_quoted = 0;
	s_quoted = 0;
	while (str[i] && i < start)
	{
		if (str[i] == '"' && d_quoted == 0 && s_quoted == 0)
			d_quoted = 1;
		else if (str[i] == '"' && d_quoted == 1)
			d_quoted = 0;
		if (str[i] == '\'' && s_quoted == 0 && d_quoted == 0)
			s_quoted = 1;
		else if (str[i] == '\'' && s_quoted == 1)
			s_quoted = 0;
		i++;
	}
	if (d_quoted || s_quoted)
		return (1);
	return (0);
}

int	ft_find_env_start(char *str, int i)
{
	static int	quoted = 0;

	while (str[i] != '\0' && str[i] != '$')
	{
		if (str[i] == '"' && quoted == 0)
			quoted = 1;
		else if (str[i] == '"' && quoted == 1)
			quoted = 0;
		if (str[i] == '\'' && quoted == 0)
			while (str[++i] != '\'' && str[i] != '\0')
				continue ;
		if (str[i] == '\0')
			break ;
		i++;
	}
	return (i);
}

int	ft_find_env_end(char *str, int start)
{
	int	end;

	end = start + 1;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	return (end);
}
