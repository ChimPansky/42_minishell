#ifndef EXECUTOR_H
#define EXECUTOR_H

# include "../minishell.h"

# define RD_END 0
# define WR_END 1
# define BROKEN_PIPE -1

typedef struct s_executor
{
	int fd_in;
	int fd_out;
	int exit_code;
	int num_of_cmds;
}		t_executor;

int		process_redirections(t_executor *executor, t_redirections *redirs);
int		execute_no_chain(t_msh *msh, t_simple_command *cmd, t_executor *executor);
int		execute_on_chain(t_msh *msh, t_cmdlist *cmds, t_executor *executor);
void	wait_with_check(pid_t* pids, t_executor *executor);
// noreturn
void	execute_in_child_process(t_msh *msh, char **cmd_with_args);

#endif  // EXECUTOR_H
