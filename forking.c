#include "minishell.h"


char *check_access(t_command *cmd)
{
	char	*path;
	char	**path_arr;
	int	i;
	char	*cmd_end;
	char	*cmd_full;

	path = getenv("PATH");
	if (path == NULL)
		return (NULL);
	path_arr = ft_split(path, ':');
	if (path_arr == NULL)
		return (NULL);
	cmd_end = ft_strjoin("/", cmd->argv[0]);
	if (cmd_end == NULL)
		return (NULL);
	i = 0;
	while (path_arr[i] != NULL)
	{
		cmd_full = ft_strjoin(path_arr[i], cmd_end);
		if (access(cmd_full, F_OK) == 0)
		{
			if (access(cmd_full, X_OK) == 0)
				break ;
		}
		free(cmd_full);
		cmd_full = NULL;
		i++;
	}
	free(cmd_end);
	return (cmd_full);
}

void	execute(t_command *cmd)
{
	extern char	**environ;
	char	*path;
	
	if (cmd->argv[0][0] == '/')
	{
		if (access(cmd->argv[0], F_OK) != 0)
			return ;
		else if (access(cmd->argv[0], X_OK) != 0)
			return ;
		path = cmd->argv[0];
	}
	else
		path = check_access(cmd);
	execve(path, cmd->argv, environ);
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

void	child_process_pipe(int pipe_fd[2], t_command *cmd)
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
	execute(cmd);
}

int	forking(t_command *cmd1, t_command *cmd2)
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
		child_process_pipe(pipe_fd, cmd1);
	pid_id[1] = fork();
	if (pid_id[1] == -1)
		return (-1); // fork error
	if (pid_id[1] == 0)
		child_process_pipe(pipe_fd, cmd2);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid_id[0], &status[0], 0);
	waitpid(pid_id[1], &status[1], 0);
	return (0);
}

int run_commands(t_command *cmd)
{
	pid_t	pid_id;
	int	status;

	if (cmd->pipe_out != NULL)
		forking(cmd, cmd->next);
	else
	{
		pid_id = fork();
		if (pid_id == -1)
			return (-1);
		if (pid_id == 0)
			child_process(cmd);
		waitpid(pid_id, &status, 0);
	}
	return (0);
}
