#include "executor.h"
#include <sys/wait.h>

// empty cmd check
int execute_one_on_chain(t_msh *msh, t_simple_command *cmd, t_executor *executor)
{
	if (process_redirections(executor, cmd->redirections) != SUCCESS)
		return EX_FAILURE;
	return (execute_in_child_process(msh, cmd->cmd_with_args, executor));
}

int execute_cmds_but_last(t_msh *msh, t_executor *executor, t_command_chain **cmds, pid_t* pids)
{
	int idx;

	idx = 0;
	while ((*cmds)->next) {
		if (pipe(executor->pipe_fds) < 0)
		{
			// free, message, return or wait created and return
		}
		pids[idx] = fork();
		if (pids[idx] < 0)
		{
			// free, message, return or wait created and return
		}
		if (pids[idx] == 0)
		{
			close(executor->pipe_fds[RD_END]), close(executor->fd_out);
			executor->fd_out = executor->pipe_fds[WR_END];
			execute_one_on_chain(msh, (*cmds)->content, executor);
		}
		else
		{
			if (executor->fd_in > 2)
				close(executor->fd_in);
			close(executor->pipe_fds[WR_END]);
			executor->fd_in = executor->pipe_fds[RD_END];
		}
		idx++;
		*cmds = (*cmds)->next;
	}
	return SUCCESS;
}

int execute_last_cmd(t_msh *msh, t_executor *executor, t_simple_command *last_cmd, pid_t* pid)
{
	*pid = fork();
	if (*pid < 0)
	{
		// free, message, return or wait created and return
	}
	if (*pid == 0)
		execute_one_on_chain(msh, last_cmd, executor);
	else
		close(executor->fd_in);
	return SUCCESS;
}

// is it 0 in waitpid options?
// waitpid errors seems to be okay to ignore
int execute_on_chain(t_msh *msh, t_command_chain *cmds, int cmd_num, t_executor *executor)
{
	int idx;
	pid_t* const pids = malloc(cmd_num * sizeof(pid_t));
	if (!pids)
		return perror(NULL), EX_FAILURE;
	if (execute_cmds_but_last(msh, executor, &cmds, pids) != SUCCESS)
	{

	}
	if (execute_last_cmd(msh, executor, cmds->content, &pids[cmd_num - 1]) != SUCCESS)
	{

	}
	idx = 0;
	printf ("\nAAAAAAAAAAAAAAAAA_%d_AAAAAAAAAAAAAAAA_%d\n", pids[0], pids[1]);
	while (idx < cmd_num)
		waitpid(pids[idx++], &executor->ret_code, 0);
	return SUCCESS;
}
