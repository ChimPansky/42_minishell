#include "minishell.h"
#include <errno.h>


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

// tokens gonna be linked list
int execute(t_msh *msh, t_command_chain *cmd)
{
    char **tokens = ft_split(input, " \t"); // (ft_split, many seps)
    // int pipefds[2];
    char *cmd_with_args[10] = {};
    int i = 0;

    if (!tokens)
        exit(EXIT_FAILURE);
    // char **cur_start_token = tokens;
    while (*tokens)
    {
        if (ft_strncmp(*tokens, ">", 2) == SUCCESS) {
            // *tokens = "SKIP";
            tokens++;
            // close(msh->out_fd); // in subprocess to not close stdin/out/err
            msh->out_fd = open(*tokens, O_TRUNC | O_CREAT | O_WRONLY, 0644);
            // *tokens = "SKIP";
        } else if (ft_strncmp(*tokens, "<", 2) == SUCCESS) {
            // *tokens = "SKIP";
            tokens++;
            // close(msh->in_fd);
            msh->in_fd = open(*tokens, O_RDONLY);
            // *tokens = "SKIP";
        } else {
            cmd_with_args[i++] = ft_strdup(*tokens);
        }
        tokens++;
    }
    cmd_with_args[i] = NULL;
    print_splitted(cmd_with_args);
    t_built_in f = get_built_in_by_name(cmd_with_args[0]);
    if (f != NULL)
        msh->last_exit_code = f(msh, cmd_with_args);
    else
        msh->last_exit_code = execute_in_subshell(msh, cmd_with_args);
    msh->in_fd = STDIN_FILENO;
    msh->out_fd = STDOUT_FILENO;
    msh->err_fd = STDERR_FILENO;
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
			|| STDOUT_FILENO != dup2(msh->out_fd, STDOUT_FILENO)
			|| STDERR_FILENO != dup2(msh->err_fd, STDERR_FILENO))
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
