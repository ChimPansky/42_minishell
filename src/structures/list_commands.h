/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:52:18 by tkasbari          #+#    #+#             */
/*   Updated: 2024/02/07 17:52:19 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_COMMANDS_H
#define LIST_COMMANDS_H

#include "list_tokens.h"
#include "redirs.h"
#include "ft_list.h"

typedef t_list t_cmdlist;

typedef enum e_cmd_type
{
	CMD_NULL,
	CMD_EXEC,
	CMD_SUBSHELL
}		t_cmd_type;

// simple command doesn't own subcommand, and shouldn't free it
typedef struct s_simple_command
{
	t_cmd_type	cmd_type;
	union
	{
		t_charptr_array	cmd_with_args;
		t_tokenlist		*subcommand;
	};
	t_redirlist	*redirections;
}		t_simple_command;

int					command_specialise(t_simple_command *cmd, t_cmd_type type);
void				cmdlist_destroy(t_cmdlist **commands);
t_simple_command	*cmdlist_add_cmd(t_cmdlist **cmdlist);

// only for debugging:
void				cmdlist_print(t_cmdlist **commands);

#endif  // LIST_COMMANDS_H
