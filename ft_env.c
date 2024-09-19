/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 15:01:30 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:45:27 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **env, int export)
{
	int	i;
	int	c;

	i = -1;
	c = 0;
	while (env && env[++i])
	{
		if (export)
		{
			write(1, "declare -x ", 11);
			while (env[i][c] && env[i][c] != '=')
				write(1, &env[i][c++], 1);
			if (env[i][c] != '\0')
			{
				write(1, "=\"", 2);
				while (env[i][c])
					write(1, &env[i][++c], 1);
				write(1, "\"", 1);
			}
			write(1, "\n", 1);
			c = 0;
		}
		else if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
	}
}

int	ft_increment_shlvl(char ***mini_envp)
{
	int		i;
	char	*level;

	i = 0;
	while (mini_envp[0][i] && ft_strncmp("SHLVL=", mini_envp[0][i], 6))
		i++;
	if (!mini_envp[0][i])
	{
		if (ft_replace_env_value(&*mini_envp, "SHLVL=1"))
			return (0);
		return (1);
	}
	level = ft_itoa(ft_atoi(mini_envp[0][i] + 6) + 1);
	if (!level)
		return (0);
	free(mini_envp[0][i]);
	mini_envp[0][i] = ft_strjoin("SHLVL=", level);
	free(level);
	if (!mini_envp[0][i])
	{
		while (mini_envp[0][++i])
			free(mini_envp[0][i]);
		return (0);
	}
	return (1);
}

int	ft_update_pwd(t_command *cmd, char *oldpwd)
{
	char	*pwd;
	int		result;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		write(2, "getcwd failed while updating pwd\n", 33);
		return (1);
	}
	if (!ft_strprepend(&pwd, "PWD="))
	{
		free(pwd);
		write(2, "ft_strprepend failed while updating pwd\n", 40);
		return (1);
	}
	result = ft_replace_env_value(&*cmd->envp, pwd);
	free(pwd);
	if (result == 0)
		result = ft_replace_env_value(&*cmd->envp, oldpwd);
	if (result != 0)
	{
		write(2, "error updating pwd\n", 19);
		return (1);
	}
	return (0);
}

char	**ft_empty_envp(char **mini_envp)
{
	char	*pwd;

	mini_envp = malloc(sizeof (char *) * 4);
	if (!mini_envp)
		return (NULL);
	mini_envp[0] = ft_strdup("OLDPWD");
	if (mini_envp[0])
	{
		pwd = getcwd(NULL, 0);
		mini_envp[1] = ft_strjoin("PWD=", pwd);
		free(pwd);
		if (mini_envp[1])
		{
			mini_envp[2] = ft_strdup("SHLVL=1");
			if (mini_envp[2])
			{
				mini_envp[3] = NULL;
				return (mini_envp);
			}
		}
	}
	ft_free_array(mini_envp);
	return (NULL);
}

char	**ft_create_envp(char **mini_envp, char **envp, char *shell)
{
	if (!envp || !*envp)
	{
		free(shell);
		mini_envp = ft_empty_envp(&*mini_envp);
		if (mini_envp)
			return (mini_envp);
		write(2, "ft_empty_envp failed while creating envp\n", 41);
	}
	else
	{
		mini_envp = ft_envpdup(envp);
		if (mini_envp)
		{
			if (!ft_increment_shlvl(&mini_envp))
				write(2, "error incrementing shlvl env\n", 29);
			else if (!ft_replace_env_value(&mini_envp, &*shell))
			{	
				free(shell);
				return (mini_envp);
			}
			ft_free_array(&*mini_envp);
		}
		free(shell);
	}
	return (NULL);
}
