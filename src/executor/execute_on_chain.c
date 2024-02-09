#include "executor.h"
#include <sys/wait.h>

// noreturn
static int execute_one_on_chain(t_msh *msh, t_simple_command *cmd, t_executor *executor)
{
	executor->is_parent = false;
	if (process_redirections(executor, cmd->redirections) != SUCCESS)
		return (EXIT_FAILURE);
	if (STDIN_FILENO != dup2(executor->fd_in, STDIN_FILENO))
		return (perror("execute_one_on_chain: dup2"), EXIT_FAILURE);
	if (STDOUT_FILENO != dup2(executor->fd_out, STDOUT_FILENO))
		return (perror("execute_one_on_chain: dup2"), close(STDIN_FILENO), EXIT_FAILURE);
	if (cmd->cmd_type == CMD_SUBSHELL)
	{
		if (parse_and_execute(msh, cmd->subcommand) != SUCCESS)
			return (close(STDIN_FILENO), close(STDOUT_FILENO), EXIT_FAILURE);
		return (close(STDIN_FILENO), close(STDOUT_FILENO), msh->last_exit_code);
	}
	else if (cmd->cmd_type == CMD_EXEC)
	{
		if (execute_in_child_process(msh, cmd->cmd_with_args.buf) != SUCCESS)
			return (close(STDIN_FILENO), close(STDOUT_FILENO), EXIT_FAILURE);
		return (close(STDIN_FILENO), close(STDOUT_FILENO), msh->last_exit_code);
	}
	return (close(STDIN_FILENO), close(STDOUT_FILENO), EXIT_SUCCESS);
}

static int execute_to_pipe(t_msh *msh, t_executor *executor, t_simple_command *cmd, pid_t *pid)
{
	int pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		return (perror("execute_to_pipe: pipe"), !SUCCESS);
	*pid = fork();
	if (*pid < 0)
		return (perror("execute_to_pipe: fork"), close(pipe_fds[0]), close(pipe_fds[1]), !SUCCESS);
	if (*pid == 0)
	{
		close(pipe_fds[RD_END]), close(executor->fd_out);
		executor->fd_out = pipe_fds[WR_END];
		return (execute_one_on_chain(msh, cmd, executor));
	}
	close(pipe_fds[WR_END]);
	(executor->fd_in > 2 && close(executor->fd_in));
	executor->fd_in = pipe_fds[RD_END];
	return (EXIT_SUCCESS);
}

// is it 0 in waitpid options?
// waitpid errors seems to be okay to ignore
int execute_on_chain(t_msh *msh, t_executor *executor, t_cmdlist *cmds)
{
	int	idx;

	idx = 0;
	while (cmds->next && !(g_signal_no))
	{
		msh->last_exit_code = execute_to_pipe(msh, executor, cmds->content, &executor->pids[idx]);
		if (!executor->is_parent)
			return (SUCCESS);
		else if (msh->last_exit_code != SUCCESS)
			return (!SUCCESS);
		idx++;
		cmds = cmds->next;
	}
	if (g_signal_no)
		return (!SUCCESS);
	executor->pids[idx] = fork();
	if (executor->pids[idx] < 0)
		return perror("execute_on_chain: fork"), !SUCCESS;
	if (executor->pids[idx] == 0)
		msh->last_exit_code = execute_one_on_chain(msh, cmds->content, executor);
	return (SUCCESS);
}
