#include "../minishell.h"
#include "executor.h"

void init_executor(t_executor *exec) {
	exec->fd_in = STDIN_FILENO;
	exec->fd_out = STDOUT_FILENO;
	exec->ret_code = EX_FAILURE;
}

void destroy_executor(t_executor *exec) {
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (exec->fd_out != STDIN_FILENO)
		close(exec->fd_out);
}

int execute(t_msh *msh, t_command_chain *cmds)
{
	const int cmds_num = ft_lstsize(cmds);
	t_executor executor;
	int res;

	if (cmds_num == 0)
		return (printf("cmds is empty. this should not happen"), 0);
	init_executor(&executor);
	if (cmds_num == 1)
		execute_no_chain(msh, cmds->content, &executor);
	else
		execute_on_chain(msh, cmds, cmds_num, &executor);
	destroy_executor(&executor);
	return 0;
}
