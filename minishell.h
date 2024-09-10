/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:54:44 by nvallin           #+#    #+#             */
/*   Updated: 2024/09/07 20:28:02 by nvallin          ###   ########.fr       */
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
# include <signal.h>

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
	char	**envp;
}		t_command_table;

void			ft_free_tokens(t_lex **tokens);
t_lex			*ft_remove_token(t_lex *token);
t_lex			*ft_new_token(char *str, int index);
t_lex			*ft_add_token(t_lex *tokens, char *str);
t_lex			*ft_tokenize(char *cmd);
char			**ft_token_split(char const *s);

void			ft_free_commands(t_command	**table);
void			ft_init_command(t_command *cmd, t_command_table *table);
int				ft_init_argv(t_command *cmd, t_command_table *table);
t_command		*ft_parse_pipes(t_command_table *table, \
				t_command *head, t_lex *tokens,
								t_command *prev);
t_command		*ft_parse_redirections(t_command *cmd, t_command_table *table);
t_lex			*ft_parse_redirect_in(t_command *cmd, t_lex *current_token, \
										t_file **file);
int				ft_handle_redir(t_command *cmd, t_lex **token, \
								t_command_table *table);
t_lex			*ft_parse_append(t_command *cmd, t_lex *current_token, \
									t_file **file);
int				ft_parse_heredoc(t_command *cmd, t_lex **token, \
									t_command_table *table);
int			ft_add_commands(t_command_table **table, t_lex *tokens);
t_command_table	*ft_create_cmd_table(t_command_table *table, char **envp);
t_command		*ft_parse(t_lex *tokens);

int				ft_isnumber(const char *str);
char			**ft_array_cpynfree(char **dest, char **src, char *ignore);
char			**ft_array_append(char **array, char *str);

void			ft_print_tokens(t_lex *t);

void			ft_free_files(t_file **files);
int				ft_create_heredoc(char **delimiter, char *filename, \
		t_command_table *table);
char			*ft_heredoc_name(void);

//void			ft_sighandler(int signal);
//void			ft_set_sig_handler(void);

int				ft_pipe_syntax_check(t_lex **tokens, t_command **cmd);
int				ft_redir_syntax_check(t_lex **token, t_command **cmd, \
										t_command_table **table);

void			ft_free_all(t_lex *token, t_command *cmd, \
							t_command_table *table);

char			*ft_str_replace(char *str, char *substitute, int start, \
								int end);
char			*ft_replace_var(char *str, int start, int end, char **envp);
char			*ft_remove_quotes(char *str, char *new, char q, int *start);
char			*ft_handle_quotes(char *str, int start);
char			*ft_handle_env(char *str, int start, char **envp);
char			*ft_expand(char *str, char **envp);
void			ft_command_type(t_command *cmd);
int			ft_is_whitespace(char c);
char			**ft_envpdup(char **envp);
char			*ft_getenv(char *str, char **envp);
char			*ft_replace_var_heredoc(char *str, int start, int end, char **envp);
char			*ft_handle_env_heredoc(char *str, int start, char **envp);
int			ft_getnenv(char *str, char **env, int n, char **envp);

void			ft_env(char **env, int export);
int			ft_builtin(t_command *cmd, t_command_table *table);
void			ft_free_array(char **array);
int			ft_replace_env_value(char ***envp, char *new);
char	*ft_strcombine(char *dest, char *src);

int	ft_builtin(t_command *cmd, t_command_table *table);
int	ft_echo(t_command *cmd);
int	ft_cd(t_command *cmd);
void	ft_pwd(void);
int	ft_export(t_command *cmd);
int	ft_unset(t_command *cmd);
void	ft_env(char **env, int export);
int	ft_exit(t_command_table *table);

int	ft_arrlen(char **arr);
char	**ft_create_envp(char **mini_envp, char **envp, char *shell);
char	**ft_envpdup(char **envp);
int	ft_update_pwd(t_command *cmd, char **oldpwd);
int	ft_envcmp(char *env, char *new);

void	ft_print_table(t_command_table *t);
#endif
