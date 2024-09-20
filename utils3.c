/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:51:09 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:51:50 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_redir_env_error(char *str)
{
	int	i;

	i = ft_find_env_start(str, 0);
	write(2, "minishell: ", 11);
	while (str[i])
		write(2, &str[i++], 1);
	write(2, ": ambiguous redirect\n", 21);
}

void	ft_print_heredoc_warning(char *delimiter)
{
	int	i;

	i = 0;
	write(2, "minishell: warning: here-document delimited by end-of-file ", 59);
	write(2, "(wanted `", 9);
	while (delimiter[i])
		write(2, &delimiter[i++], 1);
	write(2, "')\n", 3);
}
