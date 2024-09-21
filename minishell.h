/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:54:44 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/18 19:50:56 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
#include <sys/ioctl.h>
# include <fcntl.h>
# include <signal.h>

typedef enum e_token_type
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

typedef enum e_file_type
{
	OPEN,
	CREATE,
	ADD_TO,
}	t_file_type;

typedef struct s_sub_tok
{
	char					*str;
	int						quote;
	struct s_sub_tok		*next;
	struct s_sub_tok		*prev;
}		t_sub_tok;

typedef struct s_lex
{
	char			*str;
	t_token_type	type;
	int				index;
	struct s_lex	*next;
	struct s_lex	*prev;
	t_sub_tok		*subtoken;
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
	char				***envp;
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
	char				**envp;
	int					exit_status;
}		t_command_table;

void			ft_free_tokens(t_lex **tokens);
t_lex			*ft_remove_token(t_lex **token, t_command *cmd);
t_lex			*ft_new_token(char *str, int index);
t_lex			*ft_add_token(t_lex *tokens, char *str);
t_lex			*ft_tokenize(char *cmd);
char			**ft_token_split(char const *s);
void			ft_init_subtoken(t_sub_tok *subtoken, int quote);
void			ft_free_commands(t_command	**table);
void			ft_init_command(t_command *cmd, t_command_table *table);
t_command		*ft_parse_pipes(t_command_table *table, t_command *head, \
								t_lex **tokens, t_command *prev);
t_lex			*ft_remove_redirection(t_command *cmd, t_lex **token);
int				ft_parse_redirections(t_command *cmd, t_command_table *table);
int				ft_parse_cmds_and_args(t_command *cmd, t_command_table *table);
int				ft_handle_redir(t_command *cmd, t_lex **token, \
								t_command_table *table);
int				ft_parse_heredoc(t_command *cmd, t_lex **token, \
									t_command_table *table);
int				ft_add_commands(t_command_table **table, t_lex **tokens);
t_command_table	*ft_create_cmd_table(t_command_table *table, char **envp);
int				ft_isnumber(const char *str);
char			**ft_array_cpynfree(char **dest, char **src, char *ignore);
int				ft_array_append(char ***array, char *str);
int				ft_array_append_array(char ***dest, char ***src, int start);
void			ft_free_array(char **array);
void			ft_free_files(t_file **files);
void			ft_free_all(t_lex *token, t_command *cmd, \
							t_command_table *table);
int				ft_create_heredoc(char **delimiter, char *filename, \
									t_command_table *table);
char			*ft_heredoc_name(void);
void			ft_sighandler(int signal);
void			ft_heredoc_sighandler(int signal);
void			ft_set_sig_handler(void);
void			ft_set_heredoc_sig_handler(void);
int				ft_pipe_syntax_check(t_lex **tokens, t_command **cmd, \
										t_command_table *table);
int				ft_redir_syntax_check(t_lex **token, t_command **cmd, \
										t_command_table **table);
int				ft_export_syntax_check(char *arg);
char			*ft_str_replace(char *str, char *substitute, int start, \
								int end);
int				ft_replace_var(char **str, int start, t_command_table *table);
int				ft_remove_quotes(char *str, t_lex *token, char q);
int				ft_handle_quotes(char *str, t_lex *token, int start);
int				ft_handle_env(t_lex *token, t_command *cmd, t_command_table *t);
char			*ft_expand_file_name(char *str, t_lex *token, \
									t_command_table *table);
void			ft_add_file_to_list(t_command_table *table, t_file *temp);
t_file			*ft_new_heredoc_file(t_file **file, char **name, \
										t_command_table *table);
void			ft_command_type(t_command *cmd);
int				ft_is_whitespace(char c);
char			*ft_getenv(char *str, char **envp);
int				ft_getnenv(char *str, char **env, int n, \
							t_command_table *table);
char			*ft_replace_var_heredoc(char *str, int start, int end, \
										t_command_table *table);
int				ft_handle_env_heredoc(char **str, int start, \
										t_command_table *table);
int				ft_replace_env_value(char ***envp, char *new);
int				ft_strcombine(char **dest, char *src);
int				ft_builtin(t_command *cmd, t_command_table *table);
void			ft_env(char **env, int export);
int				ft_echo(t_command *cmd);
int				ft_cd(t_command *cmd);
int				ft_pwd(void);
int				ft_export(t_command *cmd);
int				ft_unset(t_command *cmd);
int				ft_exit(t_command_table *table);
int				ft_arrlen(char **arr);
char			**ft_create_envp(char **mini_envp, char **envp, char *shell);
char			**ft_envpdup(char **envp);
int				ft_update_pwd(t_command *cmd, char *oldpwd);
int				ft_envcmp(char *env, char *new);
int				ft_envlen(char *env);
int				ft_word_split(char ***dest, char const *s);
int				ft_append_empty_arr(char ***arr, char *str);
char			*ft_remove_heredoc_quotes(char *str, char *new, char q, \
											int *start);
char			*ft_handle_heredoc_quotes(char *str, int start);
int				ft_find_env_start(char *str, int i);
int				ft_find_env_end(char *str, int start);
int				ft_find_quote_end_index(const char *s, char q, int start);
char			*ft_strldup(const char *s1, size_t len);
int				ft_is_env_quoted(char *str, int start);
char			*ft_strappend(char *dest, char *src);
int				ft_strprepend(char **str, char *prefix);
void			ft_init_table(t_command_table *table);
void			ft_print_redir_env_error(char *str);
void			ft_print_heredoc_warning(char *delimiter);
t_sub_tok		*ft_remove_subtoken(t_sub_tok *subtoken);
int				ft_strchr_index(const char *s, int c);

#endif
