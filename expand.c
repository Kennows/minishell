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
	{
		free(substitute);
		free(str);
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

int	ft_getnenv(char *str, char **env, int n)
{
	char	*temp;

	temp = malloc(sizeof(char) * n);
	if (!temp)
		return (0);
	ft_strlcpy(temp, str, n);
	if (getenv(temp))
	{
		*env = ft_strdup(getenv(temp));
		if (!*env)
		{
			free(temp);
			return (0);
		}
	}
	free(temp);
	return (1);
}

char	*ft_replace_var(char *str, int start, int end, int heredoc)
{
	char	*new;
	char	*env;

	env = NULL;
	if (end - start > 1)
	{
		if (!ft_getnenv(str + start + 1, &env, end - start))
		{
			free(str);
			return (NULL);
		}
		new = ft_str_replace(str, env, start, end);
		if (!new)
		{
			write(2, "malloc failed during variable expansion\n", 40);
			return (NULL);
		}
		end = (start + ft_strlen(env));
	}
	else
		new = str;
	if (new[end] != '\0')
		new = ft_handle_env(new, end, heredoc);
	free(env);
	return (new);
}

int	ft_find_env_start(char *str, int i, int heredoc)
{
	int	quoted;

	quoted = 0;
	while (str[i] && str[i] != '$')
	{
		if (heredoc == 0)
		{
			if (str[i] == '"' && quoted == 0)
				quoted = 1;
			else if (str[i] == '"' && quoted == 1)
				quoted = 0;
			if (str[i] == '\'' && quoted == 0)
				while (str[++i] != '\'')
					continue ;
		}
		i++;
	}
	return (i);
}

char	*ft_handle_env(char *str, int start, int heredoc)
{
	int	end;

	start = ft_find_env_start(str, start, heredoc);
	if (str[start] == '$')
	{
		end = start + 1;
		while (ft_isalnum(str[end]))
			end++;
		str = ft_replace_var(str, start, end, heredoc);
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
		new = ft_handle_env(str, 0, 0);
		if (!new)
			return (NULL);
		new = ft_handle_quotes(new, 0);
		if (!new)
			return (NULL);
		return (new);
	}
	return (str);
}
