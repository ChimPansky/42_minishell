/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:43:57 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/10 00:11:38 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_commands.h"
#include "libft.h"

static int	command_init(t_simple_command **cmd)
{
	*cmd = malloc(sizeof(t_simple_command));
	if (!*cmd)
		return (!SUCCESS);
	(*cmd)->cmd_type = CMD_NULL;
	(*cmd)->redirections = NULL;
	return (SUCCESS);
}

static void	command_destroy(void *cmd_p)
{
	t_simple_command	*cmd;

	cmd = cmd_p;
	if (cmd->cmd_type == CMD_EXEC)
		charptr_array_destroy(&cmd->cmd_with_args);
	redirlist_destroy(&cmd->redirections);
	free(cmd);
}

int	command_specialise(t_simple_command *cmd, t_cmd_type type)
{
	if (!cmd)
		return (!SUCCESS);
	if (cmd->cmd_type != CMD_NULL)
		return (!SUCCESS);
	if (type == CMD_EXEC)
	{
		if (charptr_array_init(&cmd->cmd_with_args) != SUCCESS)
			return (!SUCCESS);
	}
	cmd->cmd_type = type;
	return (SUCCESS);
}

void	cmdlist_destroy(t_cmdlist **commands)
{
	ft_lstclear(commands, command_destroy);
}

t_simple_command	*cmdlist_add_cmd(t_cmdlist **cmdlist)
{
	t_simple_command	*cmd;
	t_cmdlist			*new_node;

	if (SUCCESS != command_init(&cmd))
		return (NULL);
	new_node = ft_lstnew(cmd);
	if (!new_node)
		return (command_destroy(&cmd), NULL);
	ft_lstadd_back(cmdlist, new_node);
	return (cmd);
}

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
