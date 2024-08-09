#include "minishell.h"

char	*ft_str_replace(char *str, char *substitute, int start, int end)
{
	int	len;
	char	*new;
	int	i;

	i = -1;
	len = ((ft_strlen(str) + ft_strlen(substitute)) - (end - start));
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	while (++i < start)
		new[i] = str[i];
	while (substitute && *substitute)
		new[i++] = *substitute++;
	while (str[end])
		new[i++] = str[end++];
	new[i] = '\0';
	return (new);
}

char	*ft_replace_var(char *str, int start, int end, char *env)
{
	char	*new;

	if (end - start > 1)
	{
		new = ft_str_replace(str, env, start, end);
		if (!new)
		{
			write(2, "malloc failed during variable expansion\n", 40);
			free(str);
			return (NULL);
		}
		free(str);
		end = (start + ft_strlen(env));
	}
	else
		new = str;
	while (new[end] && new[end] != '$')
		end++;
	if (new[end] == '$')
		new = ft_handle_env(new, end);
	return (new);
}

char	*ft_handle_env(char *str, int start)
{
	int	end;
	char	*var;

	while (str[start] != '\0' && str[start] != '$')
		start++;
	if (str[start] == '$')
	{
		end = start + 1;
		while (ft_isalnum(str[end]))
			end++;
		var = ft_substr(str, start, (end - start));
		if (!var)
		{
			write(2, "malloc failed during variable expansion\n", 40);
			free(str);
			return (NULL);
		}
		str = ft_replace_var(str, start, end, getenv(var + 1));
		free(var);
		return (str);
	}
	return (str);
}

char	*ft_remove_quotes(char *str)
{
	char	*new;
	int	len;
	int	i;

	i = -1;
	len = ft_strlen(str);
	new = malloc(sizeof(char) * (len - 1));
	if (!new)
	{
		write(2, "malloc failed while removing quotes\n", 36);
		free(str);
		return (NULL);
	}
	while (++i < (len - 2))
		new[i] = str[i + 1];
	new[i] = '\0';
	free(str);
	return (new);
}

char	*ft_expand(char *str)
{
	char	*new;

	new = NULL;
	if (str)
	{
		if (str[0] == '"')
		{
			new = ft_handle_env(str, 0);
			if (!new)
				return (NULL);
			new = ft_remove_quotes(new);
		}
		else if (str[0] == '\'')
			new = ft_remove_quotes(str);
		else
			new = ft_handle_env(str, 0);
		if (!new)
			return (NULL);
		return (new);
	}
	return (str);
}
