/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 14:56:25 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/10 14:56:41 by nvallin          ###   ########.fr       */
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

char	**ft_envpdup(char **envp)
{
	char	**dup;
	int	i;
	int	i2;

	i = 0;
	i2 = 0;
	dup = malloc(sizeof (char *) * (ft_arrlen(envp) + 1));
	if (!dup)
		return (NULL);
	while (i < ft_arrlen(envp))
	{
		if (ft_strchr(envp[i], '=') && envp[i][0] != '_')
		{
			dup[i2] = ft_strdup(envp[i]);
			if (!dup[i2++])
			{
					ft_free_array(dup);
				return (NULL);
			}
		}
		i++;
	}
	dup[i2] = NULL;
	return (dup);
}

int	ft_update_pwd(t_command *cmd, char **oldpwd)
{
	char	*cwd;
	char	*pwd;
	int		result;
	
	cwd = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!pwd)
	{
		free(*oldpwd);
		return (1);
	}
	result = ft_replace_env_value(&*cmd->envp, pwd);
	free(pwd);
	if (result == 0)
		result = ft_replace_env_value(&*cmd->envp, *oldpwd);
	free(*oldpwd);
	if (result != 0)
		return (1);
	return (0);
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
			return (1);
		}
	}
	else
	{
		envp[0] = ft_array_append(&*envp[0], new);
		if (!envp[0])
			return (1);
	}
	return (0);
}