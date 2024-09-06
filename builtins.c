/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:34:11 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/06 12:32:37 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(char **env, int export)
{
	int	i;

	i = 0;
	if (env)
	{
		while (env[i])
		{
			if (export)
				printf("declare -x %s\n", env[i++]);
			else if (ft_strchr(env[i], '='))
				printf("%s\n", env[i++]);
			else
				i++;
		}
	}
}

int	ft_export(t_command *cmd)
{
	if (cmd->argc == 1)
		ft_env(*cmd->envp, 1);
	return (0);
}

int	ft_builtin(t_command *cmd)
{
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "env", 4))
		ft_env(*cmd->envp, 0);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "export", 7))
		ft_export(cmd);
	return (0);
}
