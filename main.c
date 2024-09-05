/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nvallin <nvallin@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:22:21 by nvallin           #+#    #+#             */
/*   Updated: 2024/08/27 19:02:54 by nvallin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_tokens(t_lex *t)
{
	t_lex	*current;

	current = t;
	while (current->prev != NULL)
		current = current->prev;
	while (current->next != NULL)
	{
		ft_printf("token %s, type %d, index %d\n", current->str, current->type, current->index);
		current = current->next;
	}
	ft_printf("token %s, type %d, index %d\n\n", current->str, current->type, current->index);
}

void	ft_print_table(t_command_table *t)
{
	int			i;
	t_command	*current;

	i = 1;
	current = t->commands;
	while (current->next != NULL)
	{
		if (current->argv && current->argv[0])
			ft_printf("command: %s, type: %d, ", current->argv[0], current->type);
		while (i < current->argc)
		{
			if (current->argv[i])
				ft_printf("argument %d: %s, ", i, current->argv[i]);
			i++;
		}
		if (current->redir_in_file != NULL)
			ft_printf("redirect in: %s, ", current->redir_in_file->name);
		if (current->redir_out_file != NULL)
			ft_printf("redir out: %s, ", current->redir_out_file->name);
		if (current->pipe_in != NULL && current->pipe_in->argv[0])
			ft_printf("pipe in: %s, ", current->pipe_in->argv[0]);
		if (current->pipe_out != NULL && current->pipe_out->argv[0])
			ft_printf("pipe out: %s, ", current->pipe_out->argv[0]);
		current = current->next;
		printf("\n");
		i = 1;
	}
	if (current->argv && current->argv[0])
		ft_printf("command: %s, type: %d, ", current->argv[0], current->type);
	while (i < current->argc)
	{
		if (current->argv[i])
			ft_printf("argument %d: %s, ", i, current->argv[i]);
		i++;
	}
	if (current->redir_in_file != NULL)
		ft_printf("redirect in: %s, ", current->redir_in_file->name);
	if (current->redir_out_file != NULL)
		ft_printf("redir out: %s, ", current->redir_out_file->name);
	if (current->pipe_in != NULL && current->pipe_in->argv[0])
		ft_printf("pipe in: %s\n", current->pipe_in->argv[0]);
	if (current->pipe_out != NULL && current->pipe_out->argv[0])
		ft_printf("pipe out: %s\n", current->pipe_out->argv[0]);
	printf("\n");
}

void	ft_print_files(t_file *files)
{
	if (files)
		while (files->prev != NULL)
			files = files->prev;
	while (files != NULL)
	{
		ft_printf("file: %s type %d\n", files->name, files->type);
		files = files->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_lex			*tokens;
	t_command_table	*table;
	char			*cmd;

	if (argc != 1 || argv[1])
	{
		write(2, "please execute with no arguments!\n", 34);
		return (0);
	}
	table = NULL;
	table = ft_create_cmd_table(table, envp);
	if (!table)
		return (0);
	while (1)
	{
		cmd = readline("minishell$ ");
		if (cmd != NULL)
		{
			if (!ft_strncmp(cmd, "exit", 5))
			{
				free(cmd);
				//free table
				return (0);
			}
			add_history(cmd);
			tokens = ft_tokenize(cmd);
			free(cmd);
			if (!tokens)
				continue ;
			ft_print_tokens(tokens); 
			if (!ft_add_commands(&table, tokens))
				continue ;	
			ft_print_table(table);
			ft_print_files(table->files);
			puts("");
			ft_free_files(&table->files);
			ft_free_commands(&table->commands);
			table->files = NULL;
			table->commands = NULL;
		}
	}
	return (0);
}
