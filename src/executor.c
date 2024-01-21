#include "minishell.h"
#include <errno.h>
#include <sys/wait.h>


int execute_in_subshell(t_msh *msh, char **cmd_with_args)
{
    // char **curr_token;

    msh->pid_to_wait = fork();
    if (msh->pid_to_wait != 0) {
        int ret;
        waitpid(msh->pid_to_wait, &ret, 0);
        return ret;
    }
    // printf("IM CHILD\n");
    execute_by_cmd_with_args(msh, cmd_with_args);
    return 0;
}


int one_cmd_in_chain_execute(t_msh *msh, t_simple_command *cmd)
{
    int res;
    t_built_in func;
    if (!cmd || !cmd->cmd_with_args)
        return (printf("cmd is empty. this should not happen"), 0);
    int in_fd = -1, out_fd = -1;
    t_redirections curr;
    curr = cmd->redirections;
    while (curr)
    {
        process_redirection(&in_fd, &out_fd);    
        curr = curr->next;
    }
    func = get_built_in_by_name(cmd_with_args[0]);    
	if (func != NULL)
        res = func(msh, cmd->cmd_with_args, out_fd);
    else
        res = execute_external(msh, cbd->cmd_with_args, in_fd, out_fd);
    ((in_fd > 2 && close(in_fd)), (out_fd > 0 && close(out_fd)));

}

int execute(t_msh *msh, t_command_chain *cmds)
{
    if (!cmds)
        return (printf("cmds is empty. this should not happen"), 0);
    if (cmds && !cmds->next)
        return (one_cmd_in_chain_execute(msh, cmds->content));
    else
        execute_on_chain(msh, cmds);
    return 0;
}


// noreturn if found
void try_exec_built_in(t_msh *msh, char **cmd_with_args)
{
	const t_built_in func = get_built_in_by_name(cmd_with_args[0]);

	if (func == NULL)
		return ;
	exit(func(msh, cmd_with_args));
}

char *try_find_in_path(t_msh *msh, const char *exec)
{
	const char	*path = find_env(msh, "PATH");
	char 		**path_entries;
	char		*exec_in_path;

	if (path == NULL || is_empty(path))
		return NULL;
	path_entries = ft_split(path, ":");
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

// noreturn, call only in child process
void execute_by_cmd_with_args(t_msh *msh, char **cmd_with_arguments) {
	const char *exec = cmd_with_arguments[0];
	char *const *envp = vars_convert_to_array(msh->env);
	char *exec_with_path;

	if (NULL == envp
			|| STDIN_FILENO != dup2(msh->in_fd, STDIN_FILENO)
			|| STDOUT_FILENO != dup2(msh->out_fd, STDOUT_FILENO))
		perror(NULL), exit(EXIT_FAILURE);
	exec_with_path = NULL;
	if (NULL == strchr(exec, '/'))
	{
		try_exec_built_in(msh, cmd_with_arguments);
		exec_with_path = try_find_in_path(msh, exec);
	}
	else if (SUCCESS == access(exec, F_OK))
		exec_with_path = (char *)exec;
	if (exec_with_path == NULL)
		ft_printf_fd(STDERR_FILENO, "msh: command not found: %s\n", exec), exit(EXIT_COMMAND_NOT_FOUND);
	else if (SUCCESS != access(exec_with_path, X_OK))
		ft_printf_fd(STDERR_FILENO, "msh: permission denied: %s\n", exec_with_path), exit(EXIT_PERMISSION_DENIED);
	execve(exec_with_path, cmd_with_arguments, envp);
	perror("execve"), exit(errno);
}
