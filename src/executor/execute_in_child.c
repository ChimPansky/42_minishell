#include "executor.h"
#include "ft_charptr_array.h"
#include <unistd.h>

// fileno in child process always STDOUT_FILENO
static int try_exec_built_in(t_msh *msh, char **cmd_with_args)
{
	const t_built_in func = get_built_in_by_name(cmd_with_args[0]);

	if (func == NULL)
		return (!SUCCESS);
	msh->last_exit_code = func(msh, cmd_with_args, STDOUT_FILENO);
	return (SUCCESS);
}

// todo: if no path - check in getcwd
// ignore directories on path
static int	try_find_in_path(t_msh *msh, const char *exec, char **exec_in_path)
{
	const char	*path = varlist_get_value(msh->env, "PATH");
	char 		**path_entries;
	size_t 		i;

	*exec_in_path = NULL;
	if (path == NULL || !*path)
		return SUCCESS;
	path_entries = ft_split(path, ":");
	if (path_entries == NULL)
		return (perror("ft_split"), !SUCCESS);
	i = 0;
	while (path_entries[i])
	{
		*exec_in_path = ft_strnjoin(3, path_entries[i], "/", exec);
		if (!*exec_in_path)
			return (free(path_entries), perror("ft_strnjoin"), !SUCCESS);
		// if not dir and exist
		if (access(*exec_in_path, F_OK) == SUCCESS)
			return (free(path_entries), SUCCESS);
		free(*exec_in_path);
		i++;
	}
	free(path_entries);
	*exec_in_path = NULL;
	return SUCCESS;
}

// is it garanteed that cmd_with args is not empty?
// exit code?
// after fork
// is it needed to free and close?
// TODO check for directories, etc
// return SUCCESS if last exit code was set
int	execute_in_child_process(t_msh *msh, char **cmd_with_args)
{
	t_charptr_array		envp;
	const char			*exec = cmd_with_args[0];
	char				*exec_with_path;

	exec_with_path = NULL;
	msh->last_exit_code = EXIT_FAILURE;
	if (NULL == strchr(exec, '/'))
	{
		if (try_exec_built_in(msh, cmd_with_args) == SUCCESS)
			return (SUCCESS);
		if (try_find_in_path(msh, exec, &exec_with_path) != SUCCESS)
		 	return (!SUCCESS);
	}
	else if (SUCCESS == access(exec, F_OK))
	{
		exec_with_path = ft_strdup(exec);
		if (!exec_with_path)
			return (perror("strdup"), !SUCCESS);
	}
	if (exec_with_path == NULL)
	{
		msh->last_exit_code = EXIT_COMMAND_NOT_FOUND;
		return (ft_printf_err("msh: command not found: %s\n", exec), SUCCESS);
	}
	// if is dir
	else if (SUCCESS != access(exec_with_path, X_OK))
	{
		msh->last_exit_code = EXIT_PERMISSION_DENIED;
		return (ft_printf_err("msh: permission denied: %s\n", exec_with_path), free(exec_with_path), SUCCESS);
	}
	if (varlist_convert_to_array(msh->env, &envp) != SUCCESS)
		return (perror("msh: "), free(exec_with_path), !SUCCESS);
	configure_signals(SIG_EXECUTOR);
	execve(exec_with_path, cmd_with_args, envp.buf);
	return (perror("msh: exeve"), charptr_array_destroy(&envp), free(exec_with_path), !SUCCESS);
}
