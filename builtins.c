/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:34:11 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/07 20:28:41 by nvallin          ###   ########.fr       */
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
			if (env[i][c])
			{
				write(1, "=\"", 2);
				while (env[i][c])
					write(1, &env[i][++c], 1);
				write(1, "\"", 1);
				c = 0;
			}
			write(1, "\n", 1);
		}
		else if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
	}
}

int	ft_export(t_command *cmd)
{
	int	i;

	i = 1;
	if (cmd->argc == 1)
		ft_env(*cmd->envp, 1);
	else
	{
		while (cmd->argv[i])
		{
			*cmd->envp = ft_array_append(*cmd->envp, cmd->argv[i]);
			if (!*cmd->envp)
				return (1);
			i++;
		}
	}
	return (0);
}

int	ft_unset(t_command *cmd)
{
	char	**dup;
	int	len;
	int	i;

	len = 0;
	i = -1;
	while (cmd->envp && cmd->envp[0][len] != NULL)
		len++;
	dup = malloc(sizeof (char *) * len);
	if (!dup)
		return (1);
	dup = ft_array_cpynfree(dup, &**cmd->envp, cmd->argv[1]);
	if (!dup)
		return (1);
	*cmd->envp = dup;
	return (0);
}


int	ft_builtin(t_command *cmd)
{
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "env", 4))
		ft_env(*cmd->envp, 0);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "export", 7))
		ft_export(cmd);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "unset", 6))
		ft_unset(&*cmd);
	return (0);
}
