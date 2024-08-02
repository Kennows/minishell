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

char	*ft_heredoc_name(void)
{
	static int	i = 1;
	char		*ret;
	char		*num;

	if (i == 1)
	{
		ret = ft_strdup(".heredoc");
		if (!ret)
			return (NULL);
		i++;
		return (ret);
	}
	num = ft_itoa(i);
	if (!num)
		return (NULL);
	ret = ft_strjoin(".heredoc", num);
	if (!ret)
		return (NULL);
	free(num);
	i++;
	return (ret);
}

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
			{
				str = ft_strdup(buf);
				if (!str)
					write(2, "eror\n", 5);
			}
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
