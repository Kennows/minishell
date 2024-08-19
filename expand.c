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
	if (new[end] != '\0')
		new = ft_handle_env(new, end);
	return (new);
}

int	ft_find_env_start(char *str, int i)
{
	int	quoted;

	quoted = 0;
	while (str[i] && str[i] != '$')
	{
		if (str[i] == '"' && quoted == 0)
			quoted = 1;
		else if (str[i] == '"' && quoted == 1)
			quoted = 0;
		if (str[i] == '\'' && quoted == 0)
			while (str[++i] != '\'')
				continue ;
		i++;
	}
	return (i);
}


char	*ft_handle_env(char *str, int start)
{
	int	end;
	char	*var;

	start = ft_find_env_start(str, start);
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

char	*ft_remove_quotes(char *str, char *new, char q, int *start)
{
	int	i;
	int	quotes;

	quotes = 0;
	i = -1;
	while (++i < *start)
		new[i] = str[i];
	while (str[i + quotes])
	{
		while (str[i + quotes] == q && quotes < 2)
		{
			quotes++;
			if (quotes == 2)
				*start = i;
		}
		new[i] = str[i + quotes];
		i++;
	}
	new[i] = '\0';
	free(str);
	return (new);
}

char	*ft_handle_quotes(char *str, int start)
{
	char	*new;

	while (str[start] && str[start] != '"' && str[start] != '\'')
		start++;
	if (str[start] == '\0')
		return (str);
	new = malloc(sizeof(char) * (ft_strlen(str) - 1));
	if (!new)
	{
		free(str);
		write(2, "malloc failed while removing quotes\n", 36);
		return (NULL);
	}
	if (str[start] == '"')
		new = ft_remove_quotes(str, new, '"', &start);
	else if (str[start] == '\'')
		new = ft_remove_quotes(str, new, '\'', &start);
	if (new[start] != '\0')
		new = ft_handle_quotes(new, start);
	return (new);
}

char	*ft_expand(char *str)
{
	char	*new;

	new = NULL;
	if (str)
	{
		new = ft_handle_env(str, 0);
		if (!new)
			return (NULL);
		new = ft_handle_quotes(new, 0);
		if (!new)
			return (NULL);
		return (new);
	}
	return (str);
}
