/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:02:34 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/01 17:04:43 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isnumber(const char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	if (ft_isdigit(*str) == 0)
		return (0);
	while (*str != '\0')
	{
		if (ft_isdigit(*str) == 0)
			return (0);
		str++;
	}
	return (1);
}

char	**ft_array_cpynfree(char **dest, char **src)
{
	int	i;

	i = 0;
	if (src != NULL)
	{
		while (src[i] != NULL)
		{
			dest[i] = ft_strdup(src[i]);
			if (!dest[i])
				return (NULL);
			free(src[i]);
			i++;
		}
		free(src[i]);
		free(src);
	}
	return (dest);
}

char	**ft_array_append(char **array, char *str)
{
	int		i;
	char	**new;

	i = 0;
	if (array != NULL)
		while (array[i] != NULL)
			i++;
	new = malloc((i + 2) * sizeof(char *));
	if (!new)
		return (NULL);
	new = ft_array_cpynfree(new, array);
	if (!new)
		return (NULL);
	new[i] = ft_strdup(str);
	if (!new[i])
		return (NULL);
	new[++i] = NULL;
	return (new);
}

void	ft_free_files(t_file *files)
{
	t_file *temp;

	if (files)
	{
		while (files->prev)
			files = files->prev;
		while (files)
		{
			temp = files->next;
			free(files->name);
			free(files);
			files = temp;
		}
		free(files);
	}
}

void	ft_remove_token(t_lex *token)
{
	t_lex	*prev;
	t_lex	*next;
	t_lex	*current;

	prev = token->prev;
	next = token->next;
	if (prev != NULL)
		prev->next = next;
	if (next != NULL)
		next->prev = prev;
	current = token;
	while (current->next != NULL)
	{
		current = current->next;
		current->index--;
	}
	free(token->str);
	free(token);
	token = NULL;
}

void	ft_free_commands(t_command *cmd)
{
	int			i;
	t_command	*temp;

	while (cmd != NULL)
	{
		i = -1;
		temp = cmd->next;
		if (cmd->argv)
		{
			while (++i < cmd->argc)
				free(cmd->argv[i]);
			free(cmd->argv[i]);
			free(cmd->argv);
		}
		free(cmd);
		cmd = temp;
	}
	temp = NULL;
	cmd = NULL;
}

void	ft_init_command(t_command *cmd)
{
	cmd->argv = NULL;
	cmd->pipe_in = NULL;
	cmd->pipe_out = NULL;
	cmd->redir_in_file = NULL;
	cmd->redir_out_file = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
}
