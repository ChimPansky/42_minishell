/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vvilensk <vilenskii.v@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 19:41:54 by vvilensk          #+#    #+#             */
/*   Updated: 2024/02/10 00:28:32 by vvilensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "executor.h"
#include <stdlib.h>
#include <sys/wait.h>

int	init_executor(t_executor *exec, int num_of_cmds)
{
	exec->fd_in = STDIN_FILENO;
	exec->fd_out = STDOUT_FILENO;
	exec->num_of_cmds_in_pipe = num_of_cmds;
	exec->is_parent = true;
	exec->pids = malloc(num_of_cmds * sizeof(pid_t));
	if (!exec->pids)
		return (perror("init_executor: malloc"), !SUCCESS);
	while (num_of_cmds--)
		exec->pids[num_of_cmds] = -1;
	return (SUCCESS);
}

void	destroy_executor(t_executor *exec)
{
	if (exec->fd_in > 2)
		close(exec->fd_in);
	if (exec->fd_out > 2)
		close(exec->fd_out);
	free(exec->pids);
}

// waitpid errors seems to be okay to ignore
// TODO if the current process exited with WIFSIGNALED kill the others
//  , if not, keep waiting
void	wait_with_check(t_executor *executor, int *last_exit_code)
{
	int	i;
	int	status;

	i = 0;
	while (i < executor->num_of_cmds_in_pipe)
	{
		if (executor->pids[i] < 0)
		{
			*last_exit_code = EXIT_FAILURE;
			break ;
		}
		if (waitpid(executor->pids[i], &status, WNOHANG) == 0)
			usleep(1000);
		else
			i++;
	}
	if (WIFEXITED(status))
		*last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*last_exit_code = WTERMSIG(status) + 128;
}

bool	try_execute_built_in(
	t_msh *msh,
	t_simple_command *cmd,
	t_executor *executor)
{
	t_built_in	func;

	if (cmd->cmd_type != CMD_EXEC)
		return (false);
	func = get_built_in_by_name(cmd->cmd_with_args.buf[0]);
	if (func == NULL)
		return (false);
	executor->num_of_cmds_in_pipe = 0;
	if (process_redirections(executor, cmd->redirections) != SUCCESS)
		msh->last_exit_code = EXIT_FAILURE;
	else
		msh->last_exit_code
			= func(msh, cmd->cmd_with_args.buf, executor->fd_out);
	return (true);
}

// what if execute on chain returned !SUCCESS meaning sistem error?
int	execute(t_msh *msh, t_cmdlist *cmds)
{
	t_executor	executor;
	const int	num_of_cmds = ft_lstsize(cmds);

	if (num_of_cmds == 0)
		return (SUCCESS);
	if (init_executor(&executor, num_of_cmds) != SUCCESS)
		return (SUCCESS);
	if (num_of_cmds > 1 || !try_execute_built_in(msh, cmds->content, &executor))
		execute_on_chain(msh, &executor, cmds);
	(void)(executor.fd_in > 2 && close(executor.fd_in));
	if (executor.is_parent && executor.num_of_cmds_in_pipe > 0)
		wait_with_check(&executor, &msh->last_exit_code);
	if (!executor.is_parent)
		msh->done = true;
	destroy_executor(&executor);
	return (SUCCESS);
}
