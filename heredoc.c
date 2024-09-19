/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 18:53:06 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/28 17:32:45 by nvallin          ###   ########.fr       */
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
	char	*new;
	int		i;

	if (!src)
		return (dest);
	if (dest == NULL)
	{
		dest = ft_strdup(src);
		return (dest);
	}
	new = malloc(sizeof(char) * (ft_strlen(dest) + ft_strlen(src) + 2));
	if (!new)
	{
		free(dest);
		return (NULL);
	}
	i = ft_strlen(dest);
	ft_strlcpy(new, dest, i + 1);
	new[i++] = '\n';
	while (*src != '\0')
		new[i++] = *src++;
	new[i] = '\0';
	free(dest);
	return (new);
}

int	ft_readline_heredoc(char **str, char *delimiter, int quoted, \
		t_command_table *table)
{
	char	*buf;

	while (1)
	{
		buf = readline(">");
		if (ft_strncmp(buf, delimiter, ft_strlen(delimiter) + 1))
		{
			if (!quoted)
			{
				buf = ft_handle_env_heredoc(buf, 0, table->envp);
				if (!buf)
					return (0);
			}
			*str = ft_strappend(*str, buf);
			free(buf);
			if (!*str)
				return (0);
		}
		else
		{
			free(buf);
			return (1);
		}
	}
}

int	ft_write_in_heredoc(int fd, char **delim, t_command_table *table)
{
	char	*str;
	int		quoted;

	quoted = 1;
	str = NULL;
	if (!ft_strchr(*delim, '"') && !ft_strchr(*delim, '\''))
		quoted = 0;
	else
	{
		*delim = ft_handle_heredoc_quotes(&**delim, 0);
		if (!*delim)
			return (0);
	}
	if (!ft_readline_heredoc(&str, *delim, quoted, table))
		return (0);
	if (str)
	{
		write(fd, str, ft_strlen(str));
		free(str);
	}
	return (1);
}

int	ft_create_heredoc(char **delim, char *filename, t_command_table *table)
{
	int	fd;

	fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		write(2, "error creating heredoc\n", 23);
		return (0);
	}
	if (!ft_write_in_heredoc(fd, &*delim, table))
	{
		write(2, "error while writing in heredoc\n", 31);
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}
