/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:54:44 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/01 16:54:24 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

typedef enum s_token_type
{
	REDIR_IN,
	REDIR_OUT,
	HERE_DOC,
	APPEND,
	PIPE,
	BUILT_IN,
	NUMBER,
	WORD,
}	t_token_type;

typedef enum s_file_type
{
	OPEN,
	CREATE,
	ADD_TO,
}	t_file_type;

typedef struct s_lex
{
	char			*str;
	t_token_type	type;
	int				index;
	struct s_lex	*next;
	struct s_lex	*prev;
}		t_lex;

typedef struct s_file
{
	char			*name;
	t_file_type		type;
	struct s_file	*next;
	struct s_file	*prev;
}	t_file;

typedef struct s_command
{
	char				**argv;
	int					argc;
	t_token_type		type;
	struct s_lex		*token_start;
	struct s_lex		*token_end;
	struct s_command	*pipe_in;
	struct s_command	*pipe_out;
	t_file				*redir_in_file;
	t_file				*redir_out_file;
	struct s_command	*next;
	struct s_command	*prev;
}		t_command;

typedef struct s_command_table
{
	struct s_command	*commands;
	struct s_file		*files;
}		t_command_table;

void			ft_free_tokens(t_lex *tokens);
void			ft_remove_token(t_lex *token);
t_lex			*ft_new_token(char *str, int index);
t_lex			*ft_add_token(t_lex *tokens, char *str);
t_lex			*ft_tokenize(char *cmd);
char			**ft_token_split(char const *s, char c);

void			ft_free_commands(t_command	*table);
void			ft_init_command(t_command *cmd);
t_command		*ft_parse_pipes(t_command *head, t_lex *tokens, t_command *prev);
t_command		*ft_parse_redirections(t_command *cmd, t_command_table *table);
t_lex			*ft_parse_redirect_in(t_command *cmd, t_lex *current_token, t_file **file);
t_lex			*ft_parse_redirect_out(t_command *cmd, t_lex *current_token, t_file **file);
t_lex			*ft_parse_append(t_command *cmd, t_lex *current_token, t_file **file);
t_lex			*ft_parse_heredoc(t_command *cmd, t_lex *current_token, t_file **file);
t_command_table	*ft_add_commands(t_lex *tokens);
t_command		*ft_parse(t_lex *tokens);

int				ft_isnumber(const char *str);
char			**ft_array_append(char **array, char *str);

void			ft_print_tokens(t_lex *t);
void			ft_free_files(t_file *files);
void			ft_create_heredoc(char *delimiter, char *filename);

#endif
