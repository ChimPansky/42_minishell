/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_on_chain.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasbari <thomas.kasbarian@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 19:42:48 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/09 22:37:57 by tkasbari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <sys/wait.h>

static int	execute_one_on_chain(
	t_msh *msh,
	t_simple_command *cmd,
	t_executor *executor)
{
	executor->is_parent = false;
	configure_signals(SIG_EXECUTOR);
	msh->last_exit_code = EXIT_FAILURE;
	if (process_redirections(executor, cmd->redirections) != SUCCESS)
		return (SUCCESS);
	if (STDIN_FILENO != dup2(executor->fd_in, STDIN_FILENO))
		return (perror("execute_one_on_chain: dup2"), SUCCESS);
	if (STDOUT_FILENO != dup2(executor->fd_out, STDOUT_FILENO))
		return (perror("execute_one_on_chain: dup2"),
			close(STDIN_FILENO), SUCCESS);
	if (cmd->cmd_type == CMD_SUBSHELL)
	{
		parse_and_execute(msh, cmd->subcommand);
		return (close(STDIN_FILENO), close(STDOUT_FILENO), SUCCESS);
	}
	else if (cmd->cmd_type == CMD_EXEC)
	{
		execute_in_child_process(msh, cmd->cmd_with_args.buf);
		return (close(STDIN_FILENO), close(STDOUT_FILENO), SUCCESS);
	}
	msh->last_exit_code = EXIT_SUCCESS;
	return (close(STDIN_FILENO), close(STDOUT_FILENO), SUCCESS);
}

static int	execute_to_pipe(
	t_msh *msh,
	t_executor *executor,
	t_simple_command *cmd,
	pid_t *pid)
{
	int	pipe_fds[2];

	if (pipe(pipe_fds) < 0)
		return (perror("execute_to_pipe: pipe"), !SUCCESS);
	*pid = fork();
	if (*pid < 0)
		return (perror("execute_to_pipe: fork"),
			close(pipe_fds[0]), close(pipe_fds[1]), !SUCCESS);
	if (*pid == 0)
	{
		(void)(close(pipe_fds[RD_END]), close(executor->fd_out));
		executor->fd_out = pipe_fds[WR_END];
		return (execute_one_on_chain(msh, cmd, executor), SUCCESS);
	}
	close(pipe_fds[WR_END]);
	(void)(executor->fd_in > 2 && close(executor->fd_in));
	executor->fd_in = pipe_fds[RD_END];
	return (SUCCESS);
}

int	execute_on_chain(t_msh *msh, t_executor *executor, t_cmdlist *cmds)
{
	int	idx;

	msh->last_exit_code = EXIT_FAILURE;
	idx = 0;
	while (cmds->next && !g_signal_no)
	{
		if (execute_to_pipe(msh, executor, cmds->content, &executor->pids[idx])
			!= SUCCESS)
			return (!SUCCESS);
		if (!executor->is_parent)
			return (SUCCESS);
		idx++;
		cmds = cmds->next;
	}
	if (g_signal_no)
		return (!SUCCESS);
	executor->pids[idx] = fork();
	if (executor->pids[idx] < 0)
		return (perror("execute_on_chain: fork"), !SUCCESS);
	if (executor->pids[idx] == 0)
		execute_one_on_chain(msh, cmds->content, executor);
	return (SUCCESS);
}
