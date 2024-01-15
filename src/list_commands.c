/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:43:57 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/15 20:33:07 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_simple_command	*command_add(t_command_chain **commands, char **cmd_with_args, t_list *redirections)
{
	t_simple_command *command;
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
	command = malloc(sizeof(t_simple_command));
	if (!command)
		return NULL;
	ft_bzero(command, sizeof(t_simple_command));

	if (cmd_with_args)
		command->cmd_with_args= cmd_with_args;
	if (redirections)
		command->redirections = redirections;
	t_command_chain *new_command = ft_lstnew(command);
	if (!new_command)
		return (destroy_command(command), NULL);
	ft_lstadd_back(commands, new_command);
	return (command);
}


void	destroy_command(void *command_void)
{
	t_simple_command *command = command_void;

	(void) command;

	//free(commands)...;
	//free(redirections)...;
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
		printf("doc: %s\n", redir->doc);
		cur_redir = cur_redir->next;
		i++;
	}
}

void	print_commands(t_command_chain **commands)
{
	t_command_chain		*cmd_list;
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
		str_print(cur_cmd->cmd_with_args);
		printf("redirection_list:\n");
		print_redirs(&cur_cmd->redirections);
		printf("}\n");
		cmd_list = cmd_list->next;
		i++;
	}
}
