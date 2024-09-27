/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:46:28 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/26 11:48:52 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		close(STDIN_FILENO);
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
	struct sigaction	act;

	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}
