/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:55:02 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/28 16:40:11 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pipe_syntax_check(t_lex **tokens, t_command **cmd)
{
	if ((*tokens)->type == PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 50);
		free(*cmd);
		ft_free_tokens(&*tokens);
		return (0);
	}
	(*cmd)->token_start = *tokens;
	while ((*tokens)->next != NULL && (*tokens)->next->type != PIPE)
		*tokens = (*tokens)->next;
	(*cmd)->token_end = *tokens;
	if ((*tokens)->next != NULL)
	{
		if ((*tokens)->next->next == NULL)
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 50);
			free(*cmd);
			ft_free_tokens(&*tokens);
			return (0);
		}
	}
	return (1);
}

int	ft_redir_syntax_check(t_lex **token, t_command **cmd, \
				t_command_table **table)
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
		ft_free_all(*token, *cmd, *table);
		return (0);
	}
	return (1);
}

int ft_export_syntax_check(char *arg)
{
	int i;

	i = 1;
	if (ft_isalpha(arg[0]) || arg[0] == '_')
	{
		while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
			i++;
		if (arg[i] == '\0')
			return (1);
	}
	i = 0;
	write(2, "minishell: export: `", 20);
	while (arg[i])
		write(2, &arg[i++], 1);
	write(2, "': not a valid identifier\n", 26);
	return (0);
}