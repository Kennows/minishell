#include "minishell.h"

int execute_builtin(t_command *cmd, t_command_table *table, int pipe_in, int pipe_out)
{
    int saved_stdin;
    int saved_stdout;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (pipe_in != -1)
    {
        dup2(pipe_in, STDIN_FILENO);
        close(pipe_in);
    }
    if (pipe_out != -1)
    {
        dup2(pipe_out, STDOUT_FILENO);
        close(pipe_out);
    }
    table->exit_status = set_redirections(cmd);
    if (table->exit_status)
        return (table->exit_status);
    table->exit_status = ft_builtin(cmd, table);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    return (table->exit_status);
}

void execute_command(t_command *cmd, int pipe_in, int pipe_out)
{
    char *cmd_path;

	cmd_path = NULL;
	if (!ft_prepare_path(cmd, &cmd_path))
	{
		perror("minishell");
		exit(1);
	}
    // If this is not the first command in the pipeline, set up pipe for input
    if (pipe_in != -1)
    {
        dup2(pipe_in, STDIN_FILENO);
        close(pipe_in);
    }
    // If this is not the last command in the pipeline, set up pipe for output
    if (pipe_out != -1)
    {
        dup2(pipe_out, STDOUT_FILENO);
        close(pipe_out);
    }
    // Execute the command
    if (set_redirections(cmd))
        exit (1);
    if (execve(cmd_path, cmd->argv, *cmd->envp) == -1)
    {
        perror("Execution error");
        free(cmd_path);
        exit(1);
    }
    free(cmd_path);
}

int fork_and_execute(t_command *cmd, t_command_table *table, \
						int pipe_in, int pipe_out)
{
    pid_t pid;
	
	pid = fork();
    if (pid < 0)
    {
        perror("Fork error");
        return (-1);
    }
    if (pid == 0)
    {
		if (cmd->type == BUILT_IN)
			execute_builtin(cmd, table, pipe_in, pipe_out);
		else
        	execute_command(cmd, pipe_in, pipe_out);
        exit(0); // Ensure child exits after execution
    }
    else
    {
        ft_ignore_signals(); // Ignore signals in parent
        if (pipe_in != -1)
            close(pipe_in); // Close input pipe after use
    }
    return (0);
}

void wait_for_children()
{
    int status;

    while (wait(&status) > 0)
        continue;
}

int run_commands(t_command_table *table)
{
    t_command *cmd;
    int pipe_fd[2];
    int pipe_in;

	pipe_in = -1;  // Initial input is from stdin
    cmd = table->commands;
    while (cmd)
    {
        if (set_pipes(cmd, pipe_fd) == -1)
            return (2);
        if (cmd == table->commands && !cmd->pipe_out && cmd->type == BUILT_IN)
        {
            if (execute_builtin(cmd, table, pipe_in, pipe_fd[1]) == -1)
                return (-1);
        }
		else if (fork_and_execute(cmd, table, pipe_in, pipe_fd[1]) == -1)
			return (2);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
        // The next command's input will be this command's output
        pipe_in = pipe_fd[0];
        cmd = cmd->pipe_out;
    }
    wait_for_children();
    return (0);
}
