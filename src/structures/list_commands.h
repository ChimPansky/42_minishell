#ifndef LIST_COMMANDS_H
#define LIST_COMMANDS_H

#include "libft.h"
#include "redirs.h"


typedef t_list t_commandlist;

typedef struct s_simple_command
{
	char	**cmd_with_args;
	t_list	*redirections; // list of t_redir_details...
}		t_simple_command;

// list_commands.c
t_simple_command	*command_add(t_commandlist **commands, char **cmd_with_args, t_list *redirections);
void 				command_destroy(void *command_void);
void commandlist_destroy(t_commandlist **commands);
void				print_commands(t_commandlist **commands);

#endif  // LIST_COMMANDS_H