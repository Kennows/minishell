/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:33:39 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:57:39 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_replace_var_heredoc(char *str, int start, int end, \
								t_command_table *table)
{
	char	*env;

	env = NULL;
	if (end - start > 1)
	{
		if (!ft_getnenv(str + start + 1, &env, end - start, table))
		{
			free(str);
			return (NULL);
		}
		str = ft_str_replace(str, env, start, end);
		if (!str)
			return (NULL);
		end = start;
		if (env)
		{
			end += ft_strlen(env);
			free(env);
		}
	}
	if (str[end] != '\0')
		if (!ft_handle_env_heredoc(&str, end, table))
			return (NULL);
	return (str);
}

int	ft_handle_env_heredoc(char **str, int start, t_command_table *table)
{
	int		end;
	char	*new;

	new = ft_strdup(*str);
	if (!new)
	{
		free(*str);
		*str = NULL;
		return (0);
	}
	while (new[start] && new[start] != '$')
		start++;
	if (new[start] == '$')
	{
		end = start + 1;
		while (ft_isalnum(new[end]))
			end++;
		new = ft_replace_var_heredoc(new, start, end, table);
		if (!new)
			write(2, "malloc failed during variable expansion\n", 40);
	}
	free(*str);
	*str = new;
	return (1);
}
