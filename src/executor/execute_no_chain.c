#include <sys/wait.h>
#include "executor.h"

int no_chain_execute_external(t_msh *msh, char **cmd_with_args, t_executor *executor)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		return perror("fork"), EX_FAILURE;
	if (pid == 0)
		execute_in_child_process(msh, cmd_with_args, executor);
	else
	{
		if (waitpid(pid, &executor->ret_code, 0) < 0)
			return (perror("waitpid"), EX_FAILURE);
	}
	return SUCCESS;
}

// is it 0 in waitpid options?
// waitpid errors seems to be okay to ignore
// 
int execute_no_chain(t_msh *msh, t_simple_command *cmd, t_executor *executor)
{
	t_built_in func;

	if (!cmd || !cmd->cmd_with_args)
		return (printf("cmd is empty. this should not happen"), EX_FAILURE);
	if (process_redirections(executor, cmd->redirections) != SUCCESS)
		return EX_FAILURE;
	func = get_built_in_by_name(cmd->cmd_with_args[0]);
	if (func != NULL)
		executor->ret_code = func(msh, cmd->cmd_with_args, executor->fd_out);
	else
		return (no_chain_execute_external(msh, cmd->cmd_with_args, executor));
	return (SUCCESS);
}
