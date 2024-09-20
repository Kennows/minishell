#include "minishell.h"

int g_sig;

void	ft_sighandler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_set_sig_handler(void)
{
	struct sigaction	act;
	struct sigaction	sa;

	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	act.sa_handler = ft_sighandler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGINT, &act, NULL);
}

void	ft_heredoc_sighandler(int signal)
{
	if (signal == SIGINT)
	{
		g_sig = SIGINT;
		ioctl(0, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	ft_set_heredoc_sig_handler(void)
{
	struct sigaction	act;

	act.sa_handler = ft_heredoc_sighandler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
}
