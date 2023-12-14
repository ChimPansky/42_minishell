#include "minishell.h"
#include <errno.h>

char **env;

t_built_in get_built_in_by_name(char *func_name);

// noreturn built-in
void pwd(t_msh *msh, char **cmd_with_args)
{
	(void) msh;
	ft_printf("yikes! u called pwd!");
}

char *find_env(t_msh *msh, const char *var_name)
{
	(void) msh;
	(void) var_name;
	return "/bin:/usr/bin";
}

bool is_empty(const char *str)
{
	return (str != NULL && *str == '\0');
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
	path_entries = ft_split(path, ':');
	if (path_entries == NULL)
		perror("ft_split"), exit(EXIT_FAILURE);
	while (*path_entries)
	{
		exec_in_path = ft_strnjoin(3, *path++, "/", exec);
		if (!exec_in_path)
			free(path_entries), perror("ft_strnjoin"), exit(EXIT_FAILURE);
		if (access(exec_in_path, F_OK) == SUCCESS)
			return (free(path_entries), exec_in_path);
		free(exec_in_path);
		path_entries++;
	}
	return NULL;
}

// noreturn, every
void pipex(t_msh *msh, char **cmd_with_arguments, int in_fd, int out_fd) {
	const char *exec = cmd_with_arguments[0];
	char *exec_with_path;

	dup2(in_fd, STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);
	exec_with_path = NULL;
	if (NULL == strchr(exec, '/'))
	{
		try_exec_built_in(msh, cmd_with_arguments);
		exec_with_path = try_find_in_path(msh, exec);
	}
	else if (SUCCESS == access(exec, F_OK))
		exec_with_path = (char *)exec;
	if (exec_with_path == NULL)
		ft_printf_fd(STDERR_FILENO, "msh: command not found: %s", exec), exit(EXIT_COMMAND_NOT_FOUND);
	else if (SUCCESS != access(exec, F_EXLCK))
		ft_printf_fd(STDERR_FILENO, "msh: permission denied: %s", exec), exit(EXIT_PERMISSION_DENIED);
	execve(exec, cmd_with_arguments, env);
	perror("execve"), exit(errno);
}
