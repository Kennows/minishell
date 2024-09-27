/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:36:45 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:53:47 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_replace_var(char **str, int start, t_command_table *table)
{
	char	*env;
	int		end;

	while (str[0][start] != '\0')
	{	
		env = NULL;
		end = ft_find_env_end(str[0], start);
		if (end - start > 1 || str[0][end++] == '?')
		{
			if (!ft_getnenv(str[0] + start + 1, &env, end - start, table))
				return (0);
			str[0] = ft_str_replace(str[0], env, start, end);
			if (!str[0])
				return (0);
			if (env != NULL)
				start += ft_strlen(env);
			if (env)
				free(env);
			start = ft_find_env_start(str[0], start);
		}
		else if (str[0][start] != '\0')
			start = ft_find_env_start(str[0], end);
	}
	return (1);
}

int	ft_expand_and_wordsplit(t_command *cmd, char **str, int start, \
								t_command_table *table)
{
	char	**new;
	int		i;
	int		split_start;
	int		split_end;

	i = 0;
	if (!ft_replace_var(&*str, start, table))
		return (0);
	split_start = ft_is_whitespace(str[0][0]);
	split_end = ft_is_whitespace(str[0][ft_strchr_index(str[0], '\0')]);
	if (!ft_word_split(&new, str[0]))
		return (0);
	if (cmd->argv && cmd->argv[cmd->argc] && !split_start && \
		!ft_strcombine(&cmd->argv[cmd->argc], new[i++]))
	{
		ft_free_array(new);
		return (0);
	}
	if (cmd->argv && cmd->argv[cmd->argc] && !split_start)
		cmd->argc++;
	if (!ft_array_append_array(&cmd->argv, &new, i))
		return (0);
	if (i > 0 && cmd->argc > 0 && !split_end)
		cmd->argc--;
	return (1);
}

int	ft_handle_env(t_lex *token, t_command *cmd, t_command_table *t)
{
	int		start;

	while (token->subtoken != NULL)
	{
		start = ft_find_env_start(token->subtoken->str, 0);
		if (token->subtoken->str[start] == '$' && token->subtoken->quote == 0)
		{
			if (!ft_expand_and_wordsplit(cmd, &token->subtoken->str, start, t))
				return (0);
			token->subtoken = ft_remove_subtoken(token->subtoken);
			continue ;
		}		
		if (token->subtoken->str[start] == '$' && token->subtoken->quote == 2 \
			&& !ft_replace_var(&token->subtoken->str, start, t))
			return (0);
		if (cmd->argv && cmd->argv[cmd->argc] != NULL)
		{
			if (!ft_strcombine(&cmd->argv[cmd->argc], token->subtoken->str))
				return (0);
		}
		else if (!ft_array_append(&cmd->argv, token->subtoken->str))
			return (0);
		token->subtoken = ft_remove_subtoken(token->subtoken);
	}
	return (1);
}

char	*ft_handle_file_env(char **new, t_lex *token, t_command_table *table)
{
	int		start;

	while (token->subtoken != NULL)
	{
		start = ft_find_env_start(token->subtoken->str, 0);
		if (token->subtoken->str[start] == '$' && \
			(token->subtoken->quote == 0 || token->subtoken->quote == 2) && \
			!ft_replace_var(&token->subtoken->str, start, table))
			return (0);
		if (new && *new != NULL && !ft_strcombine(&*new, token->subtoken->str))
		{
			free(*new);
			return (NULL);
		}
		else if (!*new)
			*new = ft_strdup(token->subtoken->str);
		if (!*new)
			return (NULL);
		token->subtoken = ft_remove_subtoken(token->subtoken);
	}
	return (*new);
}

char	*ft_expand_file_name(char *str, t_lex *token, t_command_table *table)
{
	char	*new;

	new = NULL;
	if (str)
	{
		if (!ft_handle_quotes(&*str, token, 0))
		{
			write(2, "error expanding file name\n", 26);
			return (NULL);
		}
		new = ft_handle_file_env(&new, token, table);
		if (!new)
			return (NULL);
		if (new[0] == '\0')
			ft_print_redir_env_error(str);
		return (new);
	}
	return (NULL);
}
