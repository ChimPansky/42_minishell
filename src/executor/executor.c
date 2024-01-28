#include "../minishell.h"
#include "executor.h"
#include <sys/wait.h>

void init_executor(t_executor *exec, int num_of_cmds) {
	exec->fd_in = STDIN_FILENO;
	exec->fd_out = STDOUT_FILENO;
	exec->exit_code = EXIT_FAILURE;
	exec->num_of_cmds = num_of_cmds;
	if (num_of_cmds == 0)
		(printf("cmds is empty. this should not happen"), exit(EXIT_FAILURE));
}

void destroy_executor(t_executor *exec) {
	if (exec->fd_in > 2)
		close(exec->fd_in);
	if (exec->fd_out > 2)
		close(exec->fd_out);
}

// waitpid errors seems to be okay to ignore
void	wait_with_check(pid_t* pids, t_executor *executor)
{
	int i = 0;

	while (i < executor->num_of_cmds)
	{
		if (pids[i] < 0)
		{
			executor->exit_code = EXIT_FAILURE;
			break;
		}
		if (g_sigint_received)
			kill(pids[i++], SIGKILL);
		if (waitpid(pids[i], &executor->exit_code, WNOHANG) == 0)
			usleep(1000);
		else
			i++;
	}
}

bool try_execute_built_in(t_msh *msh, t_simple_command *cmd, t_executor *executor)
{
	t_built_in func;

	if (!cmd || !cmd->cmd_with_args)
		ft_dprintf(STDERR_FILENO, "cmd is empty. this should not happen\n"), exit(EXIT_FAILURE);
	func = get_built_in_by_name(cmd->cmd_with_args[0]);
	if (func == NULL)
		return false;
	if (process_redirections(executor, cmd->redirections) != SUCCESS)
	{
		executor->exit_code = EXIT_FAILURE;
		return true;
	}
	executor->exit_code = func(msh, cmd->cmd_with_args, executor->fd_out);
	return true;
}

// if no success frome execute_??_chain, exit code in executor ain't changed
int execute(t_msh *msh, t_command_chain *cmds)
{
	t_executor executor;

	init_executor(&executor, ft_lstsize(cmds));
	if (executor.num_of_cmds > 1 || !try_execute_built_in(msh, cmds->content, &executor))
		execute_on_chain(msh, cmds, &executor);
	msh->last_exit_code = executor.exit_code;
	destroy_executor(&executor);
	return SUCCESS;
}
