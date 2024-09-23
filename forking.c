#include "minishell.h"

int	ft_get_path(char ***path_arr, t_command *cmd)
{
	char	*path;
	int		i;

	i = 0;
	path = ft_getenv("PATH", *cmd->envp);
	if (path == NULL)
	{
		if (!ft_array_append(&path_arr[0], "/"))
			return (0);
		return (1);
	}
	path_arr[0] = ft_split(path, ':');
	if (path_arr[0] == NULL)
		return (0);
	while (path_arr[0][i])
	{
		if (!ft_strcombine(&path_arr[0][i], "/"))
		{
			ft_free_array(path_arr[0]);
			return (0);
		}
		i++;
	}
	return (1);	
}

char *check_access(t_command *cmd)
{
	char	**path_arr;
	int		i;
	char	*cmd_full;

	i = 0;
	path_arr = NULL;
	if (!ft_get_path(&path_arr, cmd))
	{
		write(2, "error getting path\n", 19);
		return (NULL);
	}
	while (path_arr[i] != NULL)
	{
		cmd_full = ft_strjoin(path_arr[i], cmd->argv[0]);
		if (access(cmd_full, F_OK) == 0)
		{
			if (access(cmd_full, X_OK) == 0)
				break ;
		}
		free(cmd_full);
		cmd_full = NULL;
		i++;
	}
	ft_free_array(path_arr);
	return (cmd_full);
}

void	execute(t_command *cmd)
{
	char	*path;
	
	if (cmd->argv[0][0] == '/')
	{
		if (access(cmd->argv[0], F_OK) != 0)
			return ;
		else if (access(cmd->argv[0], X_OK) != 0)
			return ;
		path = cmd->argv[0];
		execve(path, cmd->argv, *cmd->envp);
		return ;
	}
	path = check_access(cmd);
	if (path)
	{
		execve(path, cmd->argv, *cmd->envp);
		free(path);
	}
}

void	child_process(t_command *cmd)
{
	int		fd;

	if (cmd->redir_in_file != NULL)
	{
		fd = get_fd(cmd->redir_in_file);
		dup2(fd, 0);
		close(fd);
	}
	if (cmd->redir_out_file != NULL)
	{
		fd = get_fd(cmd->redir_out_file);
		dup2(fd, 1);
		close(fd);
	}
	execute(cmd);
	printf("test\n");
}

void	child_process_pipe(int pipe_fd[2], t_command *cmd, \
							t_command_table *table)
{
	int		fd;

	if (cmd->redir_in_file != NULL)
	{
		fd = get_fd(cmd->redir_in_file);
		dup2(fd, 0);
		close(fd);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[0]);
	}
	if (cmd->redir_out_file != NULL)
	{
		fd = get_fd(cmd->redir_out_file);
		dup2(fd, 1);
		close(fd);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[1]);
	}
	if (cmd->type == BUILT_IN)
		table->exit_status = ft_builtin(cmd, table);
	else
		execute(cmd);
}

int	forking(t_command *cmd1, t_command *cmd2, t_command_table *table)
{
	int	pipe_fd[2];
	pid_t	pid_id[2];
	int	status[2];

	if (pipe(pipe_fd) != 0)
		return (-1); //pipe error
	pid_id[0] = fork();
	if (pid_id[0] == -1)
		return (-1); //fork error
	if (pid_id[0] == 0)
		child_process_pipe(pipe_fd, cmd1, table);
	pid_id[1] = fork();
	if (pid_id[1] == -1)
		return (-1); // fork error
	if (pid_id[1] == 0)
		child_process_pipe(pipe_fd, cmd2, table);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid_id[0], &status[0], 0);
	waitpid(pid_id[1], &status[1], 0);
	return (0);
}

int run_commands(t_command *cmd, t_command_table *table)
{
	pid_t	pid_id;
	int	status;

	if (cmd->pipe_out != NULL)
		forking(cmd, cmd->next, table);
	else if (cmd->type == BUILT_IN)
		return (ft_builtin(cmd, table));
	else
	{
		pid_id = fork();
		if (pid_id == -1)
			return (-1);
		if (pid_id == 0)
			execute(cmd);
		waitpid(pid_id, &status, 0);
	}
	return (0);
}
