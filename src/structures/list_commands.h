#ifndef LIST_COMMANDS_H
#define LIST_COMMANDS_H

#include "redirs.h"
#include "ft_list.h"

typedef t_list t_cmdlist;

typedef enum e_cmd_type
{
	CMD_NULL,
	CMD_EXEC,
	CMD_SUBSHELL
}		t_cmd_type;

typedef struct s_simple_command
{
	t_cmd_type	cmd_type;
	union
	{
		t_charptr_array	cmd_with_args;
		t_cmdlist	*subcommand;
	};
	t_redirections	*redirections;
}		t_simple_command;

// list_commands.c
void cmdlist_destroy(t_cmdlist **commands);
t_simple_command *cmdlist_add_cmd(t_cmdlist **cmdlist, t_cmd_type type);
// void				cmdlist_print(t_cmdlist **commands);

#endif  // LIST_COMMANDS_H
