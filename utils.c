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
			write(2, "malloc failed while copying an array\n", 37);
			ft_free_array(dest);
			ft_free_array(src);
			return (NULL);
		}
	}
	dest[i2] = NULL;
	ft_free_array(src);
	return (dest);
}

int	ft_append_empty_arr(char ***arr, char *str)
{
	arr[0] = malloc(sizeof(char *) * 2);
	if (!arr[0])
	{
		write(2, "malloc failed while appending to an empty array\n", 48);
		return (0);
	}
	arr[0][0] = ft_strdup(str);
	if (!arr[0][0])
	{
		free(arr[0]);
		arr[0] = NULL;
		write(2, "malloc failed while appending to an empty array\n", 48);
		return (0);
	}
	arr[0][1] = NULL;
	return (1);
}

int	ft_array_append(char ***array, char *str)
{
	int		i;
	char	**new;

	if (*array == NULL && ft_append_empty_arr(&*array, str))
		return (1);
	else if (*array == NULL)
		return (0);
	i = ft_arrlen(*array);
	new = malloc((i + 2) * sizeof(char *));
	if (!new)
		write(2, "malloc failed while appending to an array\n", 42);
	else
		new = ft_array_cpynfree(new, *array, NULL);
	if (!new)
		return (0);
	new[i] = ft_strdup(str);
	if (!new[i])
	{
		write(2, "malloc failed while appending to an array\n", 42);
		ft_free_array(new);
		return (0);
	}
	new[++i] = NULL;
	*array = new;
	return (1);
}

char	*ft_str_replace(char *str, char *substitute, int start, int end)
{
	char	*new;

	if (substitute)
		new = malloc(sizeof(char) * ((ft_strlen(str) + ft_strlen(substitute) \
									- (end - start) + 1)));
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
