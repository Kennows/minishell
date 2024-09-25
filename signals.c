#include "minishell.h"

int g_sig;

void	ft_sighandler(int signal)
{
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	ft_sighandler_non_interactive(int signal)
{
	if (signal == SIGINT)
	{
		g_sig = SIGINT;
		ioctl(0, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	ft_set_sig_handler(int interactive)
{
	struct sigaction	act;
	struct sigaction	sa;

	if (interactive == 1)
	{

		act.sa_handler = ft_sighandler;
		act.sa_flags = 0;
		sigemptyset(&act.sa_mask);
		sigaction(SIGINT, &act, NULL);
	}
	else
	{
		act.sa_handler = ft_sighandler_non_interactive;
		act.sa_flags = 0;
		sigemptyset(&act.sa_mask);
		sigaction(SIGINT, &act, NULL);		
	}
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGQUIT, &sa, NULL);
}

void	ft_ignore_signals(void)
{
	struct sigaction act;

	act.sa_handler = SIG_IGN;  // Ignore signals
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	// Ignore SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\)
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}