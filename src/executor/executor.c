#include "../minishell.h"
#include "executor.h"
#include <errno.h>
#include <sys/wait.h>

void init_executor(t_executor *exec) {
	exec->fd_in = STDIN_FILENO;
	exec->fd_in = STDOUT_FILENO;
	exec->pid = 0;
}

void destroy_executor(t_executor *exec) {
	if (exec->fd_in != STDIN_FILENO)
		close(exec->fd_in);
	if (exec->fd_out != STDIN_FILENO)
		close(exec->fd_out);
}

// noreturn if found
// fileno in child process always STDOUT_FILENO
void try_exec_built_in(t_msh *msh, char **cmd_with_args)
{
	const t_built_in func = get_built_in_by_name(cmd_with_args[0]);

	if (func == NULL)
		return ;
	exit(func(msh, cmd_with_args, STDOUT_FILENO));
}

char *try_find_in_path(t_msh *msh, const char *exec)
{
	const char	*path = find_env(msh, "PATH");
	char 		**path_entries;
	char		*exec_in_path;

	if (path == NULL || is_empty(path))
		return NULL;
	path_entries = ft_split(path, ":"); // <-- MALLOC
	if (path_entries == NULL)
		perror("ft_split"), exit(EXIT_FAILURE);
	while (*path_entries)
	{
		exec_in_path = ft_strnjoin(3, *path_entries++, "/", exec);
		if (!exec_in_path)
			free(path_entries), perror("ft_strnjoin"), exit(EXIT_FAILURE);
		if (access(exec_in_path, F_OK) == SUCCESS)
			return exec_in_path;
			// return (free(path_entries), exec_in_path);
		free(exec_in_path);
		path_entries++;
	}
	return NULL;
}

// is it garanteed that cmd_with args is not empty?
// exit code?
// after fork
// is it needed to free and close?
int execute_in_child_process(t_msh *msh, char **cmd_with_args, t_executor *executor)
{
	char *const *envp = vars_convert_to_array(msh->env); // <-- MALLOC
	const char *exec = cmd_with_args[0];
	char *exec_with_path;

	if (NULL == envp
			|| STDIN_FILENO != dup2(executor->fd_in, STDIN_FILENO) // <-- OPEN
			|| STDOUT_FILENO != dup2(executor->fd_out, STDOUT_FILENO)) // <-- OPEN
		perror(NULL), exit(EXIT_FAILURE);
	exec_with_path = NULL;
	if (NULL == strchr(exec, '/'))
	{
		try_exec_built_in(msh, cmd_with_args);
		exec_with_path = try_find_in_path(msh, exec);
	}
	else if (SUCCESS == access(exec, F_OK))
		exec_with_path = (char *)exec;
	if (exec_with_path == NULL)
		ft_printf_fd(STDERR_FILENO, "msh: command not found: %s\n", exec), exit(EXIT_COMMAND_NOT_FOUND);
	else if (SUCCESS != access(exec_with_path, X_OK))
		ft_printf_fd(STDERR_FILENO, "msh: permission denied: %s\n", exec_with_path), exit(EXIT_PERMISSION_DENIED);
	execve(exec_with_path, cmd_with_args, envp);
	perror("execve"), exit(errno);
}

int no_chain_execute_external(t_msh *msh, char **cmd_with_args, t_executor *executor)
{
	int res;
	pid_t pid;

	pid = fork();
	if (pid < 0)
		return perror("fork"), EX_FAILURE;
	if (pid == 0)
		execute_in_child_process(msh, cmd_with_args, executor);
	else
	{
		if (waitpid(pid, &res, 0) < 0)
			return (perror("waitpid"), EX_FAILURE);
		return res;
	}
}

// is it 0 in waitpid options?
// waitpid errors seems to be okay to ignore
int execute_no_chain(t_msh *msh, t_simple_command *cmd)
{
	t_executor exec;
	t_built_in func;
	int res;

	init_executor(&exec);
	if (!cmd || !cmd->cmd_with_args)
		return (printf("cmd is empty. this should not happen"), EX_FAILURE);
	t_redirections *curr;
	curr = cmd->redirections;
	while (curr)
	{
		if (process_redirection(curr->content, &exec.fd_in, &exec.fd_out) == EX_FAILURE)
			return (destroy_executor(&exec), EX_FAILURE);
		curr = curr->next;
	}
	func = get_built_in_by_name(cmd->cmd_with_args[0]);
	if (func != NULL)
		res = func(msh, cmd->cmd_with_args, exec.fd_out);
	else
		res = no_chain_execute_external(msh, cmd->cmd_with_args, &exec);
	return (destroy_executor(&exec), res);
}

// empty cmd check
int execute_one_on_chain(t_msh *msh, t_simple_command *cmd, t_executor *executor)
{
	t_built_in func;
	int res;

	t_redirections *curr;
	curr = cmd->redirections;
	while (curr)
	{
		if (process_redirection(curr->content, &executor->fd_in, &executor->fd_out) == EX_FAILURE)
			return (destroy_executor(executor), EX_FAILURE);
		curr = curr->next;
	}
	execute_in_child_process(msh, cmd->cmd_with_args, executor);
}

// is it 0 in waitpid options?
// waitpid errors seems to be okay to ignore
int execute_on_chain(t_msh *msh, t_command_chain *cmds, int cmd_num)
{
	int idx;
	int res;
	pid_t* const pids = malloc(cmd_num * sizeof(pid_t));
	if (!pids)
		return perror(NULL), EX_FAILURE;

	idx = 0;
	t_executor executor;
	init_executor(&executor);
	while (cmds->next) {
		if (pipe(executor.pipe_fds) < 0)
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
			close(executor.pipe_fds[RD_END]), close(executor.fd_out);
			executor.fd_out = executor.pipe_fds[WR_END];
			execute_one_on_chain(msh, cmds->content, &executor);
		}
		else
		{
			close(executor.pipe_fds[WR_END]), close(executor.fd_in);
			executor.fd_in = executor.pipe_fds[RD_END];
		}
		idx++;
		cmds = cmds->next;
	}
	pids[idx] = fork();
	if (pids[idx] < 0)
	{
		// free, message, return or wait created and return
	}
	if (pids[idx] == 0)
		execute_one_on_chain(msh, cmds->content, &executor);
	else
		close(executor.fd_in);
	idx = 0;
	while (idx < cmd_num)
		waitpid(pids[idx], &res, 0);
	return res;
}

int execute(t_msh *msh, t_command_chain *cmds)
{
	const int cmds_num = ft_lstsize(cmds);
	if (cmds_num == 0)
		return (printf("cmds is empty. this should not happen"), 0);
	if (cmds_num == 1)
		return (execute_no_chain(msh, cmds->content));
	else
		return (execute_on_chain(msh, cmds, cmds_num));
	return 0;
}
