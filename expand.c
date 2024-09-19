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

int	ft_getnenv(char *str, char **env, int n, char **envp)
{
	char	*temp;

	temp = malloc(sizeof(char) * n);
	if (!temp)
		return (0);
	ft_strlcpy(temp, str, n);
	if (ft_getenv(temp, envp))
	{
		*env = ft_strdup(ft_getenv(temp, envp));
		if (!*env)
		{
			write(2, "malloc failed while getting env\n", 32);
			free(temp);
			return (0);
		}	
	}
	free(temp);
	return (1);
}

int	ft_find_env_end(char *str, int start)
{
	int	end;

	end = start + 1;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	return (end);
}

int	ft_replace_var(char **str, int start, char **envp)
{
	char	*env;
	int		end;

	while (str[0][start] != '\0')
	{	
		env = NULL;
		end = ft_find_env_end(str[0], start);
		if (end - start > 1)
		{
			if (!ft_getnenv(str[0] + start + 1, &env, end - start, envp))
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
			start++;
	}
	return (1);
}

int	ft_find_env_start(char *str, int i)
{
	static int	quoted = 0;

	while (str[i] != '\0' && str[i] != '$')
	{
		if (str[i] == '"' && quoted == 0)
			quoted = 1;
		else if (str[i] == '"' && quoted == 1)
			quoted = 0;
		if (str[i] == '\'' && quoted == 0)
			while (str[++i] != '\'' && str[i] != '\0')
				continue ;
		if (str[i] == '\0')
			break ;
		i++;
	}
	return (i);
}

int	ft_expand_and_word_split(t_command *cmd, char **str, int start)
{
	char	**new;
	int		i;
	int		split_start;
	int		split_end;

	i = 0;
	if (!ft_replace_var(&*str, start, *cmd->envp))
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
	while (new[i] != NULL)
	{
		if (!ft_array_append(&cmd->argv, new[i++]))
		{
			ft_free_array(new);
			return (0);
		}
		cmd->argc++;
	}
	ft_free_array(new);
	if (i > 0 && cmd->argc > 0 && !split_end)
		cmd->argc--;
	return (1);
}

int	ft_handle_env(t_lex *token, t_command *cmd)
{
	int		start;

	while (token->subtoken != NULL)
	{
		start = ft_find_env_start(token->subtoken->str, 0);
		if (token->subtoken->str[start] == '$' && token->subtoken->quote == 0)
		{
			if (!ft_expand_and_word_split(cmd, &token->subtoken->str, start))
				return (0);
			token->subtoken = ft_remove_subtoken(token->subtoken);
			continue ;
		}		
		if (token->subtoken->str[start] == '$' && token->subtoken->quote == 2 \
			&& !ft_replace_var(&token->subtoken->str, start, *cmd->envp))
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

char	*ft_handle_file_env(char **new, t_lex *token, char **envp)
{
	int		start;

	while (token->subtoken != NULL)
	{
		start = ft_find_env_start(token->subtoken->str, 0);
		if (token->subtoken->str[start] == '$' && \
			(token->subtoken->quote == 0 || token->subtoken->quote == 2) && \
			!ft_replace_var(&token->subtoken->str, start, envp))
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

char	*ft_expand_file_name(char *str, t_lex *token, char **envp)
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
		new = ft_handle_file_env(&new, token, envp);
		if (!new)
			return (NULL);
		if (new[0] == '\0')
			ft_print_redir_env_error(str);
		return (new);
	}
	return (NULL);
}

int	ft_is_env_quoted(char *str, int start)
{
	int	i;
	int	d_quoted;
	int	s_quoted;

	i = 0;
	d_quoted = 0;
	s_quoted = 0;
	while (str[i] && i < start)
	{
		if (str[i] == '"' && d_quoted == 0 && s_quoted == 0)
			d_quoted = 1;
		else if (str[i] == '"' && d_quoted == 1)
			d_quoted = 0;
		if (str[i] == '\'' && s_quoted == 0 && d_quoted == 0)
			s_quoted = 1;
		else if (str[i] == '\'' && s_quoted == 1)
			s_quoted = 0;
		i++;
	}
	if (d_quoted || s_quoted)
		return (1);
	return (0);
}

int	ft_parse_cmds_and_args(t_command *cmd)
{
	t_lex		**token;

	while (cmd != NULL)
	{
		token = &cmd->token_start;
		while (*token != NULL && cmd->token_end != NULL && \
				(*token)->index <= cmd->token_end->index)
		{
			if (!ft_handle_quotes((*token)->str, *token, 0))
				return (0);
			if (!ft_handle_env(*token, cmd))
			{
				write(2, "error handling envs\n", 20);
				return (0);
			}
			*token = ft_remove_token(&*token, cmd);
			if (cmd->argv && cmd->argv[cmd->argc])
				cmd->argc++;
		}
		ft_command_type(cmd);
		if (cmd->next == NULL)
			return (1);
		cmd = cmd->next;
	}
	return (1);
}
