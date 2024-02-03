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
#include "libft.h"

static int command_init(t_simple_command **cmd, t_cmd_type type)
{
	*cmd = malloc(sizeof(t_simple_command));
	if (!*cmd)
		return !SUCCESS;
	if (type == CMD_EXEC)
	{
		if (charptr_array_init(&(*cmd)->cmd_with_args) != SUCCESS)
			return (free(*cmd), !SUCCESS);
	}
	(*cmd)->cmd_type = type;
	(*cmd)->redirections = NULL;
	return SUCCESS;
}

static void command_destroy(void *cmd_p)
{
	t_simple_command *cmd = cmd_p;

	if (cmd->cmd_type == CMD_EXEC)
		charptr_array_destroy(&cmd->cmd_with_args);
	else if (cmd->cmd_type == CMD_SUBSHELL)
		cmdlist_destroy(&cmd->subcommand);
	// redirlist_destroy(&(*cmd)->redirections);
	free(cmd);
}

void cmdlist_destroy(t_cmdlist **commands)
{
	ft_lstclear(commands, command_destroy);
}

t_simple_command *cmdlist_add_cmd(t_cmdlist **cmdlist, t_cmd_type type)
{
	t_simple_command *cmd;

	if (SUCCESS != command_init(&cmd, type))
		return NULL;
	t_cmdlist *new_node = ft_lstnew(cmd);
	if (!new_node)
		return (command_destroy(&cmd), NULL);
	ft_lstadd_back(cmdlist, new_node);
	return cmd;
}

// t_simple_command	*command_add(t_cmdlist **commands, char **cmd_with_args, t_list *redirections)
// {
// 	t_simple_command *command;

// 	command = malloc(sizeof(t_simple_command));
// 	if (!command)
// 		return NULL;
// 	ft_bzero(command, sizeof(t_simple_command));
// 	if (cmd_with_args)
// 		command->cmd_with_args = cmd_with_args;
// 	if (redirections)
// 		command->redirections = redirections;
// 	t_cmdlist *new_command = ft_lstnew(command);
// 	if (!new_command)
// 		return (command_destroy(command), NULL);
// 	ft_lstadd_back(commands, new_command);
// 	return (command);
// }



// void	print_redirs(t_list **redirections)
// {
// 	t_redirections	*cur_redir;
// 	t_redir_detail	*redir;
// 	char			*fd_type;
// 	int			i;

// 	i = 1;
// 	cur_redir = *redirections;
// 	while (cur_redir)
// 	{
// 		redir = cur_redir->content;
// 		if (redir->type == FD_IN)
// 			fd_type = "<";
// 		else if (redir->type == FD_HEREDOC)
// 			fd_type = "<<";
// 		else if (redir->type == FD_OUT_TRUNC)
// 			fd_type = ">";
// 		else if (redir->type == FD_OUT_APPEND)
// 			fd_type = ">>";
// 		printf("%d:\n", i);
// 		printf("type: %s\n", fd_type);
// 		printf("str: %s\n", redir->string.buf);
// 		cur_redir = cur_redir->next;
// 		i++;
// 	}
// }

// void	cmdlist_print(t_cmdlist **commands)
// {
// 	t_cmdlist		*cmd_list;
// 	t_simple_command	*cur_cmd;
// 	int			i;

// 	if (!commands)
// 		return ;
// 	cmd_list = *commands;
// 	i = 1;
// 	while (cmd_list && cmd_list->content)
// 	{
// 		cur_cmd = cmd_list->content;
// 		printf("{simple command %d:\n", i);
// 		printf("cmd_with_args:\n");
// 		strings_print(cur_cmd->cmd_with_args);
// 		printf("redirection_list:\n");
// 		print_redirs(&cur_cmd->redirections);
// 		printf("}\n\n");
// 		cmd_list = cmd_list->next;
// 		i++;
// 	}
// }
