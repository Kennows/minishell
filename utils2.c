/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 19:51:09 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:51:50 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strprepend(char **str, char *prefix)
{
	int		i;
	int		i2;
	char	*new;

	i = -1;
	i2 = 0;
	new = malloc(sizeof(char) * (ft_strlen(*str) + ft_strlen(prefix) + 1));
	if (!new)
		return (0);
	while (prefix[++i] != '\0')
		new[i] = prefix[i];
	while (str[0][i2] != '\0')
		new[i++] = str[0][i2++];
	new[i] = '\0';
	free(str[0]);
	str[0] = new;
	return (1);
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

char	*ft_str_replace(char *str, char *substitute, int start, int end)
{
	char	*new;

	if (substitute)
		new = malloc(sizeof(char) * ((ft_strlen(str) + ft_strlen(substitute)) \
									- (end - start) + 1));
	else
		new = malloc(sizeof(char) * (ft_strlen(str) - (end - start) + 1));
	if (!new)
	{
		free(str);
		if (substitute)
			free(substitute);
		write(2, "malloc failed while expanding env\n", 34);
		return (NULL);
	}
	ft_strlcpy(new, str, start + 1);
	while (substitute && *substitute)
		new[start++] = *(substitute++);
	while (str[end])
		new[start++] = str[end++];
	new[start] = '\0';
	free(str);
	return (new);
}
int	ft_strchr_index(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != (char)c && s[i] != '\0')
		i++;
	if (s[i] == (char)c)
		return (i);
	return (-1);
}

void	ft_command_type(t_command *cmd)
{
	if (cmd->argv && cmd->argv[0])
	{
		if (!ft_strncmp(cmd->argv[0], "echo", 5) || \
			!ft_strncmp(cmd->argv[0], "cd", 3) || \
			!ft_strncmp(cmd->argv[0], "pwd", 4) || \
			!ft_strncmp(cmd->argv[0], "export", 7) || \
			!ft_strncmp(cmd->argv[0], "unset", 6) || \
			!ft_strncmp(cmd->argv[0], "env", 4) || \
			!ft_strncmp(cmd->argv[0], "exit", 5))
			cmd->type = BUILT_IN;
		else
			cmd->type = WORD;
	}
}
