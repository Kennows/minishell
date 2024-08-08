#include "minishell.h"

int	ft_pipe_syntax_check(t_lex **tokens, t_command **cmd)
{
	if ((*tokens)->type == PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		free(*cmd);
		ft_free_tokens(*tokens);
		return (0);
	}
	(*cmd)->token_start = *tokens;
	while ((*tokens)->next != NULL && (*tokens)->next->type != PIPE)
		*tokens = (*tokens)->next;
	(*cmd)->token_end = *tokens;
	ft_init_command(*cmd);
	if ((*tokens)->next != NULL)
	{
		if ((*tokens)->next->next == NULL)
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			free(*cmd);
			ft_free_tokens(*tokens);
			return (0);
		}
	}
	return (1);
}

int	ft_redir_syntax_check(t_lex **token, t_command **cmd, t_command_table **table)
{
	if ((*token)->next == NULL || (*token)->next->type != WORD || \
		(*cmd)->token_end->index <= (*token)->index)
	{
		if ((*token)->type == REDIR_IN)
			write(2, "minishell: syntax error near unexpected token `<'\n", 50);
		else if ((*token)->type == REDIR_OUT)
			write(2, "minishell: syntax error near unexpected token `>'\n", 50);
		else if ((*token)->type == APPEND)
			write(2, "minishell: syntax error near unexpected token `>>'\n", 51);
		else if ((*token)->type == HERE_DOC)
			write(2, "minishell: syntax error near unexpected token `<<'\n", 51);
		ft_free_tokens(*token);
		ft_free_commands((*table)->commands);
		ft_free_files((*table)->files);
		free(*table);
		return (0);
	}
	return (1);
}
