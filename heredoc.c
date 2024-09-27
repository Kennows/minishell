/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 18:53:06 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/26 11:40:11 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_heredoc_name(void)
{
	static int	i = 1;
	char		*ret;
	char		*num;

	ret = NULL;
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

int	ft_readline_heredoc(char **str, char *delimiter, int quoted, \
		t_command_table *table)
{
	char	*buf;

	g_sig = 0;
	while (1)
	{
		buf = readline(">");
		if (g_sig == SIGINT)
			return (130);
		if (buf && ft_strncmp(buf, delimiter, ft_strlen(delimiter) + 1))
		{
			if (!quoted && !ft_handle_env_heredoc(&buf, 0, table))
				return (1);
			*str = ft_strappend(*str, buf);
			free(buf);
			if (!*str)
				return (1);
			continue ;
		}
		if (buf)
			free(buf);
		else
			ft_print_heredoc_warning(delimiter);
		return (0);
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
	else if (!ft_handle_heredoc_quotes(&*delim, 0))
		return (0);
	table->exit_status = ft_readline_heredoc(&str, *delim, quoted, table);
	if (table->exit_status == 130)
		dup2(table->saved_stdin, STDIN_FILENO);	
	else if (table->exit_status)
		return (0);
	if (str)
	{
		printf("");
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
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

int	ft_parse_heredoc(t_command *cmd, t_lex **token, t_command_table *table)
{
	t_file		*temp;
	char		*name;

	temp = NULL;
	name = ft_heredoc_name();
	if (!name)
	{
		write(2, "malloc failed while creating heredoc\n", 37);
		return (0);
	}
	temp = ft_new_heredoc_file(&temp, &name);
	if (!temp)
		return (0);
	if (!ft_create_heredoc(&(*token)->next->str, name, table))
	{
		free(name);
		free(temp->name);
		free(temp);
		return (0);
	}
	ft_handle_redir_helper(cmd, token, temp);
	ft_add_file_to_list(table, temp);
	*token = ft_remove_redirection(cmd, &*token);
	free(name);
	return (1);
}
