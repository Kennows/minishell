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
	free(num);
	if (!ret)
		return (NULL);
	i++;
	return (ret);
}

char	*ft_strappend(char *dest, char *src)
{
	char 	*new;
	int	i;

	if (!src)
		return (dest);
	if (dest == NULL)
	{
		dest = ft_strdup(src);
		if (!dest)
			return (NULL);
		return (dest);
	}
	new = malloc(sizeof(char) * (ft_strlen(dest) + ft_strlen(src) + 1));
	if (!new)
	{
		free(dest);
		return (NULL);
	}
	i = ft_strlen(dest);
	ft_strlcpy(new, dest, i);
	i -= 1;
	while (src[++i - ft_strlen(dest)] != '\0')
		new[i] = src[i - ft_strlen(dest)];
	free(dest);
	return (new);
}

int	ft_write_in_heredoc(int fd, char *delimiter)
{
	char	*buf;
	char	*str;

	str = NULL;
	buf = NULL;
	while (1)
	{
		buf = readline(">");
		if (ft_strncmp(buf, delimiter, ft_strlen(delimiter)))
		{
			str = ft_strappend(str, buf);
			free(buf);
			if (!str)
				return (0);
		}
		else
		{
			if (str)
			{
				write(fd, str, ft_strlen(str));
				free(str);
			}
			free(buf);
			return (1);
		}
	}
}

int	ft_create_heredoc(char *delimiter, char *filename)
{
	int		fd;

	fd = open(filename, O_RDWR | O_CREAT, 0666);
	if (fd == -1)
	{
		write(2, "error creating heredoc\n", 23);
		return (0);
	}
	if (!ft_write_in_heredoc(fd, delimiter))
	{
		write(2, "error while writing in heredoc\n", 25);
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}
