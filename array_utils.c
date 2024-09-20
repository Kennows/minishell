/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:50:07 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/07 20:26:26 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_arrlen(char **arr)
{
	int	len;

	len = 0;
	while (arr && arr[len] != NULL)
		len++;
	return (len);
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

int	ft_array_append_array(char ***dest, char ***src, int start)
{
	int	count;

	count = 0;
	while (src[0][start] != NULL)
	{
		if (!ft_array_append(&*dest, src[0][start++]))
		{
			ft_free_array(&**src);
			return (0);
		}
		count++;
	}
	ft_free_array(&**src);
	return (count);
}
