/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:50:07 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/07 20:26:26 by nvallin          ###   ########.fr       */
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

char	**ft_array_cpynfree(char **dest, char **src, char *ignore)
{
	int	i;
	int	i2;

	i = -1;
	i2 = 0;
	while (src && src[++i] != NULL)
	{
		if (ignore != NULL && !ft_strncmp(ignore, src[i], ft_strlen(ignore)) \
			&& (src[i][ft_strlen(ignore)] == '=' \
			|| src[i][ft_strlen(ignore)] == '\0'))
			i++;
		if (src[i] == NULL)
			break ;
		dest[i2] = ft_strdup(src[i]);
		if (!dest[i2++])
		{
			ft_free_array(dest);
			ft_free_array(src);
			return (NULL);
		}
	}
	dest[i2] = NULL;
	ft_free_array(src);
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
	{
		ft_free_array(array);
		return (NULL);
	}
	new = ft_array_cpynfree(new, array, NULL);
	if (!new)
		return (NULL);
	new[i] = ft_strdup(str);
	if (!new[i])
	{
		ft_free_array(new);
		ft_free_array(array);
		return (NULL);
	}
	new[++i] = NULL;
	return (new);
}

char	*ft_str_replace(char *str, char *substitute, int start, int end)
{
	int		len;
	char	*new;
	int		i;

	i = -1;
	len = ((ft_strlen(str) + ft_strlen(substitute)) - (end - start));
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
	{
		free(str);
		free(substitute);
		return (NULL);
	}
	while (++i < start)
		new[i] = str[i];
	while (substitute && *substitute)
		new[i++] = *(substitute++);
	while (str[end])
		new[i++] = str[end++];
	new[i] = '\0';
	free(str);
	return (new);
}

void	ft_command_type(t_command *cmd)
{
	if (cmd->argv[0])
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
