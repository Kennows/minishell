/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 18:53:06 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/01 22:58:56 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_create_heredoc(char *delimiter, char *filename)
{
	int		fd;
	char	*buf;
	char	*str;
	size_t	len;

	str = NULL;
	fd = open(filename, O_RDWR | O_CREAT, 0666);
	while (1)
	{
		buf = readline(">");
		if (ft_strncmp(buf, delimiter, ft_strlen(delimiter)))
		{
			if (!str)
					str = ft_strdup(buf);
			else
					str = ft_strjoin(str, buf);
			free(buf);
		}
		else
		{
			len = ft_strlen(str);
			write(fd, str, len);
			close(fd);
			return ;
		}
	}
}
