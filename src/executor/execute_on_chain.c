#include "executor.h"
#include <sys/wait.h>

// noreturn
static void execute_one_on_chain(t_msh *msh, t_simple_command *cmd, t_executor *executor)
{
	if (process_redirections(executor, cmd->redirections) != SUCCESS)
		exit(EXIT_FAILURE);
	if (STDIN_FILENO != dup2(executor->fd_in, STDIN_FILENO))
		perror("msh: executor: dup2"), exit(EXIT_FAILURE);
	if (STDOUT_FILENO != dup2(executor->fd_out, STDOUT_FILENO))
		perror("msh: executor: dup2"), close(STDIN_FILENO), exit(EXIT_FAILURE);
	if (cmd->cmd_type == CMD_SUBSHELL)
	{
		if (parse_and_ecexute(msh, cmd->subcommand) != SUCCESS)
			close(STDIN_FILENO), close(STDOUT_FILENO), exit(EXIT_FAILURE);
		close(STDIN_FILENO), close(STDOUT_FILENO), exit(msh->last_exit_code);
	}
	else if (cmd->cmd_type == CMD_EXEC)
	{
		execute_in_child_process(msh, cmd->cmd_with_args.buf);
		close(STDIN_FILENO), close(STDOUT_FILENO), exit(msh->last_exit_code);
	}
	close(STDIN_FILENO), close(STDOUT_FILENO), exit(EXIT_SUCCESS);
}

static int execute_cmds(t_msh *msh, t_executor *executor, t_cmdlist **cmds, pid_t* pids)
{
	int idx;
	int pipe_fds[2];

	idx = 0;
	while ((*cmds)->next) {
		if (g_sigint_received)
			return !SUCCESS;
		if (pipe(pipe_fds) < 0)
			return perror("pipe"), !SUCCESS;
		pids[idx] = fork();
		if (pids[idx] < 0)
			return perror("fork"), close(pipe_fds[0]), close(pipe_fds[1]), !SUCCESS;
		if (pids[idx] == 0)
		{
			close(pipe_fds[RD_END]), close(executor->fd_out);
			executor->fd_out = pipe_fds[WR_END];
			execute_one_on_chain(msh, (*cmds)->content, executor);
		}
		else
		{
			if (executor->fd_in > 2)
				close(executor->fd_in);
			close(pipe_fds[WR_END]);
			executor->fd_in = pipe_fds[RD_END];
		}
		idx++;
		*cmds = (*cmds)->next;
	}
	pids[idx] = fork();
	if (pids[idx] < 0)
		return perror("fork"), !SUCCESS;
	if (pids[idx] == 0)
		execute_one_on_chain(msh, (*cmds)->content, executor);
	return SUCCESS;
}

// is it 0 in waitpid options?
// waitpid errors seems to be okay to ignore
int execute_on_chain(t_msh *msh, t_cmdlist *cmds, t_executor *executor)
{
	int				i;
	pid_t *const	pids = malloc(executor->num_of_cmds * sizeof(pid_t));

	if (!pids)
		return perror(NULL), !SUCCESS;
	i = 0;
	while (i < executor->num_of_cmds)
		pids[i++] = -1;
	execute_cmds(msh, executor, &cmds, pids);
	wait_with_check(pids, executor);
	free(pids);
	return SUCCESS;
}
