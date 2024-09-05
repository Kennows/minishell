#include "minishell.h"

char	*ft_replace_var_heredoc(char *str, int start, int end, char **envp)
{
	char	*env;
	char	*new;

	env = NULL;
	if (end - start > 1)
	{
		if (!ft_getnenv(str + start + 1, &env, end - start, envp))
		{
			free(str);
			return (NULL);
		}
		str = ft_str_replace(str, env, start, end);
		if (!str)
			return (NULL);
		end = (start + ft_strlen(env));
		free(env);
	}
	if (str[end] != '\0')
	{
		new = ft_handle_env_heredoc(str, end, envp);
		return (new);
	}
	return (str);
}

char	*ft_handle_env_heredoc(char *str, int start, char **envp)
{
	int	end;
	char	*new;

	new = ft_strdup(str);
	if (!new)
	{
		free(str);
		return (NULL);
	}
	while (new[start] && new[start] != '$')
		start++;
	if (new[start] == '$')
	{
		end = start + 1;
		while (ft_isalnum(new[end]))
			end++;
		new = ft_replace_var_heredoc(new, start, end, envp);
		if (!new)
			write(2, "malloc failed during variable expansion\n", 40);
	}
	free(str);
	return (new);
}
