#ifndef EXECUTOR_H
#define EXECUTOR_H

# include "../minishell.h"

# define EX_FAILURE -2
# define RD_END 0
# define WR_END 1

typedef struct s_executor
{
	int pipe_fds[2];
	int fd_in;
	int fd_out;
	int ret_code;
}		t_executor;

int process_redirections(t_executor *executor, t_redirections *redirs);
int execute_in_child_process(t_msh *msh, char **cmd_with_args, t_executor *executor);
int execute_no_chain(t_msh *msh, t_simple_command *cmd, t_executor *executor);
int execute_on_chain(t_msh *msh, t_command_chain *cmds, int cmd_num, t_executor *executor);
void init_executor(t_executor *exec);
void destroy_executor(t_executor *exec);

#endif  // EXECUTOR_H
