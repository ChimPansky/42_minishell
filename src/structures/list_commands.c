/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:43:57 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/01 15:36:18 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_commands.h"

t_simple_command	*command_add(t_commandlist **commands, char **cmd_with_args, t_list *redirections)
{
	t_simple_command *command;

	command = malloc(sizeof(t_simple_command));
	if (!command)
		return NULL;
	ft_bzero(command, sizeof(t_simple_command));
	if (cmd_with_args)
		command->cmd_with_args= cmd_with_args;
	if (redirections)
		command->redirections = redirections;
	t_commandlist *new_command = ft_lstnew(command);
	if (!new_command)
		return (command_destroy(command), NULL);
	ft_lstadd_back(commands, new_command);
	return (command);
}

void	command_destroy(void *command_void)
{
	t_simple_command *command = command_void;

	(void) command;

	//free(commands)...;
	//free(redirections)...;
}

void commandlist_destroy(t_commandlist **commands)
{
	ft_lstclear(commands, command_destroy);
}

void	print_redirs(t_list **redirections)
{
	t_redirections	*cur_redir;
	t_redir_detail	*redir;
	char			*fd_type;
	int			i;

	i = 1;
	cur_redir = *redirections;
	while (cur_redir)
	{
		redir = cur_redir->content;
		if (redir->type == FD_IN)
			fd_type = "<";
		else if (redir->type == FD_HEREDOC)
			fd_type = "<<";
		else if (redir->type == FD_OUT_TRUNC)
			fd_type = ">";
		else if (redir->type == FD_OUT_APPEND)
			fd_type = ">>";
		printf("%d:\n", i);
		printf("type: %s\n", fd_type);
		printf("str: %s\n", redir->string.buf);
		printf("had whitespace in expansion: %d\n", redir->whitespace_expansion);
		cur_redir = cur_redir->next;
		i++;
	}
}

void	print_commands(t_commandlist **commands)
{
	t_commandlist		*cmd_list;
	t_simple_command	*cur_cmd;
	int			i;

	if (!commands)
		return ;
	cmd_list = *commands;
	i = 1;
	while (cmd_list && cmd_list->content)
	{
		cur_cmd = cmd_list->content;
		printf("{simple command %d:\n", i);
		printf("cmd_with_args:\n");
		strings_print(cur_cmd->cmd_with_args);
		printf("redirection_list:\n");
		print_redirs(&cur_cmd->redirections);
		printf("}\n\n");
		cmd_list = cmd_list->next;
		i++;
	}
}
