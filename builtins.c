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
			if (ft_replace_env_value(&*cmd->envp, cmd->argv[i]))
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

	len = 0;
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

void	ft_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	printf("%s\n", cwd);
	free(cwd);
}

int	ft_cd(t_command *cmd)
{
	char	*cwd;
	char	*oldpwd;

	if (cmd->argc > 2)
		write(2, "minishell: cd: too many arguments\n", 34);
	else if (cmd->argc == 2)
	{
		cwd = getcwd(NULL, 0);
		oldpwd = ft_strjoin("OLDPWD=", cwd);
		free(cwd);
		if (!oldpwd)
			return (1);
		if (chdir(cmd->argv[1]))
		{
			free(oldpwd);
			perror("minishell: cd");
			return (-1);
		}
		if (ft_update_pwd(cmd, &oldpwd))
		{
			return (1);
		}
	}
	return (0);
}

int	ft_builtin(t_command *cmd, t_command_table *table)
{
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "env", 4))
		ft_env(*cmd->envp, 0);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "export", 7))
		ft_export(cmd);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "unset", 6))
		ft_unset(&*cmd);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "pwd", 4))
		ft_pwd();
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "cd", 3))
		ft_cd(cmd);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "echo", 5))
		ft_echo(cmd);
	if (cmd->argv && !ft_strncmp(cmd->argv[0], "exit", 5))
		return (ft_exit(table));
	return (0);
}