/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:43:57 by tkasbari          #+#    #+#             */
/*   Updated: 2024/01/14 17:39:49 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

t_simple_command	*command_add(t_command_chain **commands, char **cmd_with_args, t_list *redirections)
{
	t_simple_command *command;

	command = malloc(sizeof(t_simple_command));
	if (!command)
		return NULL;
	ft_bzero(command, sizeof(t_simple_command));
	//command->cmd_with_args = ...;
	//command->redirections = ...;
	(void)commands;
	(void)cmd_with_args;
	(void)redirections;

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

void	print_commands(t_command_chain **commands)
{
	t_command_chain		*cmd_list;
	t_simple_command	*cur_cmd;
	int			i;

	if (!commands)
		return ;
	cmd_list = *commands;
	i = 0;
	while (cmd_list && cmd_list->content)
	{
		cur_cmd = cmd_list->content;
		printf("%p <--(Command Nr. %d:)-->%p", cmd_list->prev, i, cmd_list->next);
		cmd_list = cmd_list->next;
		i++;
	}
	(void)cur_cmd;
}
